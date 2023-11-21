#ifndef TAGHANDLER_H
#define TAGHANDLER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "gateKeeper.h"

typedef struct {
    char name[256];  // ex alex 
    char idS[2];     // ex C = customer V = Visitor E = Employee
    int idD;         // ex 1,2,3,4 -> etc
    char pass[17];   // Random 16 char pass     
    int access;      // 0 = true 1 = false
} TAG;


int arrayHandler(TAG *tag, int option, int *direction);
void newTag(TAG *tag, int *direction);
int asignIdD();
int checkPass(char tPass[17]);
int fileReader();
int fileWriter(TAG *tag);
int isFileEmpty();
void onExit();

#endif