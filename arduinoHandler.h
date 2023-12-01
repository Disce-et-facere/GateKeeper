#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "tagHandler.h"

#define MAX_BUFFER_SIZE 256

enum State {
    IDLE_STATE,
    START_UP,
    SET_ID,
    SEND_PASS,
    ERROR_STATE
};

// Define your TAG struct
typedef struct {
    char id[9];
    char pass[17];
    char success[7];
    // Add other fields if needed
} MESSAGE;

void HandleArduinoEvent(HANDLE hSerial);
void GetLatestTag(MESSAGE* message);
void SendDataToArduino(HANDLE hSerial, const char* data);
DWORD WINAPI ListenThreadFunction(LPVOID lpParam);
HANDLE StartListenThread(HANDLE hSerial);
void StopListenThread(HANDLE hThread);
void CleanupResources();
void ProcessReceivedPass(const char* pass);
int InitializeAndStartListenThread();
void hexStringToCharArray(const char* hexString, char* charArray);
int readAndWriteTag(int option, char pass[17], char *receivedId);


#endif