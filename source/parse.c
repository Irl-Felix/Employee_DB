#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "parse.h"
#include "common.h"

void list_employees(struct dbheader_t *dbhdr,struct employee_t *employees){
    int i = 0;
    for (; i < dbhdr->count; i++){
        printf("Employee %d\n", i);
        printf("\tName: %s\n", employees[i].name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %d\n", employees[i].hours);
        printf("\n");
    }

}

int add_employee(struct dbheader_t *dbhdr,struct employee_t **employees, char *addstring) {
    printf("%s\n", addstring);

    if (dbhdr == NULL) return STATUS_ERROR;
    if (employees == NULL) return STATUS_ERROR;
    if (addstring == NULL) return STATUS_ERROR;

    char *name = strtok(addstring, ",");
    if (name == NULL) return STATUS_ERROR;

    char *addr = strtok(NULL, ",");
    if (addr == NULL) return STATUS_ERROR;

    char *hours = strtok(NULL, ",");
    if (hours == NULL) return STATUS_ERROR;

    /* realloc handles NULL correctly */
    struct employee_t *new_array = realloc(*employees, sizeof(struct employee_t) * (dbhdr->count + 1));

    if (new_array == NULL) {
        return STATUS_ERROR;
    }

    *employees = new_array;

    int index = dbhdr->count;

    strncpy((*employees)[index].name,name,sizeof((*employees)[index].name) - 1);
    
    (*employees)[index].name[sizeof((*employees)[index].name) - 1] = '\0';

    strncpy((*employees)[index].address,addr,sizeof((*employees)[index].address) - 1);

    (*employees)[index].address[sizeof((*employees)[index].address) - 1] = '\0';

    (*employees)[index].hours = atoi(hours);

    dbhdr->count++;

    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t *dbhdr, struct employee_t **employeesOut) {
    if (fd < 0){
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    int count = dbhdr->count;

    struct employee_t *employees = calloc(count, sizeof(struct employee_t));
    if (employees == NULL){
        printf("Malloc failed\n");
        return STATUS_ERROR;
    }
    ssize_t bytes = read(fd, employees, count * sizeof(struct employee_t));

    if (bytes != count * sizeof(struct employee_t)) {
        free(employees);
        return STATUS_ERROR;
    }

    for (int i = 0; i <count;i++){
        employees[i].hours = ntohl(employees[i].hours);
    }

    *employeesOut = employees;
    return STATUS_SUCCESS;
}

int update_employee_by_name(struct dbheader_t *dbhdr,struct employee_t *employees,char *updatestring){
    if (dbhdr == NULL || employees == NULL || updatestring == NULL)
        return STATUS_ERROR;

    char *name = strtok(updatestring, ",");
    char *hours_str = strtok(NULL, ",");

    if (!name || !hours_str) {
        printf("Invalid format. Use name,hours\n");
        return STATUS_ERROR;
    }

    int new_hours = atoi(hours_str);

    if (new_hours < 0)
        return STATUS_ERROR;

    for (int i = 0; i < dbhdr->count; i++) {
        if (strcmp(employees[i].name, name) == 0) {
            employees[i].hours = new_hours;
            return STATUS_SUCCESS;
        }
    }

    printf("Employee not found\n");
    return STATUS_ERROR;
}

int remove_employee_by_name(struct dbheader_t *dbhdr,struct employee_t **employees, char *name){
    if (dbhdr == NULL || employees == NULL || *employees == NULL || name == NULL){
        return STATUS_ERROR;
    }

    struct employee_t *arr = *employees;
    int count = dbhdr->count;

    int index = -1;

    for (int i = 0; i < count; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Employee not found\n");
        return STATUS_ERROR;
    }

    for (int i = index; i < count - 1; i++) {
        arr[i] = arr[i + 1];
    }

    dbhdr->count--;

    if (dbhdr->count == 0) {
        free(arr);
        *employees = NULL;
        return STATUS_SUCCESS;
    }

    struct employee_t *tmp = realloc(arr, sizeof(struct employee_t) * dbhdr->count);

    if (!tmp) {
        return STATUS_ERROR;
    }

    *employees = tmp;

    return STATUS_SUCCESS;
}


void output_file(int fd, struct dbheader_t *dbhdr, struct employee_t *employees) {
    if (fd < 0) {
        return;
    }

    int realcount = dbhdr->count;

    struct dbheader_t tmp = *dbhdr;

    tmp.version  = htons(tmp.version);
    tmp.count    = htons(tmp.count);
    tmp.magic    = htonl(tmp.magic);
    tmp.filesize = htonl(sizeof(struct dbheader_t) + sizeof(struct employee_t) * realcount);

    lseek(fd, 0, SEEK_SET);

    write(fd, &tmp, sizeof(tmp));

    for (int i = 0; i < realcount; i++) {
        struct employee_t tmp_emp = employees[i];
        tmp_emp.hours = htonl(tmp_emp.hours);
        write(fd, &tmp_emp, sizeof(tmp_emp));
    }

    off_t new_size = sizeof(struct dbheader_t) + sizeof(struct employee_t) * realcount;

    if (ftruncate(fd, new_size) == -1) {
        perror("ftruncate");
    }
}

int validate_db_header(int fd, struct dbheader_t **headerOut){
    if (fd < 0){
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    struct dbheader_t *header = calloc(1, sizeof(struct dbheader_t ));

    if (header == NULL) {
        printf("Malloc failed to create a db header\n");
        return STATUS_ERROR;
    }
    if (read(fd,header,sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
        perror("read");
        free(header);
        return STATUS_ERROR;
    }

    header->version = ntohs(header->version);
    header->count = ntohs(header->count);
    header->magic = ntohl(header->magic);
    header->filesize = ntohl(header->filesize);

    if (header->magic != HEADER_MAGIC) {
        printf("Improper header magic\n");
        free(header);
        return STATUS_ERROR;
    }

    if (header->version != 1) {
        printf("Improper header version\n");
        free(header);
        return STATUS_ERROR;
    }
    struct stat dbstat = {0};
    fstat(fd,&dbstat);
    if (header->filesize != dbstat.st_size){
        printf("Corrupted database\n");
        free(header);
        return STATUS_ERROR;
    }
    *headerOut = header;
    return STATUS_SUCCESS;
}

int create_db_header(int fd, struct dbheader_t **headerOut){
    struct dbheader_t *header = calloc(1,sizeof(struct dbheader_t));
    if (header == NULL){
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }
    header->version = 0x1;
    header->count = 0;
    header->magic = HEADER_MAGIC;
    header->filesize = sizeof(struct dbheader_t);

    *headerOut = header;

    return STATUS_SUCCESS;
}