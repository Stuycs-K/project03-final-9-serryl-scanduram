#include "networking.h"

void subserver_logic(int client_socket){
    char buffer[BUFFER_SIZE];
    
    while (1) {
        // Receive data from client
        int Rbytes = read(client_socket, buffer, sizeof(buffer));
        if (Rbytes <= 0) {
            printf("recv error");
            break; // Client disconnected
        }

        //rot13 (code found online)
        for (int i = 0; i < Rbytes; ++i) {
            if (isalpha(buffer[i])) {
                char base = isupper(buffer[i]) ? 'A' : 'a';
                buffer[i] = (buffer[i] - base + 13) % 26 + base;
            }
        }

        //send data back to client
        //brief pseudo code for sending to all clients:
            // for each client connected, write, 
        int Sbytes = write(client_socket, buffer, Rbytes);
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
        
        //forking
        int f = fork();
        if (f == -1) {
            printf("fork failed\n");
        }
        else if (f==0){ //child
            close(listen_socket);
            subserver_logic(client_socket);
            exit(0);
        }
        else {
            close(client_socket);
            printf("server waiting for another request\n");
        }
    }
}


