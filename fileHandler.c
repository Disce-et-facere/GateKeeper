#include "fileHandler.h"

int fileReader() {

    FILE *tagFile;
    int direction = 1;

    if (tagFile != NULL) {

        TAG tag;

        tagFile = fopen("tags.txt", "r");

        while(fscanf(tagFile, "%255s %1s %d %16s %d", tag.name, tag.idS, &tag.idD, tag.pass, &tag.access) == 5){
            
            newTag(&tag, &direction);

        }

        fclose(tagFile);
        return 0; // not needed? good for something? :P
        
    }else{ 
        
        perror("Error opening file/ READ");
        return -1;

    } 

}

int fileWriter(TAG *tag) { // add new tags to file

    FILE *tagFile;

    if (tagFile != NULL) {
        
        tagFile = fopen("tags.txt", "a");

        fprintf(tagFile, "%s %s %d %s %d\n", tag->name, tag->idS, tag->idD, tag->pass, tag->access);

        fclose(tagFile);

    } else {

        perror("Error opening file/ APPEND");
        return -1;

    } 
    return 0;
}

int isFileEmpty(){

    FILE *tagsFile = fopen("tags.txt", "r");

    if (tagsFile == NULL) {
        perror("Error opening file");
        return -1; 
    }

    fseek(tagsFile, 0, SEEK_END);  
    long fileSize = ftell(tagsFile); 

    fclose(tagsFile);

    if (fileSize == 0) {
        return 0; // File is empty
    } else {
        return 1; // File is not empty
    }

}

void changeInFile(){

}