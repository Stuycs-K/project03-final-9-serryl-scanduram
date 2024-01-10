#include <signal.h>

void err(int i, char*message){
  if(i < 0){
      printf("Error: %s - %s\n",message, strerror(errno));
      exit(1);
  }
}

static void sighandler( int signo ){
    if (signo == SIGINT){
        printf("Are you sure you want the server to quit? Client connections will fail (y/n)\n");
        char* input[2];
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "y") == 0){
            printf("exiting program\n");//change to smth involving usr?
            exit(0);
        }
        else{
            printf("okay, all connections intact\n");
        }
    }
    else if(signo == SIGQUIT){
        printf("not sure what this does");
    }
}

