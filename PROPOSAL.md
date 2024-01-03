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
    
    The signals will be used to shut the program down gracefully. 
    
    Semaphores/Shared Memory can be used to control access to a user list. The user list would help us log which users are connected to the chat room. We could also use it to implement private messaging, though we're not sure if we'll have time for that. 
    
    
    Lily will be in charge of signals and using the semaphores to create the user list. Margaux will focus on the file operations for logging the history, along with the sockets and networking that will allow for connection back and forth. Lily will also participate in the networking part. 
     
    
# Intended pacing:

A timeline with expected completion dates of parts of the project.
