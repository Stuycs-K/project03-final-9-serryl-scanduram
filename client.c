#include "networking.h"


void clientLogic(int server_socket, char username[50]){
    char buffer[BUFFER_SIZE];
    //char username [50];
    
    //prompt user input
    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(server_socket, &read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        
        
        int i = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
        if (i < 0) {
            perror("select error");
            break;
        }
        
        if (FD_ISSET(STDIN_FILENO, &read_fds)) {
             fgets(buffer, sizeof(buffer), stdin);
             buffer[strlen(buffer) - 1] = '\0';
             int sbytes = write(server_socket, buffer, strlen(buffer));
             if (sbytes == -1) {
                 perror("send error");
                 exit(1);
             }
         }
        
        if (FD_ISSET(server_socket, &read_fds)) {
            int rbytes = read(server_socket, buffer, sizeof(buffer));
            if (rbytes <= 0) {
                perror("recv error");
                break;
            }
            buffer[rbytes] = '\0';
            printf("%s", buffer);
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
    
    
    char name[100];
    printf("Enter your username: ");
    fgets(name, sizeof(name), stdin);
    for (int i = 0; i < strlen(name); i++){
        if(name[i]== '\n'|| name[i]== '\r'){
            name[i] = 0;
        }
    }
    
    int sbytes = write(server_socket, name, strlen(name));
    
    if (sbytes < 0) {
        perror("send error");
        close(server_socket);
        exit(1);
    }
    
    // Remove newline from the end of the username
    for (int i = 0; i < strlen(name); i++){
        if (name[i] == '\n' || name[i] == '\r'){
            name[i] = 0;
            break; // Stop once newline is removed
        }
    }
    
    //while(1){
    //char name[100];
    //printf("Enter your username: ");
    /*fgets(name, sizeof(name), stdin);
     
     //removing new line from end of username
     for (int i = 0; i < strlen(name); i++){
     if(name[i]== '\n'|| name[i]== '\r'){
     name[i] = 0;
     }
     }
     */
    //while(1){
        clientLogic(server_socket, name);
    //}
    //usleep(50);
    close(server_socket);
}
