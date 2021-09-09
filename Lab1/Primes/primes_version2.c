// Prime with redirection
#include"kernel/types.h"
#include"user/user.h"

#define R 0
#define W 1

void redirect(int r_or_w, int fd[2])
{
    close(r_or_w);
    dup(fd[r_or_w]);
    close(fd[R]);
    close(fd[W]);
}

void source(int l)
{
    for (int i = 2; i < l; i++)
        write(W, &i, sizeof(i));
    close(W);
}

void cull(int p)
{
    int num;
    while (read(R, &num, sizeof(num)))
        if (num % p != 0)
            write(W, &num, sizeof(num));
    close(W);
}

void sink()
{
    int p;
    int fd[2];
    while (read(R, &p, sizeof(p)))
    {
        printf("prime: %d\n", p);
        pipe(fd);
        if (fork())
        {
            redirect(W, fd);
            cull(p);
            wait(0);
            break;
        }
        else
        {
            redirect(R, fd);
            continue;
        }
    }
}

int main()
{
    int fd[2];
    pipe(fd);
    if (fork())
    {
        redirect(W, fd);
        source(100);
        wait(0);
    }
    else
    {
        redirect(R, fd);
        sink();
    }
}
