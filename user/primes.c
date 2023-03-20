#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int isprime(int n)
{
    int j, flag = 1;

    for (j = 2; j <= n / 2; ++j)
    {
        if (n % j == 0)
        {
            flag = 0;
            break;
        }
    }
    return flag;
}

int create_process(int parent_fd[2])
{
    char bytes[4];
    close(parent_fd[1]);
    /*如果有指向管道写端的文件描述符没有关闭，而持有写端的进程也没有向管道内
    写入数据的时候，那么管道剩余的数据被读取后，再次read会被阻塞，之后有数据
    可读会再次返回。
    如果所有的写端均被关闭，那么再次read会直接返回0，就像读到了文件末尾一样。
    */
    read(parent_fd[0], bytes, sizeof(bytes));
    int prime = *(int *)bytes;
    printf("prime %d\n", prime);
    /*如果管道内没有其他prime，则不再创建子进程*/
    if (read(parent_fd[0], bytes, sizeof(bytes)) == 0)
    {
        close(parent_fd[0]);
        exit(0);
    }

    int child_fd[2];
    pipe(child_fd);

    int pid = fork();
    if (pid < 0)
    {
        printf("fork error");
        exit(1);
    }
    else if (pid == 0)
    {
        create_process(child_fd);
        exit(0);
    }
    else
    {
        close(child_fd[0]);
        do
        {
            int selectprime = *(int *)bytes;
            if (isprime(selectprime))
            {
                write(child_fd[1], bytes, sizeof(bytes));
            }
        } while (read(parent_fd[0], bytes, sizeof(bytes)));
        close(parent_fd[0]);
        close(child_fd[1]);
        //必须等到所有子进程都结束了才能exit
        wait((int *)0);
        exit(0);
    }
}
int main(int argc, char *argv[])
{
    int parent_fd[2];
    pipe(parent_fd);
    int count = 35;
    int pid = fork();
    if (pid < 0)
    {
        printf("fork error");
        exit(1);
    }
    else if (pid == 0)
    {
        create_process(parent_fd);
        exit(0);
    }
    else
    {
        close(parent_fd[0]);
        for (int i = 2; i <= count; i++)
        {
            char bytes[4];
            bytes[3] = (i >> 24) & 0xff;
            bytes[2] = (i >> 16) & 0xff;
            bytes[1] = (i >> 8) & 0xff;
            bytes[0] = i & 0xff;
            write(parent_fd[1], bytes, sizeof(bytes));
        }
        close(parent_fd[1]);
        wait((int *)0);
        exit(0);
    }
}
