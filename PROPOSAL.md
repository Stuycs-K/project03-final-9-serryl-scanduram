# Final Project Proposal

## Group Members:

Lily Serry and Margaux Scandura
       
# Intentions:

    We are going to create chat room that will allow for two or more separate users on two or more separate computers to communicate through their terminals.
    
# Intended usage:

    Users will create their username, along with adding in the server's IP address. They will then enter a starting message when prompted. After sending one intial message, they will be entered into the chat room and will be able to send a receive chats in real time. They can disconnect from the chat room using a specific key word. 

# Technical Details:

    This project utilizes sockets and networking, file operations, signals, and shared memory and semaphores. 
    
    The sockets and networking will be implemented for communication between a client and sevrer, and will help the chatroom establish connections and send and receive messages. Socket programming could also be used for private messaging, but that might not be implemented. 
    
    The file operations will be used to log/save the chat history. This involves using read and write. 
    
    The signals will be used to shut the program down. 
        
    
    Lily will be in charge of signals and creating the user list. Margaux will focus on the file operations for logging the history. Margaux will start on creating a basic server, then Lily and Margaux will work together to enable the clients to talk to each other through the server.
     
    
# Intended pacing:
    From January 4th-5th (in class and at home), we will start with picking and choosing from all of our other labs and implementing the proper parts. This involves the shared memory lab, the socket lab, the csv lab, and the signal lab. Margaux will also create a simple client and server. The server should be a forking server. We can also make sure that the client can put in input. 
    
    From January 8th-10th, Margaux will begin creating the file operations and figuring out how to save the chat memory. Lily will work on using signals to create a graceful exit from the chat room. 
    
    From January 11th-12th, Margaux will figure out how to create real time messages, while Lily creates the user list. 
    
    From January 13th-END, we will debug and work on visual aspects of the program. Though it will be the weekend, we will also practice our project and tie together all loose ends. 
    
