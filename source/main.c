#include <stdio.h>
#include <stdbool.h>
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
    bool newfile = false;
    int c;

    int dbfd = -1;
    struct dbheader_t *dbhdr= NULL;

    while ((c = getopt(argc, argv, "nf:")) != -1){
        switch (c) {
            case 'n':
                newfile = true;
                break;
            
            case 'f':
                filepath = optarg;
                break;
            
            case '?':
                printf("\nUnkown option -%c\n", c);
                break;
            default:
                return STATUS_ERROR;
        }
    }
    if (filepath == NULL){
        printf("Filepath is a required arguement\n\n");
        print_usage(argv);

        return STATUS_SUCCESS;

    }

    if(newfile){
        dbfd = create_db_file(filepath); 
        if(dbfd == 1) {
            printf("Unable to create database file\n");
            return STATUS_ERROR;
        }
        if (create_db_header(dbfd,&dbhdr) == -1){
            printf("Failed to Create database header\n");
            return STATUS_ERROR;
        }
    } else {
        dbfd = open_db_file(filepath);
        if(dbfd == 1) {
            printf("Unable to open database file\n");
            return STATUS_ERROR;
        }
        if (validate_db_header(dbfd,&dbhdr) == -1){
            printf("Failure to validate database header\n");
            return STATUS_ERROR;
        }
    }

    printf("Newfile: %d\n", newfile);
    printf("Filepath: %s\n", filepath);

    output_file(dbfd,dbhdr);

    return STATUS_SUCCESS;

}