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
    printf("[%s] joined the chat\n", username);
    fd_set read_fds;
    
    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);
        
        int activity = select(client_socket + 1, &read_fds, NULL, NULL, NULL);
        if (activity < 0) {
            perror("select error");
            break;
        }
        
        if(FD_ISSET(STDIN_FILENO, &read_fds)){
            fgets(buffer, sizeof(buffer), stdin); //read from standard in
            buffer[strlen(buffer) - 1] = '\0';
            for (int i = 0; i < userCount; i++){
                if(user_list[i].socket_id != -1){
                    int Sbytes = write(user_list[i].socket_id, buffer, strlen(buffer));
                    if(Sbytes == -1){
                        perror("Send error");
                    }
                }
            }
        }
        
        if (FD_ISSET(client_socket, &read_fds)) {
            // Receive data from client
            int Rbytes = read(client_socket, buffer, sizeof(buffer));
            if (Rbytes <= 0) {
                // Client disconnected
                printf("[%s] disconnected\n", username);
                break;
            }
            
            // Broadcast the received message to all clients
            for (int i = 0; i < userCount + 1; i++) {
                if (user_list[i].socket_id != -1 && user_list[i].socket_id != client_socket) {
                    int Sbytes = write(user_list[i].socket_id, buffer, Rbytes);
                    if (Sbytes == -1) {
                        perror("Send error");
                    }
                }
            }
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
        
        struct User new_user;
        new_user.socket_id = client_socket;
        //snprintf(new_user.username, sizeof(new_user.username), "User%d", userCount + 1);
        //user_list[userCount++] = new_user;
        // printf("usercount: %d", userCount);
        char buffer[BUFFER_SIZE];
        int rbytes = read(client_socket, buffer, sizeof(buffer));
        if (rbytes > 0) {
            buffer[rbytes] = '\0';
            char* name = new_user.username;
            for (int i = 0; i < strlen(name); i++){
                if (name[i] == '\n' || name[i] == '\r'){
                    name[i] = 0;
                    break; // Stop once newline is removed
                }
            }
            snprintf(name, sizeof(name), "%s", buffer);
            user_list[userCount++] = new_user;
            printf("[%s] has joined\n", name);
        } 
        else {
            perror("Username receive error");
            close(client_socket);
            continue;
        }
        
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
            //printf("[%s] joined the chat\n", new_user.username);
        }
    }
    close(listen_socket);
}


