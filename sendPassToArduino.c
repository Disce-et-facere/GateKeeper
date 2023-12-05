#include "sendPassToArduino.h"

int receivePointerSendPass(char pass[17], char* receivedId) {

    CleanupResources();

   
    HANDLE h2Serial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

   if (h2Serial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM3 port\n");
        return 1;
    }

    
    DCB dcbSerialParams = {0};
    COMMTIMEOUTS timeouts = { 0 };

    if (!GetCommState(h2Serial, &dcbSerialParams)) {
 
        CloseHandle(h2Serial);
        return;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(h2Serial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(h2Serial);
        return 1;
    }

    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(h2Serial, &timeouts)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(h2Serial);
        return 1;
    }   

    DWORD bytesRead;
    DWORD bytesWritten;
    
    char idBuffer[256];

    char tempPass[17];
    strcpy(tempPass, pass);
    char preSuPass[20];
    strcpy(preSuPass, ":");
    strcat(preSuPass, tempPass);
    strcat(preSuPass, ";");

    

    if (!WriteFile(h2Serial, preSuPass, (DWORD)strlen(preSuPass), &bytesWritten, NULL)) {
        fprintf(stderr, "Error writing to serial port\n");
    }else{
        printf("SENT MESSAGE: %s\n", preSuPass);
        
        if (!FlushFileBuffers(h2Serial)) {
            fprintf(stderr, "Error flushing file buffers. Error code: %lu\n", GetLastError());
        }

    }

    const char* tagIDPrefix = "ID:";
    const char* tagIDSuffix = ":ID";
    size_t tagIDPrefixLength = strlen(tagIDPrefix);
    size_t tagIDSuffixLength = strlen(tagIDSuffix);

    while(1){
        if (ReadFile(h2Serial, idBuffer, sizeof(idBuffer) - 1, &bytesRead, NULL)) {
            if (strncmp(idBuffer, tagIDPrefix, tagIDPrefixLength) == 0 &&
            strncmp(idBuffer + bytesRead - tagIDSuffixLength, tagIDSuffix, tagIDSuffixLength) == 0) {

            char formattedID[9];
            strncpy(formattedID, idBuffer + tagIDPrefixLength, 8);
            formattedID[8] = '\0';
            printf("FORMATTED ID: %s\n", formattedID);
            strncpy(receivedId, formattedID, sizeof(receivedId));
            PurgeComm(h2Serial, PURGE_RXCLEAR | PURGE_TXCLEAR);
            break;
            }
            
        }
    }  

    CloseHandle(h2Serial);

    InitializeAndStartListenThread();
}
