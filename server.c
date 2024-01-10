#include "networking.h"
#include "history.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#define MAX_USERS 30

static int sigint_received = 0; // Global variable to track SIGINT reception

static void sighandler( int signo ){
    if (signo == SIGINT){
        printf("Are you sure you want the server to quit? Client connections will fail (y/n)\n");
        char input[2];
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "y") == 0){
            printf("exiting program\n");//change to smth involving usr?
            exit(0);
        }
        else{
            printf("okay, all connections intact\n");
            sigint_received = 1; // Set the flag to continue normal execution
        }
    }
    else if(signo == SIGQUIT){
        printf("not sure what this does");
    }
}





struct User {
    int socket_id;
    char username[50];
};

struct User user_list[MAX_USERS];
int userCount = 0;


void subserver_logic(int client_socket, char *username, char save){
    
    char buffer[BUFFER_SIZE];
    printf("[%s] joined the chat\n", username);
    
    
    while (1) {
        int c = getchar();
        if (c != EOF) {
            save = (char)c;
        }
        
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);
        
        int i = select(client_socket + 1, &read_fds, NULL, NULL, NULL);
        if (i < 0) {
            perror("select error");
            break;
        }
        
        //int Sbytes = write(user_list[i].socket_id, username, strlen(username));
        
        if(FD_ISSET(STDIN_FILENO, &read_fds)){
            printf("in stdin sevrer");
            fgets(buffer, sizeof(buffer), stdin); //read from standard in
            buffer[strlen(buffer) - 1] = '\0';
            printf("[%s]: %s\n", username, buffer);
            
            for (int i = 0; i < userCount; i++){
                if(user_list[i].socket_id != -1 && user_list[i].socket_id != client_socket){
                    
                    int Sbytes = write(user_list[i].socket_id, buffer, strlen(buffer));
                    if(Sbytes <0){
                        perror("Send error");
                    }
                }
            }
            printf("after stdin");
        }
        
        if (FD_ISSET(client_socket, &read_fds)) {
            printf("[%s] is typing from client socket\n", username);
            // Receive data from client
            int Rbytes = read(client_socket, buffer, sizeof(buffer));
            if (save =='y'){
                reader(username, buffer);
            }
            
            if (Rbytes <= 0) {
                // Client disconnecteD
                printf("[%s] disconnected\n", username);
                
                for (int i = 0; i < userCount; i++) {
                    if (user_list[i].socket_id == client_socket) {
                        user_list[i].socket_id = -1;  // Mark as disconnected
                        break;
                    }
                }
                
                break;
            }
            else{
                buffer[Rbytes] = '\0';
                printf("[%s] received: %s\n", username, buffer);
                // Broadcast the received message to all clients
                //printf("\nusercount: %d\n", userCount);
                printf("printing userList: \n");
                for (int i = 0; i < userCount; i++){
                    printf("user %d: %s\n", i, user_list[i].username);
                }
                
                for (int i = 0; i < userCount; i++) {
                    if (user_list[i].socket_id != -1){
                        printf("Broadcasting to %s: %s\n", user_list[i].username, buffer);//does do that
                        
                        if(strcmp(user_list[i].username, username)!=0){
                            printf("current user: %s\n", user_list[i].username);
                            int Sbytes = write(user_list[i].socket_id, buffer, Rbytes);
                            if (Sbytes<0) {
                                perror("Send error");
                            }
                        }
                    }
                }
                printf("[%s]: %s\n", username, buffer);
            }
        }
        
    }
    
    close(client_socket);
}

int main(int argc, char *argv[] ) {
    //signal(SIGINT, sighandler);
    //signal(SIGQUIT, sighandler);

    //semUserCount();
    int listen_socket = server_setup();
    printf("server open, waiting for client\n");

    while(1){
       // if (sigint_received == 1){
            
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(listen_socket, &read_fds);
            
            int max_fd=listen_socket;
            
            for (int i =0; i<userCount; i++){
                FD_SET(user_list[i].socket_id, &read_fds);
                if(user_list[i].socket_id > max_fd) {
                    max_fd = user_list[i].socket_id;
                }
            }
            
            int i = select(max_fd + 1, &read_fds, NULL, NULL, NULL);
            if (i < 0) {
                perror("select error");
                break;
            }
            
            if (FD_ISSET(listen_socket, &read_fds)) {
                int client_socket = server_tcp_handshake(listen_socket);
                
                char save[2];
                int rbytes = read(client_socket, save, sizeof(save));
                if (strcmp(save, "y") == 0){
                    printf("chat will be saved\n");
                    //stub for next
                }
                else{
                    printf("chat will not be saved\n");
                }
                
                struct User new_user;
                new_user.socket_id = client_socket;
                rbytes = read(client_socket, new_user.username, sizeof(new_user.username));
                if (rbytes > 0) {
                    new_user.username[rbytes] = '\0';
                    user_list[userCount++] = new_user;
                    printf("[%s] joined the chat\n", new_user.username);
                } else {
                    perror("Username receive error");
                    close(client_socket);
                }
            }
            
            for(int i =0; i<userCount; i++){
                if (FD_ISSET(user_list[i].socket_id, &read_fds)) {
                    char buffer[BUFFER_SIZE];
                    int rbytes = read(user_list[i].socket_id, buffer, sizeof(buffer));
                    
                    if (rbytes <= 0) {
                        // Client disconnected
                        printf("[%s] disconnected\n", user_list[i].username);
                        
                        // Remove user from the list
                        for (int j = i; j < userCount - 1; j++) {
                            user_list[j] = user_list[j + 1];
                        }
                        userCount--;
                        i--;
                    }
                    else{
                        buffer[rbytes] = '\0';
                        printf("[%s]: %s\n", user_list[i].username, buffer);
                        for (int j = 0; j < userCount; j++) {
                            if (j != i) {
                                int sbytes = write(user_list[j].socket_id, buffer, rbytes);
                                if (sbytes < 0) {
                                    perror("Send error");
                                }
                            }
                        }
                    }
                }
            }
       // }
    }
    close(listen_socket);
}
