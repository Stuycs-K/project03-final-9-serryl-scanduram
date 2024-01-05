#include "networking.h"

void subserver_logic(int client_socket, char* username){
    char buffer[BUFFER_SIZE];
    
    while (1) {
        // Receive data from client
        int Rbytes = read(client_socket, buffer, sizeof(buffer));
        if (Rbytes <= 0) {
            printf("[%s] disconnected\n", username);
            break; // Client disconnected
        }

        char formatted_message[BUFFER_SIZE + 100];
        snprintf(formatted_message, sizeof(formatted_message), "[%s]: %s", username, buffer);
        
        //send data back to client
        //brief pseudo code for sending to all clients:
            // for each client connected, write,
        int Sbytes = write(client_socket, formatted_message, strlen(formatted_message));
        if(Sbytes==-1){
            perror("Send error");
        }
    }

    close(client_socket);
    //printf("server waiting for another request\n");
}

int main(int argc, char *argv[] ) {
    int listen_socket = server_setup();
    printf("server open, waiting for client\n");
    
    while(1){
        int client_socket = server_tcp_handshake(listen_socket);
        
        char username[100];
        snprintf(username, sizeof(username), "User%d", client_socket);
        
        //forking
        int f = fork();
        if (f == -1) {
            printf("fork failed\n");
        }
        else if (f==0){ //child
            close(listen_socket);
            subserver_logic(client_socket, username);
            exit(0);
        }
        else {
            close(client_socket);
            printf("[%s] joined the chat\n", username);
        }
    }
}


