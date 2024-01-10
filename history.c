#include "networking.h"
#include <fcntl.h>

#define MAX_LINE_LENGTH 1000

void writer( char *username, char *message){
    FILE *log_file = fopen("./log.txt", "a");
    if (log_file == NULL) {
           perror("Error opening log file");
           return;
       }
    fprintf(log_file, "[%s]: %s\n", username, message);

    fclose(log_file);
}

void reader(){
    FILE *r_file = fopen("./log.txt", "r");
    if(r_file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), r_file) != NULL) {
            // Print each line
            printf("%s", line);
        }

        fclose(r_file);
}

void creator(){
    
}
