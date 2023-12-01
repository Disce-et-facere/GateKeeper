#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "tagHandler.h"

int readAndWriteTag(int option, char *receivedId, int *check);

#endif