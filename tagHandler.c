#include "tagHandler.h"

// TODO's: 
// add timestamp created/changed


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
            printf("Timestamp: %s\n", tag->createdTs);
            strncpy(tags[tagCount].name, tag->name, sizeof(tags[tagCount].name));
            tags[tagCount].name[sizeof(tags[tagCount].name) - 1] = '\0';

            strncpy(tags[tagCount].idS, tag->idS, sizeof(tags[tagCount].idS));
            tags[tagCount].idS[sizeof(tags[tagCount].idS) - 1] = '\0';

            tags[tagCount].idD = tag->idD;

            strncpy(tags[tagCount].pass, tag->pass, sizeof(tags[tagCount].pass));
            tags[tagCount].pass[sizeof(tags[tagCount].pass) - 1] = '\0';

            tags[tagCount].access = tag->access;

            strncpy(tags[tagCount].createdTs, tag->createdTs, sizeof(tags[tagCount].createdTs));
            tags[tagCount].createdTs[sizeof(tags[tagCount].createdTs) - 1] = '\0';


            strncpy(tags[tagCount].changedTs, tag->changedTs, sizeof(tags[tagCount].changedTs));
            tags[tagCount].changedTs[sizeof(tags[tagCount].changedTs) - 1] = '\0';


            if(*direction == 1){

                fileALI(tag);

            }
            
            tagCount++;
            return 0;

        } else { // if tags exists -> realloc
            
            tags = realloc(tags, sizeof(TAG) * (tagCount + 1)); // this was the clusterfick
                                                                // tagCount + 1 -> (tagCount + 1) 
            if (tags == NULL) {  
            fprintf(stderr, "Memory reallocation failed.\n");
            exit(EXIT_FAILURE); 
            }

            printf("%s\n",tag->name);
            strncpy(tags[tagCount].name, tag->name, sizeof(tags[tagCount].name));
            tags[tagCount].name[sizeof(tags[tagCount].name) - 1] = '\0';

            strncpy(tags[tagCount].idS, tag->idS, sizeof(tags[tagCount].idS));
            tags[tagCount].idS[sizeof(tags[tagCount].idS) - 1] = '\0';

            tags[tagCount].idD = tag->idD;

            strncpy(tags[tagCount].pass, tag->pass, sizeof(tags[tagCount].pass));
            tags[tagCount].pass[sizeof(tags[tagCount].pass) - 1] = '\0';

            tags[tagCount].access = tag->access;

            strncpy(tags[tagCount].createdTs, tag->createdTs, sizeof(tags[tagCount].createdTs));
            tags[tagCount].createdTs[sizeof(tags[tagCount].createdTs) - 1] = '\0';


            strncpy(tags[tagCount].changedTs, tag->changedTs, sizeof(tags[tagCount].changedTs));
            tags[tagCount].changedTs[sizeof(tags[tagCount].changedTs) - 1] = '\0';
            

            tagCount++; 
            
            if(*direction == 1){

                fileALI(tag);
            }
            return 0;
        }
    }else if(option == 1){ // option = 1 make changes in tag
            //implement changes


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
    }else if(option == 8){ // save tags to file and  release memory onExit()


        fileWriter(tags, tagCount);
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

    strncpy(newTag.createdTs, tag->createdTs, sizeof(newTag.createdTs) - 1);
    newTag.createdTs[sizeof(newTag.createdTs) - 1] = '\0'; 

    strncpy(newTag.changedTs, tag->changedTs, sizeof(newTag.changedTs) - 1);
    newTag.changedTs[sizeof(newTag.changedTs) - 1] = '\0'; 

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
            
            if (sscanf(line, "%255[^,], %1s %d %16s %d %19s %19s", tag.name, tag.idS, &tag.idD, tag.pass, &tag.access, tag.createdTs, tag.changedTs) == 7) {
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

                fprintf(tagFileW, "%s, %s %d %s %d %s %s\n", tags[i].name, tags[i].idS, tags[i].idD, tags[i].pass, tags[i].access, tags[i].createdTs, tags[i].changedTs);

            }
        
            fclose(tagFileW);
    }
    return 0;
}