#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAXLINE 32
int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("xargs need at least three params");
        exit(1);
    }
    char *cmd = argv[1];
    char line[MAXLINE];
    // 管道前产生的结果放入line中
    memset(line, 0, sizeof(line));

    // 读入的每一个char
    int i = 0;
    char ch;
    while (read(0, &ch, sizeof(ch)) != 0)
    {
        if (ch == '\n')
        {
            char *child_argv[4];
            // 1: cmd 2: argv[2] 3:line 4:0表示结束
            child_argv[0] = cmd;
            child_argv[1] = argv[2];
            child_argv[2] = line;
            child_argv[3] = 0;
            if (fork() == 0)
            {
                exec(cmd, child_argv);
            }
            else
            {
                wait((int *)0);
                // parent process need to wait child process
                
            }
            memset(line,0,sizeof(line));
            i=0;
        }
        else
        {
            line[i++] = ch;
        }
    }
    exit(0);
}