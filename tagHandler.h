#ifndef TAGHANDLER_H
#define TAGHANDLER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "gateKeeper.h"
#include "timestamp.h"

typedef struct TAG{
    char name[256];  // ex alex 
    char idS[2];     // ex C = customer V = Visitor E = Employee
    char idD[9];        //  // ex 1,2,3,4 -> etc | change to byte idD[];
    char pass[17];   // Random 16 char pass     
    int access;      // 1 = true 0 = false
    char createdTs[20]; // YYYY-MM-DD HH:MM:SS
    char changedTs[20]; // YYYY-MM-DD HH:MM:SS
} TAG;


int arrayHandler(TAG *tag, int option, int *dir);
void newTag(TAG *tag, int *direction);
int asignIdD();
int checkPass(char tPass[17]);
int fileReader();
int fileWriter(TAG *tags, int tagCount);
int isFileEmpty();
void onExit();
void changeTag(TAG *tag);
void deleteTag(TAG *tag);

#endif