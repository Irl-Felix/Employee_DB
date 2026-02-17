#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include "file.h"
#include "parse.h"
#include "common.h"

void print_usage(char *argv[]){
    printf("Usage: %s -n -f <database file>\n",argv[0]);
    printf("\t -n - create new database file\n");
    printf("\t -f -  (required)\n");
    return;
}
    
int main (int argc, char *argv[]){    
    char *filepath = NULL;
    char *addstring = NULL;
    char *updatestring = NULL;
    char *removestring = NULL;
    bool newfile = false;
    bool list = false;
    int c;
    int dbfd = -1;
    int ret = STATUS_SUCCESS;
    struct dbheader_t *dbhdr= NULL;
    struct employee_t *employees = NULL;

    while ((c = getopt(argc, argv, "nf:a:u:r:l")) != -1){
        switch (c) {
            case 'n':
                newfile = true;
                break;
            case 'f':
                filepath = optarg;
                break;
            case 'a':
                addstring = optarg;
                break;
            case 'u':
                updatestring = optarg;
                break;
            case 'r':
                removestring = optarg;
                break;
            case 'l':
                list = true;
                break;
            case '?':
                printf("\nUnkown option -%c\n", c);
                break;
            default:
                ret = STATUS_ERROR;
                goto cleanup;
        }
    }

    if (filepath == NULL){
        printf("Filepath is a required arguement\n\n");
        print_usage(argv);
        goto cleanup;
    }

    if(newfile){
        dbfd = create_db_file(filepath); 
        if(dbfd < 0) {
            printf("Unable to create database file\n");
            ret = STATUS_ERROR;
            goto cleanup;
        }
        if (create_db_header(&dbhdr) == -1){
            printf("Failed to Create database header\n");
            ret = STATUS_ERROR;
            goto cleanup;
        }
    } else {
        dbfd = open_db_file(filepath);
        if(dbfd < 0) {
            printf("Unable to open database file\n");
            ret = STATUS_ERROR;
            goto cleanup;
        }
        if (validate_db_header(dbfd,&dbhdr) == -1){
            printf("Failure to validate database header\n");
            ret = STATUS_ERROR;
            goto cleanup;
        }
    }

    if (read_employees(dbfd, dbhdr, &employees) != STATUS_SUCCESS){
        printf("Failed to read employees\n");
        ret = STATUS_ERROR;
        goto cleanup;
    }

    if (addstring) {
        add_employee(dbhdr, &employees, addstring);
    }
    if (updatestring) {
        update_employee_by_name(dbhdr, employees, updatestring);
    }
    if (removestring) {
        remove_employee_by_name(dbhdr, &employees, removestring);
    }
    if (list) {
        list_employees(dbhdr, employees);
    }

    output_file(dbfd, dbhdr, employees);

cleanup:
    free(employees);
    free(dbhdr);
    if (dbfd >= 0)
        close(dbfd);
    return ret;
}