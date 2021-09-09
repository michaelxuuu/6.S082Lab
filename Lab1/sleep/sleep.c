#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
int 
main(int argc, char *argv[])
{
  if (argc != 2){
    fprintf(2, "Must 1 arg for sleep!\n");
    exit(1);
  }
  int sleepTime = atoi(argv[1]);
  fprintf(1, "Nothing happens for a while!\n");
  sleep(sleepTime);
  exit(0);
}

