#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *sleeptime_char;
    int sleeptime;
    if (argc <= 1)
    {
        printf("sleep need one parm");
        exit(1);
    }
    if (argc > 2)
    {
        printf("too many parm");
        exit(1);
        /* code */
    }

    sleeptime_char = argv[1];
    sleeptime = atoi(sleeptime_char);
    sleep(sleeptime);
    exit(0);
}