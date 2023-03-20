#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    //0 read 1 write
    int child_fd[2];
    int parent_fd[2];

    pipe(child_fd);
    pipe(parent_fd);

    
    if (fork()==0)
    {   
        char buf[80];
        close(child_fd[1]);
        read(child_fd[0],buf,sizeof(buf));
        close(child_fd[0]);
        printf("%d: received p%sng\n",getpid(),buf);
        close(parent_fd[0]);
        write(parent_fd[1],"o",1);
        close(parent_fd[1]);
        exit(0);
    }
    else 
    {
        
        char buf[80];
        close(child_fd[0]);
        write(child_fd[1],"i",1);
        close(child_fd[1]);

        close(parent_fd[1]);
        read(parent_fd[0],buf,sizeof(buf));
        close(parent_fd[0]);
        printf("%d: received p%sng\n",getpid(),buf);
        exit(0);

    }
    return 0;
    
}
// #include "kernel/types.h"
// #include "user.h"
 
// int main(int argc,char* argv[]){
//     //创建两个管道，分别实现ping、pong的读写
//     int p[2];
//     pipe(p);
//     char readtext[10];//作为父进程和子进程的读出容器
//     //子程序读出
//     int pid = fork();
//     if(pid==0){
//         read(p[0],readtext,10);
//         printf("%d: received %s\n",getpid(),readtext);
//         write(p[1],"pong",10);
//         exit(0);//子进程一定要退出
//     }
//     //父程序写入
//     else{
//         write(p[1],"ping",10);
//         wait(0);//父进程阻塞，等待子进程读取
//         read(p[0],readtext,10);
//         printf("%d: received %s\n",getpid(),readtext);
//         exit(0);//父进程一定要退出
//     }
//     return 0;
// }