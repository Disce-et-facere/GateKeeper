#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <stdio.h>
#include <Windows.h>
#include "tagHandler.h"

#define MAX_BUFFER_SIZE 17

int readAndWriteTag(int option, char pass[17], char *receivedId, size_t idSize);

#endif