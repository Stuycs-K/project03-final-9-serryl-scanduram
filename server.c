#include "networking.h"

#define MAX_USERS 30


struct User {
    int socket_id;
    char username[50];
};

struct User user_list[MAX_USERS];
int userCount = 0;

void subserver_logic(int client_socket, char* username){
    
    char buffer[BUFFER_SIZE];
    FD_SET(client_socket, &read_fds);

    if (client_socket > max_fd) {
         max_fd = client_socket;
     }
    
    printf("[%s] joined the chat\n", username);
    
    while (1) {
        fd_set read_fds = master_fds;
        
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
        FD_ZERO(&read_fds);
        int client_socket = server_tcp_handshake(listen_socket);
        
        struct User new_user;
        new_user.socket_id = client_socket;
        snprintf(new_user.username, sizeof(new_user.username), "User%d", userCount++);
        user_list[userCount] = new_user;
        
        //forking
        int f = fork();
        if (f == -1) {
            printf("fork failed\n");
        }
        else if (f==0){ //child
            close(listen_socket);
            subserver_logic(client_socket, new_user.username);
            exit(0);
        }
        else {
            close(client_socket);
            printf("[%s] joined the chat\n", new_user.username);
        }
    }
}


