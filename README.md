[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/SQs7pKlr)
# PROJECT NAME HERE

### GROUP NAME HERE

Lily Serry and Margaux Scandura
       
### Project Description:

This project is a chat room that will allow for two or more separate users on two or more separate computers to communicate through their terminals.
  
### Instructions:

To compile, enter make compile into your terminal<br>
To create a server to run, type ./server into your terminal. <br>
To connect to a server, type ./client into your terminal. If the server is already running on YOUR computer, just enter ./client. If the server is on a different computer, enter ./client SERVERIPADDRESS.<br>
Then you will be prompted with questions and connected to the chat.<br>
To exit the chat, press ^c.<br>

There are some bugs/things we couldn't quite make work:<br>
1. Not all of the clients are automatically disconnected when the server disconnects. This is due to the reconnection part of the client control c.<br>
2. When the client inputs that they didn't mean to disconnect, the code doesn't put them back in their chat, it brings them back to the home screen.<br>
3. Not really a bug, but the code doesn't tell the clients whether another user is saving the chat or not. It also doesn't show up on the client side if another client has connected.<br>
