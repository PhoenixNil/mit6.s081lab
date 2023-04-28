// #include "user/user.h"
// #include "kernel/param.h"
// #include "kernel/types.h"
// int main(int argc, char *argv[])
// {
//     int i, paras;
//     char buff[MAXARG];
//     char *xargv[MAXARG];
//     read(0, buff, MAXARG); // 0 is stdin，把标准输入读到buff里
//     char *p = buff;        // 子进程的p不会继承父进程的p
//     for (i = 0; i < MAXARG; i++)
//     {
//         if (buff[i] == '\n')
//         {
//             int pid = fork(); // 给每一行命令创建一个子进程
//             if (pid > 0)      // parent process
//             {
//                 char *p = &buff[i + 1];
//                 wait(0);
//             }
//             else
//             {
//                 buff[i] = 0;
//                 xargv[paras] = p;
//                 paras++;
//                 xargv[paras] = 0;
//                 paras++;
//                 exec(argv[0], xargv); // 执行这一行的命令,argv[0]是xargs，argv是xargs后面的参数，等到下一行命令的时候就会把下一行命令的参数放到argv里
//                 exit(0);
//             }
//         }
//     }
//     exit(0);
// }
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[])
{
    char buf[512];
    char *args[MAXARG];
    int i, n;

    // Parse command-line arguments
    if (argc < 2)
    {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    // Read lines from standard input
    while ((n = read(0, buf, sizeof(buf))) > 0)
    {
        // Replace newline with null character
        for (i = 0; i < n; i++)
        {
            if (buf[i] == '\n')
            {
                buf[i] = 0;
                break;
            }
        }

        // Split line into arguments
        args[0] = argv[1]; // argv[0]是xargs，argv[1]是command，argv[2]是command后面的参数
        for (i = 2; i < argc; i++) 
        {
            args[i - 1] = argv[i];
        }
        args[argc - 1] = buf; // 把输入放到最后一个位置，比如echo "param1" | xargs mycommand, echo "parama1"是buf，mycommand是argv[1]
        args[argc] = 0;       // 代表参数结束

        // Fork a child process to run the command
        if (fork() == 0)
        {
            exec(args[0], args); // 在这段代码中，exec(args[0], args)的作用是执行一个新的程序。具体来说，它将第一个参数args[0]作为要执行的程序的名称，将args数组作为要传递给程序的参数
            exit(0);
        }
        else
        {
            wait(0);
        }
    }

    exit(0);
}
