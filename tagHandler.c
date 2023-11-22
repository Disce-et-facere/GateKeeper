#include "tagHandler.h"

int arrayHandler(TAG *tag, int option, int *direction){

    static TAG *tags = NULL;
    static int tagCount = 0;

    if(option == 0){ // option = 0 adds tags
        if(tagCount == 0){ //  -> malloc

            tags = malloc(sizeof(TAG)); 

            if (tags == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE); // to harsh maybe? 
            }
            
            strncpy(tags[tagCount].name, tag->name, sizeof(tags[tagCount].name));
            tags[tagCount].name[sizeof(tags[tagCount].name) - 1] = '\0';

            strncpy(tags[tagCount].idS, tag->idS, sizeof(tags[tagCount].idS));
            tags[tagCount].idS[sizeof(tags[tagCount].idS) - 1] = '\0';

            tags[tagCount].idD = tag->idD;

            strncpy(tags[tagCount].pass, tag->pass, sizeof(tags[tagCount].pass));
            tags[tagCount].pass[sizeof(tags[tagCount].pass) - 1] = '\0';

            tags[tagCount].access = tag->access;

            if(*direction == 0){

                fileWriter(tag);

            }else if(*direction == 1){

                fileALI(tag);

            }
            
            tagCount++;

        } else { // if tags exists in file -> realloc

            tags = realloc(tags, sizeof(TAG) * tagCount + 1);

            if (tags == NULL) {  //this will never be true after malloc!!!
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(EXIT_FAILURE); // to harsh maybe? 
            }

            strncpy(tags[tagCount].name, tag->name, sizeof(tags[tagCount].name));
            tags[tagCount].name[sizeof(tags[tagCount].name) - 1] = '\0';

            strncpy(tags[tagCount].idS, tag->idS, sizeof(tags[tagCount].idS));
            tags[tagCount].idS[sizeof(tags[tagCount].idS) - 1] = '\0';

            tags[tagCount].idD = tag->idD;

            strncpy(tags[tagCount].pass, tag->pass, sizeof(tags[tagCount].pass));
            tags[tagCount].pass[sizeof(tags[tagCount].pass) - 1] = '\0';

            tags[tagCount].access = tag->access;

            
                printf("Element %d:\n", 2);
                printf("  Name: %s\n", tags[1].name);
                printf("  idS: %s\n", tags[1].idS);
                printf("  idD: %d\n", tags[1].idD);
                printf("  Pass: %s\n", tags[1].pass);
                printf("  Access: %d\n", tags[1].access);
                printf("\n");
            

            if(*direction == 0){

                fileWriter(tag);

            }else if(*direction == 1){

                fileALI(tag);
            }

            tagCount++;
        }
    
    }else if(option == 1){ // option = 1 make changes in tag



    }else if(option == 2){ // option = 2 Check array for avaliable idD
        
        int availableIdD = 1;

        if(tagCount == 0){

            return 1;
           
        }else{
          
            int i = 0;

            while(i < tagCount){
                    
                    if(tags[i].idD == availableIdD){ 
                        availableIdD++;
                        i = 0;
                    }else{
                        i++;
                    }
            }

            return availableIdD;
        }

    }else if(option == 3){ // checks if new password exists

        if(tags == NULL){ // if there is no tags return 0 since there is no current passwords

            return 0;

        }else{  // if tags exists check if current pass exists

            for(int i = 0; i < tagCount; i++){

                if(strcmp(tags[i].pass, tag->pass) == 0){

                    return 1;

                }

            }

            return 0;

        }

    }else if(option == 9){ // release memory when closing program / calling onExit()
        free(tags);
    }
    return 0;
}   

void newTag(TAG *tag, int *direction){ // from user dir 0, from file dir 1
    
    TAG newTag;

    strncpy(newTag.name, tag->name, sizeof(newTag.name) - 1);
    newTag.name[sizeof(newTag.name) - 1] = '\0'; 
    
    strncpy(newTag.idS, tag->idS, sizeof(newTag.idS) - 1);
    newTag.idS[sizeof(newTag.idS) - 1] = '\0'; 

    newTag.idD = tag->idD;

    strncpy(newTag.pass, tag->pass, sizeof(newTag.pass) - 1);
    newTag.pass[sizeof(newTag.pass) - 1] = '\0';

    newTag.access = tag->access;

    arrayHandler(&newTag, 0, direction);
}

int asignIdD(){

    TAG tag;
    int direction = 3; // 3 isnt used -> leads nowhere

    int newIdD = arrayHandler(&tag, 2, &direction);

    return newIdD;
}

int checkPass(char tPass[17]){
    TAG tag;
    int direction = 3;

    strncpy(tag.pass, tPass, sizeof(tag.pass) - 1);
    tag.pass[sizeof(tag.pass) - 1] = '\0';

     
    if(arrayHandler(&tag, 3 , &direction) == 0){

        return 0;
        
    }else

        return 1;
} 

void onExit(){
    TAG tag;
    int direction = 3;
    arrayHandler(&tag, 9, &direction);
}

// Reads tags from file
int fileReader() {

    FILE *tagFile = fopen("tags.txt", "r");
    int direction = 1;

    if (tagFile != NULL) {
        char line[256]; // Adjust the buffer size as needed

        while (fgets(line, sizeof(line), tagFile) != NULL) {
            // Check for an empty line
            if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0) {
                continue;  // Skip empty lines
            }

            TAG tag;

            // Use sscanf to parse the content of the line
            if (sscanf(line, "%255[^,], %1s %d %16s %d", tag.name, tag.idS, &tag.idD, tag.pass, &tag.access) == 5) {
                // Process the tag
                newTag(&tag, &direction);
            } else {
                fprintf(stderr, "Error parsing line: %s\n", line);
                // Handle the error if needed
            }
        }

        
        return 0;
    } else {
        perror("Error opening file/ READ");
        return -1;
    }

    fclose(tagFile);
}

// saves tag to file
int fileWriter(TAG *tag) { // add new tags to file

    FILE *tagFile = fopen("tags.txt", "a");

    if (tagFile != NULL) {

        fprintf(tagFile, "%s, %s %d %s %d\n", tag->name, tag->idS, tag->idD, tag->pass, tag->access);

        

    } else {

        perror("Error opening file/ APPEND");
        return -1;

    } 

    fclose(tagFile);
    return 0;
}

// checks if file is empty
int isFileEmpty(){

    FILE *tagsFile = fopen("tags.txt", "r");

    if (tagsFile == NULL) {
        perror("Error opening file / isFileEmpty");
        return -1; 
    }

    fseek(tagsFile, 0, SEEK_END);  
    long fileSize = ftell(tagsFile); 

    

    if (fileSize == 0) {
        return 0; // File is empty
    } else {
        return 1; // File is not empty
    }
fclose(tagsFile);
}
