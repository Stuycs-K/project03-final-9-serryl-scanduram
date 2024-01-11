#include "networking.h"
#include "history.h"
#include <signal.h>
#include <string.h>

char* FILENAME;
char save[2];

static int sigint_received = 0; // Global variable to track SIGINT reception

static void sighandler( int signo ){
    if (signo == SIGINT){
        printf("Are you sure you want to exit? (y/n)\n");
        char input[2];
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "y") == 0){
            printf("\nyou are exiting\n");//change to smth involving usr?
            exit(0);
        }
        else{
            printf("okay!\n");
            sigint_received = 1; // Set the flag to continue normal execution
        }
    }
    else if(signo == SIGQUIT){
        printf("not sure what this does");
    }
}

void clientLogic(int server_socket, char username[50]){
    char buffer[BUFFER_SIZE];
    char user[50];
    //prompt user input
    while (1) {
        /*
       //if (sigint_received == 1){
     //   }

        */
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        

        int i = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
        
        if (i < 0) {
            perror("select error");
            break;
        }
        
        if (FD_ISSET(server_socket, &read_fds)) {
            int rbytes = read(server_socket, buffer, sizeof(buffer));
            if (save[0]=='y'){
                writer(FILENAME, "", buffer);
            }
            if (rbytes <= 0) {
                perror("recv error");
                break;
            }
            buffer[rbytes] = '\0';
            printf("%s\n", buffer);
        }
        
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
             //printf("[%s]: ", username);
             fgets(buffer, sizeof(buffer), stdin);
             buffer[strlen(buffer) - 1] = '\0';
            if (save[0]=='y'){
                writer(FILENAME, username, buffer);
            }
             int sbytes = write(server_socket, buffer, strlen(buffer));
             if (sbytes == -1) {
                 perror("send error");
                 exit(1);
             }
         }
    }
}



int main(int argc, char *argv[] ) {
    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    char* IP = "127.0.0.1";
    if(argc>1){
        IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    
    printf("Please enter 'c' if you would like to enter a chat and 'h' if you would like to see previous chat histories.\n");
    char ans[2];
    fgets(ans, sizeof(ans), stdin);
    if(strcmp(ans, "h") == 0){
        directoryPrint();
        printf("Please enter the exact name of the history you would like to see: ");
        char historyName[100];
        fgets(historyName, sizeof(historyName), stdin);
        historyName[strcspn(historyName, "\n")] = '\0';
        reader(historyName);
        
    }
    else {
        int s;
        while ((s = getchar()) != '\n' && s != EOF);
        printf("Do you want to save chat history? (y/n):\n");
        fgets(save, sizeof(save), stdin);
        save[strcspn(save, "\n")] = '\0';
        if (save[0]=='y') {
            printf("Ok. chat will be saved!\n");
            FILENAME = creator();
        }
        else{
            printf("Ok. chat will not be saved!\n");
        }
        
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        
        char name[100];
        printf("Enter your username: ");
        fgets(name, sizeof(name), stdin);
        for (int i = 0; i < strlen(name); i++){
            if(name[i]== '\n'|| name[i]== '\r'){
                name[i] = 0;
                break;
            }
        }
        int sbytes = write(server_socket, name, strlen(name));
        if (sbytes < 0) {
            perror("send error");
            close(server_socket);
            exit(1);
        }
        clientLogic(server_socket, name);
        close(server_socket);
    }
}
