#include "networking.h"
#include <fcntl.h>

void reader( char *username, char *message){
    FILE *log_file = fopen(LOG_FILE, "a");
    if (log_file == NULL) {
           perror("Error opening log file");
           return;
       }
    fprintf(log_file, "[%s]: %s\n", username, message);

    fclose(log_file);
}

void creator(){
    
}
