/* CSC 453 Lab 01
 * Due Friday 9/28/18
 * Daniel Kirkpatrick (djkirkpa)
 * Patrick Farrell (pifarrel)
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

/*Helper function to check if forked correctly*/
static void checkFork(pid_t p) {
   if (p < 0) {
      perror("fork");
      exit(1);
   }
}

int main(int argc, char * argv[]) {
   pid_t pid_child1;
   pid_t pid_child2;

   int fd1[2];

   /* Create the pipe in the parent */
   if (pipe(fd1) == -1) {
      fprintf(stderr, "Pipe failed");
      exit(1);
   }

   /* Fork and check status */
   pid_child1 = fork();
   checkFork(pid_child1); 

   if (pid_child1 == 0) { /* In child 1 (ls) */
      if (dup2(fd1[1],STDOUT_FILENO) < 0) {
         perror("error");
         exit(3);
      }
      /* close read end of pipe */
      close(fd1[0]);
      execlp("ls","ls",NULL); /* exec ls. if runs past, error */
      perror("error");
      exit(1);
   }
   else { /* in parent */
      int status;
      /*wait for first child, exit parent if error*/
      waitpid(pid_child1,&status,0);
      if (WEXITSTATUS(status)!= 0) {
         perror("child1 exit error");
         exit(1);
      }
      
      /* fork and check */
      pid_child2 = fork();
      checkFork(pid_child2); 
      if (pid_child2 == 0) { /* In child 2 (sort) */
         close(fd1[1]);
         if (dup2(fd1[0],STDIN_FILENO) < 0) { 
            perror("error");
            exit(1);
         }
         /* open and check outfile */
         int outfile = open("outfile",O_RDWR | O_CREAT,0777);
         if (outfile < 0) {
            perror("outfile");
            exit(EXIT_FAILURE);
         }
         /* redirect stdout to outfile */
         if (dup2(outfile,STDOUT_FILENO) < 0) {
            perror("error");
            exit(1);
         }
         close(outfile);
         /* exec sort. If runs past, error */
         execlp("sort","sort","-r", NULL); 
         perror("sort exec error");
         exit(1);
      }
      else { /* Back in parent */
         int status;
         /* close both ends of pipe in parent */
         close(fd1[0]);
         close(fd1[1]);
         /* wait for child 2, exit if failure */
         waitpid(pid_child2,&status,0);
         if (WEXITSTATUS(status) != 0) {
            exit(1);
         }
         exit(0);
      }
   }
}
