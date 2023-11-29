#include "tagHandler.h"

int arrayHandler(TAG *tag, int option, int *direction){

    static TAG *tags = NULL;
    static int tagCount = 0;

    if(option == 0){ // Add tag to array
        if(tagCount == 0){ //  -> malloc

            tags = malloc(sizeof(TAG)); 

            if (tags == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            exit(EXIT_FAILURE);
            }
            
            tags[tagCount] = *tag;
            printf("ett: %s\n", tags[tagCount].changedTs);
            if(*direction == 1){

                fileALI(tag);

            }
            
            tagCount++;
            return 0;

        } else { // if tags exists -> realloc
            
            tags = realloc(tags, sizeof(TAG) * (tagCount + 1)); 
            
            if (tags == NULL) {  
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(EXIT_FAILURE); 
            }
            tags[tagCount] = *tag;
            printf("ett: %s\n", tags[tagCount].changedTs);
            tagCount++; 
            
            if(*direction == 1){

                fileALI(tag);
            }
            return 0;
        }
    }else if(option == 1){ // option = 1 make changes in tag / well, just replace tags

            // need to change this -> idD will be bytes array[];
            for (int i = 0; i < tagCount; ++i) {

                if (strcmp(tags[i].idD, tag->idD) == 0) {
                    
                    tags[i] = *tag;
                    printf("tva: \n");
                    printf("%s\n",tag->name);
                    printf("%s\n",tag->changedTs);
                    break; 
                }
            }

    }else if(option == 2){ // TODO: check if tag exist by pass, and then check if tag has access or not
        
        for (int i = 0; i < tagCount; i++) {
            if (strcmp(tags[i].pass, tag->pass) == 0) {
                
                return tags[i].access;
            }
        }

    
        return -1; 
    


    }else if(option == 3){ // checks if new password exists

        if(tagCount == 0){ // if there is no tags return 0 since there is no current passwords

            return 0;

        }else{  // if tags exists check if current pass exists

            for(int i = 0; i < tagCount; i++){

                if(strcmp(tags[i].pass, tag->pass) == 0){

                    return 1;

                }

            }

            return 0;

        }
    }else if(option == 4){ // remove tag from array

        int indexToRemove = -1;
        // change this to be compatible with byte array[4]
        // Find tag to remove by its idD | change to by its pass -> strcmp 
        
        for (int i = 0; i < tagCount; ++i) {
            if (strcmp(tags[i].pass, tag->pass) == 0) {
                indexToRemove = i;
                break;
            }
        }

        if (indexToRemove != -1) {

            // Fills the gap
            for (int i = indexToRemove; i < tagCount - 1; ++i) {
                tags[i] = tags[i + 1];
            }

            --tagCount;
            
            // Resizes memory
            TAG *temp = realloc(tags, sizeof(TAG) * tagCount); // temp array in case of failure
            if (temp != NULL) {  
                tags = temp;
            } else {
                fprintf(stderr, "Memory reallocation failed. REMOVE\n");
                exit(EXIT_FAILURE);
            }
        }


    }else if(option == 8){ // save tags to file and  release memory onExit()

        char listPassDummie[17];
        char listIdDDummie[12];
        listIdDDummie[0] = '\0';
        readAndWriteTag(3,listPassDummie, listIdDDummie, sizeof(listIdDDummie));
        fileWriter(tags, tagCount);
        free(tags);

    
    }
    return 0;
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
    
    TAG tag; // dont like these dummies -> solution?!
    int direction = 3;
    arrayHandler(&tag, 8, &direction); // saves tags to file and release allocated memory

}

// Reads tags from file
int fileReader() { // change this to be compatible with byte array[4]

    FILE *tagFileR = fopen("tags.txt", "r");
    int direction = 1;

    if (tagFileR != NULL) {
        char line[256]; 

        while (fgets(line, sizeof(line), tagFileR) != NULL) {
            // Check for an empty line
            if (strcmp(line, "\n") == 0 || strcmp(line, "\r\n") == 0) {
                continue;  
            }

            TAG tag;
            
            if (sscanf(line, "%255[^,], %1s %11s %16s %d %19[^,], %19[^,],", tag.name, tag.idS, &tag.idD, tag.pass, &tag.access, tag.createdTs, tag.changedTs) == 7) {
                // Process the tag
                //newTag(&tag, &direction);
                arrayHandler(&tag, 0, direction);
            } else {
                fprintf(stderr, "Error parsing line: %s\n", line);
                
            }
        }

        fclose(tagFileR);
        return 0;
    } else {
        perror("Error opening file/ READ");
        return -1;
    }
    
}

// saves tag to file
int fileWriter(TAG *tags, int tagCount) { // add new tags to file // change this to be compatible with byte array[4]

    if(tagCount > 0){

        FILE *tagFileW = fopen("tags.txt", "w");

            if (tagFileW == NULL) {
            perror("Error opening file for writing");
            return -1;
            }
            
            for(int i = 0; i < tagCount; i++){

                fprintf(tagFileW, "%s, %s %s %s %d %s, %s,\n", tags[i].name, tags[i].idS, tags[i].idD, tags[i].pass, tags[i].access, tags[i].createdTs, tags[i].changedTs);

            }
        
            fclose(tagFileW);
    }
    return 0;
}