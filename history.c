#include "networking.h"
#include <fcntl.h>
#include <time.h>

#define MAX_LINE_LENGTH 1000

void writer( char* filename, char *username, char *message){
    FILE *log_file = fopen("./log.txt", "a");
    if (log_file == NULL) {
           perror("Error opening log file");
           return;
       }
    fprintf(log_file, "[%s]: %s\n", username, message);

    fclose(log_file);
}

void reader(char *filename){
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

char* creator(){
    time_t t = time(NULL);
    struct tm tm_info = *localtime(&t); //found this code online
    
    char *filename = malloc(200);
    if (filename == NULL) {
            perror("Memory allocation error");
            exit(EXIT_FAILURE);
        }
    
    strcpy(filename, "./chat_history_");
    sprintf(filename + strlen(filename), "%04d%02d%02d_%02d%02d%02d.txt",
                tm_info.tm_year + 1900, tm_info.tm_mon + 1, tm_info.tm_mday,
                tm_info.tm_hour, tm_info.tm_min, tm_info.tm_sec);
    
    strcat(filename, ".txt");
    
    return filename;
}

void directoryPrint(){
    
}
