#include "networking.h"
#include "history.h"
#include <signal.h>
#include <string.h>

char* FILENAME;
char save[2];
char* IP;



static int sigint_received = 0;
static int reconnect_requested = 0; // N

static void sighandler( int signo ){
    if (signo == SIGINT){
        printf("Are you sure you want to exit? (y/n)\n");
        char input[2];
        fgets(input, sizeof(input), stdin);
        if (strcmp(input, "y") == 0){
            printf("you are exiting\n");//change to smth involving usr?
            exit(0);
        }
        else{
            printf("Resuming...\n");
            sigint_received = 1; // Set the flag to continue normal execution
            reconnect_requested = 1; // Set the flag to request reconnection
        }
    }
    else if(signo == SIGQUIT){
        printf("not sure what this does");
    }
   // sigint_received = 0;
}

void clientLogic(int server_socket, char username[50]){
    char buffer[BUFFER_SIZE];
    //char user[50];

    //prompt user input
    while (1) {
        //if (sigint_received == 1) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(server_socket, &read_fds);
            FD_SET(STDIN_FILENO, &read_fds);
            
            
            int i = select(server_socket + 1, &read_fds, NULL, NULL, NULL);
            
            if (i < 0) {
                perror("select error");
                break;
            }
        /*
        if (reconnect_requested) {
            // Reconnect to the server
            close(server_socket);
            server_socket = client_tcp_handshake(IP);
            if (server_socket == -1) {
                perror("Reconnection error");
                exit(1);
            }
            reconnect_requested = 0; // Reset the reconnection flag
        }
         */
        
       // if (sigint_received == 1) {
            if (FD_ISSET(server_socket, &read_fds)) {
                int rbytes = read(server_socket, buffer, sizeof(buffer));
                if (save[0]=='y'){
                    writer(FILENAME, buffer);
                }
                if (rbytes <= 0) {
                    perror("recv error");
                    break;
                }
                buffer[rbytes] = '\0';
                printf("%s\n", buffer);
            }
            
            if (FD_ISSET(STDIN_FILENO, &read_fds)) {
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strlen(buffer) - 1] = '\0';
                if (save[0]=='y'){
                    char message[BUFFER_SIZE + 55];
                    snprintf(message, sizeof(message), "[%s]: %s", username, buffer);
                    writer(FILENAME, buffer);
                }
                int sbytes = write(server_socket, buffer, strlen(buffer));
                if (sbytes == -1) {
                    perror("send error");
                    exit(1);
                }
            }
        }
    }



int main(int argc, char *argv[] ) {

    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);
    
    IP = "127.0.0.1";
    if(argc>1){
        IP=argv[1];
    }
    
    int server_socket = client_tcp_handshake(IP);
    printf("client connected.\n");
    
    while(1){
        printf("Please enter 'c' if you would like to enter a chat or 'h' if you would like to see previous chat histories.\n");
        char ans[2];
        fgets(ans, sizeof(ans), stdin);
        
        
        if(strcmp(ans, "h") == 0){
            directoryPrint();
            
            printf("Please enter the exact name of the history you would like to see: ");
            char historyName[100];
            scanf("%99s", historyName);
            historyName[strcspn(historyName, "\n")] = '\0';
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            printf("Selected history file: %s\n", historyName);
            reader(historyName);
            printf("Do you want to continue? (y/n): ");
            char continueChoice[2];
            fgets(continueChoice, sizeof(continueChoice), stdin);
            if (strcmp(continueChoice, "n") == 0) {
                break; // Exit the loop if 'n' is entered
            }
            
            
            
        }
        
        else {
            int s;
            while ((s = getchar()) != '\n' && s != EOF);
            printf("Do you want to save chat history? (y/n):\n");
            fgets(save, sizeof(save), stdin);
            save[strcspn(save, "\n")] = '\0';
            if (save[0]=='y') {
                printf("Ok. chat will be saved!\n");
                FILENAME = creator();
            }
            else{
                printf("Ok. chat will not be saved!\n");
            }
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            char name[100];
            printf("Enter your username: ");
            fgets(name, sizeof(name), stdin);
            for (int i = 0; i < strlen(name); i++){
                if(name[i]== '\n'|| name[i]== '\r'){
                    name[i] = 0;
                    break;
                }
            }
            int sbytes = write(server_socket, name, strlen(name));
            if (sbytes < 0) {
                perror("send error");
                close(server_socket);
                exit(1);
            }
            clientLogic(server_socket, name);
        }
    }
     
    close(server_socket);

}
