#include "networking.h"
#include "history.h"
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#define MAX_USERS 30

struct User {
    int socket_id;
    char username[50];
};

struct User user_list[MAX_USERS];
int userCount = 0;

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
        if(FD_ISSET(listen_socket, &read_fds)){
            int client_socket = server_tcp_handshake(listen_socket);
            struct User new_user;
            new_user.socket_id = client_socket;
            int rbytes = read(client_socket, new_user.username, sizeof(new_user.username));
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
                    char message[BUFFER_SIZE + sizeof(user_list[i].username) + 5];
                    snprintf(message, sizeof(message), "[%s]: %s", user_list[i].username, buffer);
                    for (int j = 0; j < userCount; j++) {
                        if (j != i) {
                            int sbytes = write(user_list[j].socket_id, message, strlen(message));
                            if (sbytes < 0) {
                                perror("Send error");
                            }
                        }
                    }
                }
            }
        }
            
    }
    close(listen_socket);
}
