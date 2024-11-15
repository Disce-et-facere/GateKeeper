#ifndef ARDUINOHANDLER_H
#define ARDUINOHANDLER_H

#include <stdio.h>
#include <Windows.h>
#include <string.h>
#include "tagHandler.h"

#define MAX_BUFFER_SIZE 256

enum State {
    IDLE_STATE,
    RUNNING_STATE,
};

// Define your TAG struct
typedef struct {
    char id[9];
    char pass[17];
    bool openDoor;
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
void openDoor();


#endif