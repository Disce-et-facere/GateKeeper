#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>
#include "tagHandler.h"

typedef struct TAG TAG;

int fileReader();
int fileWriter(TAG *tag);
int isFileEmpty();

#endif