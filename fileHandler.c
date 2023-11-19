#include "fileHandler.h"
#include <stdio.h>

int fileReader(int i) {

    FILE *tagFile;
    tagFile = fopen("tags.txt", "r");
    char buffer[256];  // <---use malloc ?

    if (tagFile == NULL) {

        perror("Error opening file");
        return 1;

    }else{

        if(i == 0){ // check if tags exist in file, obselete since we already check if file isempty




        }else if(i == 1){ // files exist start sending tags  

            //TODO: add setup for splitting each line into its parts 
            while (fgets(buffer, sizeof(buffer), tagFile) != NULL) {
            // rewrite for one line at a time

            }

        }
    
    }

    fclose(tagFile);

    return 0;
}

int fileWriter() { // add new tags to file
    FILE *tagFile;

    
    tagFile = fopen("example.txt", "w");

    if (tagFile == NULL) {
        perror("Error opening file");
        return 1;
    }

    fprintf(tagFile, "Hello, World!\n");


    fclose(tagFile);

    return 0;
}