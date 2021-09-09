#include"kernel/types.h"
#include"user/user.h"

int 
main()
{
    int fd[2];
    pipe(fd);
    if (fork()) // Driving process that continuously generates numbers less than 32
    {
        close(fd[0]);
        for (int i = 2; i < 32; i++)
            write(fd[1], &i, sizeof(i));
        close(fd[1]); // close write end to enable the child to reach EOF condition !!!
        wait(0);
    }
    else /* The second child process that's reponsible of (not only retrieving 2 as the 1st prime)
            but alsoe giving birth to all other process in the pipeline */
    {
        close(fd[1]);
        int num, p;

        // MAIN LOGIC
        while (read(fd[0], &p, sizeof(p)))
        {
            // retrieving one prime out from the pipeline
            printf("prime %d\n", p);
            // Passing all other numbers down the pipeline to its child
            /* Requires :
                1. fd[0] remaining open (except that the prime which is just read off from the pipe all remianing numbers are still in the pipe)
                2. A new pipe connecting the current process and its child
                Therefore, fd[0] needs to be preserved with some other id (using dup and close)
            */
            int temp = dup(fd[0]); // Obtain a new file descriptor id for fd[0]
            close(fd[0]); // Close fd[0] which is soon gonna be replaced with a pipe syscall

            pipe(fd);
            if (fork()) /* Create a child process to carry all the numbers 
                        that the current prcocess did not filter out */
            { // Parent (current process)
                close(fd[0]);
                while (read(temp, &num, sizeof(num)))
                    if (num % p != 0)
                        write(fd[1], &num, sizeof(num));
                close(temp);
                close(fd[1]);
                wait(0);
                break; /* Parent, after having done it job of passing all the remaining numbers to the pipe through fd[1],
                        should break or exit and no longer loops because it is the child's job to 
                        iterate on and to retrieve another prime and create another process down the pipeline */
            }
            else // Child
            {
                close(temp); // temp fd though is copied to the child through a fork syscall, but it's only useful to the parent process
                close(fd[1]); // child doesn't write to the pipe, and it only does it job after looped back (noy here)
                /* its job being :
                    in the next iteration, retrieving 1 prime
                    then fork a new process as its child and pass the remaining numbers to it
                */
                continue;
            }
        }
    }
}