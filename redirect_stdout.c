#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LEN 40

int main( int argc, char *argv[] ) {
  char buffer[MAX_LEN+1] = {0};
  int out_pipe[2];
  int saved_stdout;

  saved_stdout = dup(STDOUT_FILENO);  /* save stdout for display later */

  if( pipe(out_pipe) != 0 ) {          /* make a pipe */
    exit(1);
  }

  dup2(out_pipe[1], STDOUT_FILENO);   /* redirect stdout to the pipe */
  close(out_pipe[1]);

  /* anything sent to printf should now go down the pipe */
  printf("moving the data to buffer %p", buffer);
  fflush(stdout);

  read(out_pipe[0], buffer, MAX_LEN); /* read from pipe into buffer */

  dup2(saved_stdout, STDOUT_FILENO);  /* reconnect stdout for testing */
  printf("read: %s\n", buffer);

  return 0;
}
