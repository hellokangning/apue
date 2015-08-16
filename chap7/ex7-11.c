#include "apue.h"

#define TOK_ADD 5

jmp_buf jmpbuffer;

void do_line(char *);
void cmd_add(void);
void get_token(void);

int main(void)
{
    char line[MAXLINE];

    if (setjmp(jmpbuffer) != 0)
        print("error\n");

    while (fgets(line, MAXLINE, stdin) != NULL)
        do_line(line);

    exit(0);
}

char *tok_ptr; // global pointer to get_token()

void do_line(char *ptr)
{
    int cmd;

    tok_ptr = ptr;
    while ((cmd = get_token()) > 0)
    {
        switch (cmd)
        {
            case TOK_ADD:
                cmd_add();
                break;
        }
    }
}

void cmd_add(void)
{
    int token;
    token = get_token();     
    if (token < 0)
        longjmp(jmp_buf, 1);
    // reset of processiong for this cmd
}

int get_token(void)
{
    // fetch next token from line pointed to by tok_ptr}
}
