#include "apue.h"

int globvar = 6;

int main(void)
{
    int var;
    pid_t pid;
    
    var = 88;
    printf("before fork\n");

    if ((pid = fork()) < 0)
    {
        err_sys("fork error");
    }
    else if (pid == 0)
    {
        ++globvar;
        ++var;
        _exit(0);
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar, var);

    exit(0);
}

