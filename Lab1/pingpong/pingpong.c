#include"kernel/types.h"
#include"user/user.h"

int
main()
{
    int pipe1[2];
    int pipe2[2];
    pipe(pipe1);
    pipe(pipe2);
    char buf[1] = {'b'}; // a buf for storing the recieved char from the pipe
    if (fork() == 0)
    {
        // pipe1[0] reading port of child
        // pipe2[1] writing port of child
        close(pipe1[1]);
        close(pipe2[0]);
        // read from the parent through pipe1[0]
        // (if not recieved, wait)
        if (read(pipe1[0], buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "Child read error!\n");
            exit(1);
        }
        fprintf(1, "%d: received ping!\n", getpid());
        // write to the parent through pipe2[1]
        if (write(pipe2[1], buf, sizeof(buf)) != sizeof(buf))
        {
            fprintf(2, "Child read error!\n");
            exit(1);
        }
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    }
    // pipe1[1] writing port of the parent
    // pipe2[0] reading port of the parent
    close(pipe1[0]);
    close(pipe2[1]);
    if (write(pipe1[1], buf, sizeof(buf)) != sizeof(buf))
    {
        fprintf(2, "Parent write error!\n");
        exit(1);
    }
    if(read(pipe2[0], buf, sizeof(buf)) != sizeof(buf))
    {
        fprintf(2, "Parent read error!\n");
        exit(1);
    }
    fprintf(2, "%d: recieved pong!\n", getpid());
    close(pipe1[1]);
    close(pipe2[0]);
    wait(0);
    exit(0);
}
