#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "file.h"

int create_db_file(char *filename) {
    int fd = open(filename, O_RDWR);
    if (fd != -1) {
        close(fd);
        printf("File already exists\n");
        return 1;
    }

    fd = open(filename, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    return fd;

}

int open_db_file(char *filename) {
    int fd = open(filename, O_RDWR , 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }
    return fd;
}


