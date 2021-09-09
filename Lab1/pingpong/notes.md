# <center> Pingpong </center>

<center><b> A pipe example </b></center>

In the following program the parent only talks to the child while the child only recieves from the parent and there isn't really mutual communication happening here, fro which we need just oen single pipe, one side from the parent to write and the other side for the child to read.

parent p[0]-----pipe------>p[1] child

```c
int p[2];
char *argv[2];
argv[0] = "wc";
argv[1] = 0;
pipe(p);
if(fork() == 0) {
  // release file discriptor 0
  // file discriptor table of child:
  // {
  //   0: stdin
  //   1: stdout
  //   2: stderr
  //   3: p[0]     
  //   4: p[1]     
  //   5: avaliable
  //   ...
  // }
  close(0); // release the file discriptor 0 from the occupation of stdin
            // which makes 0 the smallest file discriptor avaliable for future use
            // A dup(some_discriptor) will place that discriptor to as discriptor 0
  // file discriptor table of child:
  // {
  //   0: avaliable
  //   1: stdout
  //   2: stderr
  //   3: p[0]
  //   4: p[1]
  //   5: avaliable
  //   ...
  // }
  // duplicate p[0] file discriptor to file discriptor 0 which is avaliable
  dup(p[0]);
  // file discriptor table of child:
  // {
  //   0: p[0]     --> read from p[0] (read) end of the pipe 
  //   1: stdout
  //   2: stderr
  //   3: p[0]     --> can be closed
  //   4: p[1]     --> can be closed
  //   5: avaliable
  //   ...
  // }
  close(p[0]);
  close(p[1]);
  // file discriptor table of child:
  // {
  //   0: p[0]     --> read from p[0] (read) end of the pipe 
  //   1: stdout
  //   2: stderr
  //   3: p[0]
  //   4: p[1]
  //   5: avaliable
  //   ...
  // }
  exec("/bin/wc", argv); 
  // exec will have the input of wc redirected from stdin to p[0]

  // WHY DOING THIS IN CHILD?
  // That's the only way we can take control of
  // the file discriptor of "wc"
} else {
  close(p[0]); // No need to use the read end of the pipe
  write(p[1], "hello world\n", 12);
  close(p[1]);
}
```

<center><b> Pingpong </b></center>

Different from the above example, pingpong requires the parent and the child to both read from and write to the pipe which achieves the mutual communication like them two playing pingpong with each other.

Therefore, instead of using one pipe, two pipes are required:

parent p1[1]-----pipe------>p1[0] child => p1 : parent sending message to child
parent p2[0]<-----pipe------p2[1] child => p2 : child sending message to parent

The point here is that a pipe is unidirectional plus that a file discriptor can either act as the reading or sending prot but not both.

