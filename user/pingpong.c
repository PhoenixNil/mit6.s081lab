#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2], c[2]; // 读端和写端分别0和1
    char buf[64];
    pipe(p), pipe(c);
    int pid = fork();
    if (pid > 0) // parent
    {
        write(c[1], "ping", 4);
        read(p[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
    }
    else // child
    {
        read(c[0], buf, 4);
        printf("%d: received %s\n", getpid(), buf);
        write(p[1], "pong", 4);
        //当子进程收到父进程发送的数据后，首先会在终端上打印出一个带有进程ID的消息，表示已经收到了父进程发送的数据。然后它将这个数据通过pipe(p)管道发送回父进程。因此，在子进程中，printf语句的作用是为了告诉用户子进程已经接收到了数据。
        // 子进程中的write函数调用将数据发送回父进程。由于printf语句会将子进程的输出缓冲区中的数据刷新到终端，因此在执行write函数之前必须等到printf函数完成。否则，子进程的输出可能会与write函数写入管道的数据混合，导致输出的结果不正确。
        // 因此，将write函数放在printf语句后面可以确保子进程在发送数据之前先将收到数据的消息打印出来，从而避免输出数据混合的问题。
    }
    exit(0);
}