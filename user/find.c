#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    int findflag = 0;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return -1;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return -1;
    }
    // 将P指针指向path的最后buf+1的位置赋值"/"
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
        {
            continue;
        }

        if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
        {
            continue;
        }
        // 将指定路径目录下的dirent的name赋值给p，由此buf获取完整的路径
        memmove(p, de.name, DIRSIZ);
        *(p + DIRSIZ) = 0;
        // 可以通过完整的文件名获取文件的stat，由此可以获取其type
        if (stat(buf, &st) < 0)
        {
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        switch (st.type)
        {
        case T_FILE:
            if (strcmp(de.name, filename) == 0)
            {
                printf("%s\n", buf);
                findflag = 1;
            }
            break;
        case T_DIR:
            findflag = find(buf, filename);
            break;
        }
    }
    close(fd);
    return findflag;
}
int main(int argc, char *argv[])
{
    int findflag = 0;
    if (argc < 2)
    {
        printf("find need para\n");
        exit(1);
    }
    else if (argc == 2)
    {
        findflag = find(".", argv[1]);
    }
    else if (argc == 3)
    {
        findflag = find(argv[1], argv[2]);
    }
    else
    {
        printf("too many para\n");
        exit(1);
    }

    if (findflag == 0)
    {
        printf("can not find the file\n");
        exit(1);
    }
    exit(0);
}