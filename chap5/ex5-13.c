#include "apue.h"
#include <errno.h>

void make_temp(char *template);

int main()
{
    char good_template[] = "/tmp/dirXXXXXX"; // right way
    char *bad_template =  "/tmp/dirXXXXXX"; // wrong way

    printf("trying to create first temp file...\n");
    make_temp(good_template);

    printf("trying to create second temp file...\n");
    make_temp(bad_template);

    exit(0);
}

void make_temp(char *template)
{
    int fd;
    struct stat sbuf;

    if ((fd = mkstemp(template)) < 0)
        err_sys("cannot create temp file");
    printf("temp name = %s\n", template);
    close(fd);

    if (stat(template, &sbuf) < 0)
    {
        if (ENOENT == errno)
            printf("file does not exits\n");
        else
            printf("stat failed");
    }
    else
    {
        printf("file exists\n");
        unlink(template);
    }
}

