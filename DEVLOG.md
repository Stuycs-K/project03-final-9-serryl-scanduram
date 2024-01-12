# Dev Log:

## Player 1 Lily

### 2024-01-02 - Brainstormed ideas for project
Decided to create a chat program that works with different users on different computers because it could utilise concepts like forking and sockets, which are fresh in our minds.

### 2024-01-03 - Came up with technical details together
Set up our files and started on the proposal. Started looking back through old labs (semaphores and signals)

### 2024-01-04 - Started working on signals
School: added sighandler file and function to double check when ctrl-c is pressed whether user wants to exit, then exit if user inputs "y".
Home: Created struct for user list (including username and socketID). Tried to incorporate that into the server main function which would give each client a new user(right now generic user#) and add them to the user list.

###2024-01-5 - School: Lily and Margaux worked TOGETHER (on Margaux's account), to try and get the client to receive another client's messages in real time. We did not completely succeed, but we got the message to print to server and implemented some select stuff. We also fixed our makefile.
Home: Lily worked on getting the clients to send messages to eachother through the server, but only succeeded in havign the clients send to the server concisely. 

###2024-01-8 - School: Margaux and Lily worked together on Lily's account during class to debug. We spent the entire period debugging and trying to figure out where the code stops working. Unfortunately, we were unsuccessful with our print statements. 
Home: Lily worked after Margaux on trying to get the clients to communicate and figured out that the issue was that the user_list didn't update for the first client when a new one connected. She is going to maybe try using semaphores to fix that and share memory?

###2024-01-09
    School: Margaux and Lily worked together. We were trying to figure out the really hard part, so we asked mr.K, turns out we were not supposed to use a forking server, oops! We spent the rest of the period switching it to a persistent server, and now the code works!! Our clients connect and can see each others messages. 
    Home: Lily fixed a bug that Margaux was working on earlier that had messed up the username input. Also worked on the formatting of the chat so that it is clear who is sending what message.

###2024-01-09
School: Started working on sighandler. Made a plan for server and client actions with SIGINT. Started to work on client sighandler.
Home: Almost finished with client sighandler, but there is a little bug I tried to fix. Also started worker on server sighandler, but I am running into a lot of problems where the sighandler code in either client or server will disrupt the connections.


## Player 2 Margaux

### 2024-01-02 - Brainstormed ideas for project
Decided to create a chat program that works with different users on different computers because it could utilise concepts like forking and sockets, which are fresh in our minds.

### 2024-01-03 - Technical details and splitting up work
Finished proposal doc and made a plan for splitting work.

###2024-01-04 - School: Margaux worked on creating the basic forking server and allowing the client to submit a username. 
    Home: The username disconnect and connect works for the server, but it's showing up twice for the client, which I will try to fix tomorrow during free periods. I also went through our old labs for remembering how to save chat history and decided how we should format the retrieval of previous chats. 

###2024-01-5 - School: Lily and Margaux worked TOGETHER (on Margaux's account), to try and get the client to receive another client's messages in real time. We did not completely succeed, but we got the message to print to server and implemented some select stuff. We also fixed our makefile.
    Home: Margaux worked a lot on ther server/client connection, trying to get the server to broadcast the messages back to the clients. So far, the clients can send messages that will both appear on the server, but won't appear on each other's terminals. The clients messages will appear on their own terminals with their usernames though!

###2024-01-08
    School: Margaux and Lily worked together on Lily's account during class to debug. We spent the entire period debugging (I swear, people saw us) and trying to figure out where the code stops working. Unfortunately, we were unsuccessful with our print statements. 
    Home: Margaux continued the debugging by including print statements and determined that the client code never enters the server_Socket if statement, only the STDIN_FILENO. The server code only goes into the Client_Socket, not the stdin fileno. Margaux also prepped the homescreen, so that it prompts the user to see if they want to enter a chat or see previous history. 
    
###2024-01-09
    School: Margaux and Lily worked together. We were trying to figure out the really hard part, so we asked mr.K, turns out we were not supposed to use a forking server, oops! We spent the rest of the period switching it to a persistent server, and now the code works!! Our clients connect and can see each others messages. 
    Home: Margaux worked on beginning the saving of chats. She created the new history files and reconfigured makefile to include them and work properly. She also gave the client the option to sva ethe history and made a preliminary writing method for writing to a file. There are a few bugs, but ntohing that can't be fixed tomorrow.
    
###2024-01-10
    School: During school, Margaux worked on creating the history functions. The function reading a file back works, along with the creator function. I used code found online to use the current date and time to differnetiate file names. I also fixed the username issue and the some clients saving, some not. 
    Home: I created the directory print function, fixed bugs, and fixed formatting. I changed it so the client writes to the file instead of the server and removed subserver_logic as it wasn't doing anything. 
    
###2024-01-11
    School: During school, I worked on fixing the directory functions. This involved actually creating the files (oops) and fixing the formatting of how the messages were saved.
    Home: At home, I completely fixed reader, so that works. I also edited the names of the files to make them more clear to the reader, fixed the history reading loop in the client so that it reads it out to you, and gave the user the option to exit or continue after. 
