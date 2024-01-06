#include "networking.h"


void clientLogic(int server_socket){
    char buffer[BUFFER_SIZE];
    char username [50];
    
    //prompt user input
    
    
    //printf("Hello. Please type a message to get started!\n ", username);
    //fgets(chat, sizeof(chat), stdin);
    
    
    // Receive prompt for username change
    int rbytes = read(server_socket, buffer, sizeof(buffer));
    if (rbytes > 0) {
        buffer[rbytes] = '\0';
        printf("%s", buffer);

        // Input new username
        fgets(username, sizeof(username), stdin);
        username[strcspn(username, "\n")] = 0; // Remove newline character
        write(server_socket, username, strlen(username));

        printf("Username changed to: %s\n", username);
    } else {
        perror("recv error");
    }

    close(server_socket);
}

int main(int argc, char *argv[] ) {
    
    char* IP = "127.0.0.1";
    if(argc>1){
        IP=argv[1];
    }
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    
    /*
    char name[100];
    char buffer[BUFFER_SIZE];
    printf("Enter your username: ");
    fgets(name, sizeof(name), stdin);
    
    // Remove newline from the end of the username
    for (int i = 0; i < strlen(name); i++){
        if (name[i] == '\n' || name[i] == '\r'){
            name[i] = 0;
            break; // Stop once newline is removed
        }
    }
    */
    while(1){
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
        clientLogic(server_socket);
        //usleep(50);
    }
    close(server_socket);
}
