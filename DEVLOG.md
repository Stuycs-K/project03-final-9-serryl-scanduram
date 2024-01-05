# Dev Log:

## Player 1 Lily

### 2024-01-02 - Brainstormed ideas for project
Decided to create a chat program that works with different users on different computers because it could utilise concepts like forking and sockets, which are fresh in our minds.

### 2024-01-03 - Came up with technical details together
Set up our files and started on the proposal. Started looking back through old labs (semaphores and signals)

### 2024-01-04 - Started working on signals
School: added sighandler file and function to double check when ctrl-c is pressed whether user wants to exit, then exit if user inputs "y".
Home: Created struct for user list (including username and socketID). Tried to incorporate that into the server main function which would give each client a new user(right now generic user#) and add them to the user list.

## Player 2 Margaux

### 2024-01-02 - Brainstormed ideas for project
Decided to create a chat program that works with different users on different computers because it could utilise concepts like forking and sockets, which are fresh in our minds.

### 2024-01-03 - Technical details and splitting up work
Finished proposal doc and made a plan for splitting work.

###2024-01-04 - School: Margaux worked on creating the basic forking server and allowing the client to submit a username. 
    Home: The username disconnect and connect works for the server, but it's showing up twice for the client, which I will try to fix tomorrow during free periods. I also went through our old labs for remembering how to save chat history and decided how we should format the retrieval of previous chats. 
