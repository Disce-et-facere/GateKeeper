#include "arduinoHandler.h"


// Globals
volatile MESSAGE sharedMessage;
CRITICAL_SECTION messageCriticalSection;
HANDLE hListenThread = NULL;
HANDLE hSerial;
HANDLE hExitEvent;
bool running = true;


void HandleArduinoEvent(HANDLE hSerial) {
    DWORD bytesRead;
    char buffer[MAX_BUFFER_SIZE];

    const char* tagPassPrefix = "TAGPASS:";
    const char* tagPassSuffix = ":TAGPASS";
    size_t tagPassPrefixLength = strlen(tagPassPrefix);
    size_t tagPassSuffixLength = strlen(tagPassSuffix);

    if (ReadFile(hSerial, buffer, sizeof(buffer) - 1, &bytesRead, NULL)) {

        buffer[bytesRead] = '\0';

        if (strstr(buffer, tagPassPrefix) != NULL) {

            char* passwordStart = strstr(buffer, tagPassPrefix) + tagPassPrefixLength;
            char* passwordEnd = strstr(passwordStart, tagPassSuffix);
            
            if (passwordEnd != NULL) {
                int passLength = passwordEnd - passwordStart;
                char formattedPass[17];
                strncpy(formattedPass, passwordStart, passLength);
                formattedPass[passLength] = '\0';

                ProcessReceivedPass(formattedPass);
                PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
            }
        }

    }
}




void GetLatestTag(MESSAGE* message) {
    EnterCriticalSection(&messageCriticalSection);
  
    memcpy(message, &sharedMessage, sizeof(MESSAGE));
    LeaveCriticalSection(&messageCriticalSection);
}

void SendDataToArduino(HANDLE hSerial, const char* data) {

    DWORD bytesWritten;

    if (!WriteFile(hSerial, data, (DWORD)strlen(data), &bytesWritten, NULL)) {
        fprintf(stderr, "Error writing to serial port\n");
    }else{
        printf("SENT MESSAGE: %s\n", data);
        
        if (!FlushFileBuffers(hSerial)) {
            fprintf(stderr, "Error flushing file buffers. Error code: %lu\n", GetLastError());
        }

    }
}


DWORD WINAPI ListenThreadFunction(LPVOID lpParam) {
    HANDLE hSerial = (HANDLE)lpParam;
    MESSAGE latestMessage;

    while (1) {
        
        
        
            if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) {
                break; 
            }

            GetLatestTag(&latestMessage);
    
            if(sharedMessage.openDoor){
                
                const char* doorMessage = "[";

                SendDataToArduino(hSerial, doorMessage);

                sharedMessage.openDoor = false;

            }

                HandleArduinoEvent(hSerial);
        
    }

    return 0;
}


HANDLE StartListenThread(HANDLE hSerial) {
    hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    return CreateThread(NULL, 0, ListenThreadFunction, hSerial, 0, NULL);
}


void StopListenThread(HANDLE hThread) {
    SetEvent(hExitEvent);
    WaitForSingleObject(hThread, INFINITE);
    CloseHandle(hThread);
    CloseHandle(hExitEvent);
}

void CleanupResources() {
    
    if (hListenThread != NULL) {
        StopListenThread(hListenThread);
        hListenThread = NULL;
    }

    DeleteCriticalSection(&messageCriticalSection);

    CloseHandle(hSerial);

    printf("THREAD/PORT TERMINATED");

}

void ProcessReceivedPass(const char* pass) {

    TAG tag;

    strcpy(tag.pass, pass);

    printf("PRP: %s\n", tag.pass);

    int dir = 3;
    int answer;

    answer = arrayHandler(&tag, 2, &dir);
    printf("answer: %d",answer);
    if (answer == 1) {
        const char* greenLight = "{";
        SendDataToArduino(hSerial, greenLight);
    } else if(answer == 0){
        const char* redLight = "}";
        SendDataToArduino(hSerial, redLight);
    }else if(answer == 2){
        // just somthing to catch no match.
    }
}

int InitializeAndStartListenThread() {
    
    InitializeCriticalSection(&messageCriticalSection);

    printf("initiating thread...\n");

    hSerial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
 
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

   
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM3 port\n");
        CleanupResources(); // Clean up on error
        return 1;
    }

 
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting serial port state\n");
        CloseHandle(hSerial);
        return 1;
    }

    
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        fprintf(stderr, "Error setting timeouts\n");
        CloseHandle(hSerial);
        return 1;
    }        

    hListenThread = StartListenThread(hSerial);
    printf("ALL INITIATED!\n");
    return 0;
}

void openDoor(){

    EnterCriticalSection(&messageCriticalSection);
    sharedMessage.openDoor = true;
    LeaveCriticalSection(&messageCriticalSection);

}