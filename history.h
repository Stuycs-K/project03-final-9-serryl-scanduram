#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>


#ifndef HISTORY_H
#define HISTORY_H
#define BUFFER_SIZE 1024
void reader(char *filename);
void writer( char* filename, char *username, char *message);
char * creator();

#endif


