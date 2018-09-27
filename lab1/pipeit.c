/* CSC 453 Lab 01
 * Due Friday 9/28/18
 * Daniel Kirkpatrick (djkirkpa)
 * Patrick Farrell (pifarrel)
 */

/* STILL NOT GETTING OUTFILE:PERMISSION DENIED EXIT STATUS TO BE 1 */

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

//stdout dup2(fd[1] write end

int main(int argc, char * argv[]) {
   pid_t pid_child1;
   pid_t pid_child2;

   int fd1[2];


   if (pipe(fd1) == -1) {
      fprintf(stderr, "Pipe failed");
      exit(1);
   }

   pid_child1 = fork();
   checkFork(pid_child1); 

   if (pid_child1 == 0) { /* In child 1 (ls?) */
      if (dup2(fd1[1],STDOUT_FILENO) < 0) {
         perror("error");
         exit(3);
      }
      close(fd1[0]);
      execlp("ls","ls",NULL); /* exec ls. if runs past, error */
      perror("error");
      exit(1);
   }
   else { /* in parent */
      pid_t child1Exit;
      child1Exit = waitpid(pid_child1);
      if (child1Exit == -1) {
         perror("child1 exit error");
         exit(1);
      }

      pid_child2 = fork();
      checkFork(pid_child2); /* Call helper function to check if forked correctly */
      if (pid_child2 == 0) { /* In child 2 (sort?) */
         close(fd1[1]);
         if (dup2(fd1[0],STDIN_FILENO) < 0) { 
            perror("error");
            exit(1);
         }
         int outfile = open("outfile",O_RDWR | O_CREAT,0777);
         if (outfile < 0) {
            perror("outfile");
            exit(EXIT_FAILURE);
         }
         if (dup2(outfile,STDOUT_FILENO) < 0) {/* Write from STDOUT */
            perror("error");
            exit(1);
         }
         close(outfile);
         execlp("sort","sort","-r", NULL); /* exec sort. If runs past, error */
         perror(" exec error");
         exit(1);
      }
      else { /* Back in parent */
         int status;
         close(fd1[0]);
         close(fd1[1]);
         waitpid(pid_child2,&status,0); /*wait for second child*/
         if (WEXITSTATUS(status) != 0) {
            exit(1);
         }
         exit(0);
      }
   }
}
