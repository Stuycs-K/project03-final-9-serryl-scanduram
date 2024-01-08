#include "networking.h"


void clientLogic(int server_socket, char username[50]){
    char buffer[BUFFER_SIZE];
    
    //prompt user input
    while (1) {
        
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        
        //printf("before select\n");

        int i = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
       // printf("after select\n");
        if (i < 0) {
            perror("select error");
            break;
        }
        
        
        if (FD_ISSET(server_socket, &read_fds)) {
            //printf("in server socket");
            int rbytes = read(server_socket, buffer, sizeof(buffer));
            if (rbytes <= 0) {
                perror("recv error");
                break;
            }
            buffer[rbytes] = '\0';
            printf("[%s] %s\n", username, buffer);
        }
        
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
            //printf("in client stdin fileno");
            printf("[%s]:", username);
             fgets(buffer, sizeof(buffer), stdin);
             buffer[strlen(buffer) - 1] = '\0';
             int sbytes = write(server_socket, buffer, strlen(buffer));
             if (sbytes == -1) {
                 perror("send error");
                 exit(1);
             }
         }
    }
}



int main(int argc, char *argv[] ) {
    
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
        printf("in progress\n");
    }
    else{
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
        
        //printing username
        //printf("Enter a message: ");
        
        
        int sbytes = write(server_socket, name, strlen(name));
        if (sbytes < 0) {
            perror("send error");
            
            //int sbytes = write(server_socket, name, strlen(name));
            
            //if (sbytes < 0) {
                perror("send error");
                close(server_socket);
                exit(1);
            }
            clientLogic(server_socket, name);
            close(server_socket);
        }
    }

