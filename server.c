#include "networking.h"
#include <semaphore.h>
#include <fcntl.h>
#define MAX_USERS 30


struct User {
    int socket_id;
    char username[50];
};

struct User user_list[MAX_USERS];
//sem_t *userCount_sem;
int userCount = 0;

/*
void semUserCount() {
    userCount_sem = sem_open("/userCount_sem", O_CREAT | O_EXCL, 0644, 1);
    if (userCount_sem == SEM_FAILED) {
        perror("Semaphore initialization failed");
        exit(EXIT_FAILURE);
    }
}
*/
void subserver_logic(int client_socket, char *username){
    
    char buffer[BUFFER_SIZE];
    printf("[%s] joined the chat\n", username);
    
    
    while (1) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(STDIN_FILENO, &read_fds);
        FD_SET(client_socket, &read_fds);
        
        int i = select(client_socket + 1, &read_fds, NULL, NULL, NULL);
        if (i < 0) {
            perror("select error");
            break;
        }
        
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
            printf("in client scokey\n");
            // Receive data from client
            int Rbytes = read(client_socket, buffer, sizeof(buffer));

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
                    if (user_list[i].socket_id != -1){ //&& user_list[i].socket_id == client_socket) {
                        printf("Broadcasting to %s: %s\n", user_list[i].username, buffer);//does do that
                        printf("current user: %s\n", user_list[i].username);
                        int Sbytes = write(user_list[i].socket_id, buffer, Rbytes);
                        if (Sbytes<0) {
                            perror("Send error");
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
    //semUserCount();
    int listen_socket = server_setup();
    printf("server open, waiting for client\n");

    while(1){
        int client_socket = server_tcp_handshake(listen_socket);

        struct User new_user;
        new_user.socket_id = client_socket;
        int rbytes = read(client_socket, new_user.username, sizeof(new_user.username));
        
        if (rbytes > 0) {
            new_user.username[rbytes] = '\0';
            //sem_wait(userCount_sem); // Acquire semaphore before modifying userCount
            user_list[userCount++] = new_user;
            //sem_post(userCount_sem); // Release semaphore after modifying userCount
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
    //sem_close(userCount_sem);
    //em_unlink("/userCount_sem"); // Cleanup semaphore
}
