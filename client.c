#include "networking.h"


void clientLogic(int server_socket, char name[100]){
    char chat [BUFFER_SIZE];
   // char name [100];
    
    //prompt user input
    
    
    printf("Hello, %s. Please type a message to get started!\n ", name);
    fgets(chat, sizeof(chat), stdin);
    
    
    // Send string to  server
    int  sbytes = write(server_socket, chat, strlen(chat));
    if (sbytes<0){
        perror("send error");
        exit(1);
    }

    // Receive  modified string from  server
        int rbytes = read(server_socket, chat, sizeof(chat));
        if (rbytes > 0) {
            // Print the modified string
            printf("[%s]: %s\n", name, chat);
        } else {
            perror("recv error");
        }
}

int main(int argc, char *argv[] ) {

    char* IP = "127.0.0.1";
    if(argc>1){
      IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    while(1){
        char name[100];
        printf("Enter your username: ");
        fgets(name, sizeof(name), stdin);
        
        for (int i = 0; i<strlen(name); i++){
            if(name[i]== '\n'|| name[i]== '\r'){
                name[i] = 0;
            }
        }
        clientLogic(server_socket, name);
        usleep(50);
    }
    close(server_socket);
}

