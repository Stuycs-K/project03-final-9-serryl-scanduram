#include <signal.h>

void err(int i, char*message){
  if(i < 0){
      printf("Error: %s - %s\n",message, strerror(errno));
      exit(1);
  }
}

static void sighandler( int signo ){
    
    if (signo == SIGINT){
        printf("Process with PID: %d Exiting due to SIGINT\n", getpid());//change to smth involving usr?
        char buffer[50];
        sprintf(buffer, "Process with PID: %d Exiting due to SIGINT\n", getpid());
       
        int w_file = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0600);
        err(w_file, "error opening file");
        
        write(w_file, &buffer, sizeof(buffer));
        exit(0);
    }
    else if(signo == SIGQUIT){
        printf("PPID: %d , PID: %d\n", getppid(), getpid());
    }
}

