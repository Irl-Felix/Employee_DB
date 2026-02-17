#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x424f5353

struct dbheader_t {
    unsigned int magic;
    unsigned int version;
    unsigned int count;
    unsigned int filesize;
};

struct employee_t {
    int name[256];
    int address[256];
    unsigned int hours;
};

int create_db_header(int fd, struct dbheader_t **headerOut);
int validate_db_header(int fd, struct dbheader_t **headerOut);
void list_employees(struct dbheader_t *dbhdr,struct employee_t *employees);
int read_employees(int fd, struct dbheader_t *, struct employee_t **employeesOut);
int add_employee(struct dbheader_t *dbhdr,struct employee_t **employees,char *addstring);
int update_employee_by_name(struct dbheader_t *dbhdr,struct employee_t *employees,char *updatestring);
int remove_employee_by_name(struct dbheader_t *dbhdr,struct employee_t **employees,char *name);
void output_file(int fd, struct dbheader_t *, struct employee_t *employees);

#endif