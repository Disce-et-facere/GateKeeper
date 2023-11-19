#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tagHandler.h"
#include "fileHandler.h"

// DO NOT RUN YET !
// since free() function isnt implemented and would cause memory leaks

int arrayHandler(TAG tag, int option){

    TAG *tags = NULL;
    size_t tagCount = 0;
    int availableIdD = 1;


    if(option == 0){ // option = 0 adds tags
        if(fileReader(0) == 0){ // if file is empty -> malloc

            tags = malloc(sizeof(TAG)); 

            if (tags == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
            }
            
            strcpy(tags[tagCount].name, tag.name);
            strcpy(tags[tagCount].idS, tag.idS);
            tags[tagCount].idD = tag.idD;
            strcpy(tags[tagCount].pass, tag.pass);
            tags[tagCount].access = tag.access;

            tagCount++;

        } else { // if tags exists in file -> realloc

            tags = realloc(tags, sizeof(TAG) * tagCount + 1);

            if (tags == NULL) {
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(EXIT_FAILURE);
            }

            strcpy(tags[tagCount].name, tag.name);
            strcpy(tags[tagCount].idS, tag.idS);
            tags[tagCount].idD = tag.idD;
            strcpy(tags[tagCount].pass, tag.pass);
            tags[tagCount].access = tag.access;

        }
    }else if(option == 1){ // option = 1 make changes in tag



    }else if(option == 2){ // option = 2 Check array for avaliable idD
        if(tags == NULL){
            return 1;
        }else{
            for(size_t i = 0; i < tagCount; i++){ // iterates through the tag array to see what idD number is available.
                
                if(tags[i].idD == availableIdD){
                    ++availableIdD;
                    i = 0;
                }

            }
            return availableIdD;
        }
    }else if(option == 3){ // checks if new password exists
        if(tags == NULL){
            return 0;
        }else{

        }
    }
    return 0;
}   

void newTag(const char tName[256], const char tIdS[2], const int tIdD, const char tPass[17], const int tAccess){
    
    TAG tag;

    strncpy(tag.name, tName, sizeof(tag.name) - 1);
    tag.name[sizeof(tag.name) - 1] = '\0';

    strncpy(tag.idS, tIdS, sizeof(tag.idS) - 1);
    tag.idS[sizeof(tag.idS) - 1] = '\0'; 

    tag.idD = tIdD;

    strncpy(tag.pass, tPass, sizeof(tag.pass) - 1);
    tag.pass[sizeof(tag.pass) - 1] = '\0';

    tag.access = tAccess;

    arrayHandler(tag, 0);
}

int checkIdD(){
    TAG tag;
    
    int newIdD = arrayHandler(tag, 2);

    return newIdD;
}

int checkPass(char tPass[17]){
    TAG tag;
    strncpy(tag.pass, tPass, sizeof(tag.pass) - 1);
    tag.pass[sizeof(tag.pass) - 1] = '\0';

    int i = arrayHandler(tag, 3);
    if(i == 0){

        return 0;
        
    }else

    return 1;
} 

void changeTag(){

}

void readTag(){

}

void sendTags(){

}

void removeTag(){
    
}

void saveTags(){

}