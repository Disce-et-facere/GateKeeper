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

            tagCount++; 
            
            if(*direction == 1){

                fileALI(tag);
            }
            return 0;
        }
    }else if(option == 1){ // option = 1 make changes in tag / well, just replace tags
           
            for (int i = 0; i < tagCount; ++i) {

                if (tags[i].idD == tag->idD) {
                    
                    tags[i] = *tag;
                    printf("tva: \n");
                    printf("%s\n",tags[tagCount].name);
                    printf("%s\n",tags[tagCount].idS);
                    printf("%d\n",tags[tagCount].idD);
                    printf("%s\n",tags[tagCount].pass);
                    printf("%d\n",tags[tagCount].access);
                    printf("%s\n",tags[tagCount].createdTs);
                    printf("%s\n",tags[tagCount].changedTs);
                    break; 
                }
            }

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

        // Find tag to remove by its idD
        for (int i = 0; i < tagCount; ++i) {
            if (tags[i].idD == tag->idD) {
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


        fileWriter(tags, tagCount);
        free(tags);
    
    }
    return 0;
}   

// remove newTag and ChangeTag? they are obsolete as we can just pass the tag directly to arrayHandler.
// Cleanup later
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

    strncpy(newTag.createdTs, tag->createdTs, sizeof(newTag.createdTs) - 1);
    newTag.createdTs[sizeof(newTag.createdTs) - 1] = '\0'; 

    strncpy(newTag.changedTs, tag->changedTs, sizeof(newTag.changedTs) - 1);
    newTag.changedTs[sizeof(newTag.changedTs) - 1] = '\0'; 

    arrayHandler(&newTag, 0, direction);
}

void changeTag(TAG *tag){ 

    TAG changeTag;
    
     strncpy(changeTag.name, tag->name, sizeof(changeTag.name) - 1);
    changeTag.name[sizeof(changeTag.name) - 1] = '\0'; 
    
    strncpy(changeTag.idS, tag->idS, sizeof(changeTag.idS) - 1);
    changeTag.idS[sizeof(changeTag.idS) - 1] = '\0'; 

    changeTag.idD = tag->idD;

    strncpy(changeTag.pass, tag->pass, sizeof(changeTag.pass) - 1);
    changeTag.pass[sizeof(changeTag.pass) - 1] = '\0';

    changeTag.access = tag->access;

    strncpy(changeTag.createdTs, tag->createdTs, sizeof(changeTag.createdTs) - 1);
    changeTag.createdTs[sizeof(changeTag.createdTs) - 1] = '\0'; 

    strncpy(changeTag.changedTs, tag->changedTs, sizeof(changeTag.changedTs) - 1);
    changeTag.changedTs[sizeof(changeTag.changedTs) - 1] = '\0'; 


    // arrayHandler(&changeTag, 1,3);
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
    
    TAG tag; // dont like these dummies -> solution?!
    int direction = 3;
    arrayHandler(&tag, 8, &direction); // saves tags to file and release allocated memory

}

// Reads tags from file
int fileReader() {

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
            
            if (sscanf(line, "%255[^,], %1s %d %16s %d %19[^,], %19s", tag.name, tag.idS, &tag.idD, tag.pass, &tag.access, tag.createdTs, tag.changedTs) == 7) {
                // Process the tag
                newTag(&tag, &direction);
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
int fileWriter(TAG *tags, int tagCount) { // add new tags to file

    if(tagCount > 0){

        FILE *tagFileW = fopen("tags.txt", "w");

            if (tagFileW == NULL) {
            perror("Error opening file for writing");
            return -1;
            }
            
            for(int i = 0; i < tagCount; i++){

                fprintf(tagFileW, "%s, %s %d %s %d %s, %s\n", tags[i].name, tags[i].idS, tags[i].idD, tags[i].pass, tags[i].access, tags[i].createdTs, tags[i].changedTs);

            }
        
            fclose(tagFileW);
    }
    return 0;
}