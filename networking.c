#include "networking.h"

//149.89.161.120
//looopback address: 127.0.0.1

void err(int i, char*message){
  if(i < 0){
      printf("Error: %s - %s\n",message, strerror(errno));
      exit(1);
  }
}

/*Connect to the server
 *return the to_server socket descriptor
 *blocks until connection is made.*/
int client_tcp_handshake(char * server_address) {
    //getaddrinfo, socket, connect
    struct addrinfo *hints, *results;
    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    

  //getaddrinfo->fills in my struct for me
    int status = getaddrinfo(server_address, PORT, hints, &results);  //Server sets node to NULL
    err(status, "getaddrinfo error");
  
    
  //create the socket
    int serverd;
    serverd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    err(serverd, "socket error");
  
  //connect to the server
    status = connect(serverd, results->ai_addr, results->ai_addrlen);
    err(status, "connect error");
  
    free(hints);
    freeaddrinfo(results);

    return serverd;
}

/*Accept a connection from a client
 *return the to_client socket descriptor
 *blocks until connection is made.
 */
int server_tcp_handshake(int listen_socket){
    // accept   
    
    int client_socket;
    struct sockaddr_storage client_address;
    socklen_t sock_size = sizeof(client_address);

    //accept the client connection
    client_socket = accept(listen_socket,(struct sockaddr *)&client_address, &sock_size);
    err(client_socket, "accept error");
  
    return client_socket;
}

/*Create and bind a socket.
* Place the socket in a listening state.
*/
int server_setup() {
    //get addrinfo, socket, bind, listen
    
    //setup structs for getaddrinfo
    struct addrinfo * hints, * results;
    hints = calloc(1,sizeof(struct addrinfo));
    hints->ai_family = AF_INET;
    hints->ai_socktype = SOCK_STREAM; //TCP socket
    hints->ai_flags = AI_PASSIVE; //only needed on server
    
    int status = getaddrinfo(NULL, PORT, hints, &results);  //Server sets node to NULL
    err(status, "getaddrinfo error");
  
    //create the socket
    int clientd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);
    err(clientd, "socket error");
    
    //this code should get around the address in use error
    int yes = 1;
    int sockOpt =  setsockopt(clientd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));
    err(sockOpt,"sockopt  error");
  
    //bind the socket to address and port
    status = bind(clientd, results->ai_addr, results->ai_addrlen);
    err(status, "binding error");
  
    //set socket to listen state
    status = listen(clientd, BUFFER_SIZE);
    err(status, "listening error");
  

    //free the structs used by getaddrinfo
    free(hints);
    freeaddrinfo(results);
  
  return clientd;
}

