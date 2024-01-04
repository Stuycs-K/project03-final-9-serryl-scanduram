#include <signal.h>


static void sighandler( int signo ){
    
    if (signo == SIGINT){
        printf("Process with PID: %d Exiting due to SIGINT\n", getpid());
        char buffer[50];
        sprintf(buffer, "Process with PID: %d Exiting due to SIGINT\n", getpid());
        int w_file = open("output.txt", O_WRONLY | O_CREAT | O_APPEND, 0600);
        if(w_file == -1) err();
        write(w_file, &buffer, sizeof(buffer));
        exit(0);
    }
    else if(signo == SIGQUIT){
        printf("PPID: %d , PID: %d\n", getppid(), getpid());
    }
}

