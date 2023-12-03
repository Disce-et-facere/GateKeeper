#include "arduinoHandler.h"


// Globals
volatile MESSAGE sharedMessage;
CRITICAL_SECTION messageCriticalSection;
HANDLE hListenThread = NULL;
HANDLE hSerial;
HANDLE hExitEvent;
enum State currentState = START_UP;


// Function for handling Arduino events and update data
void HandleArduinoEvent(HANDLE hSerial) {
    DWORD bytesRead;
    char buffer[MAX_BUFFER_SIZE];


    const char* tagPassPrefix = "TAGPASS:";
    const char* tagPassSuffix = ":TAGPASS";
    const char* tagIDPrefix = "ID:";
    const char* tagIDSuffix = ":ID";
    size_t tagPassPrefixLength = strlen(tagPassPrefix);
    size_t tagPassSuffixLength = strlen(tagPassSuffix);
    size_t tagIDPrefixLength = strlen(tagIDPrefix);
    size_t tagIDSuffixLength = strlen(tagIDSuffix);
    

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

        else if (strncmp(buffer, tagIDPrefix, tagIDPrefixLength) == 0 &&
            strncmp(buffer + bytesRead - tagIDSuffixLength, tagIDSuffix, tagIDSuffixLength) == 0) {

            char formattedID[9];
            strncpy(formattedID, buffer + tagIDPrefixLength, 8);
            formattedID[8] = '\0';

            
            EnterCriticalSection(&messageCriticalSection);
            strncpy((char*)sharedMessage.id, formattedID, sizeof(sharedMessage.id));
            sharedMessage.id[sizeof(sharedMessage.id) - 1] = '\0';
            LeaveCriticalSection(&messageCriticalSection);

            currentState = SET_ID;
            PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
        }
        
    } else {
        
        printf("Error reading from serial port. Error code: %d\n", GetLastError());
    }
}



// Function to get the latest shared data from the main thread
void GetLatestTag(MESSAGE* message) {
    EnterCriticalSection(&messageCriticalSection);
    // Copy the shared data to the provided TAG struct
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

// Function to listen for Arduino events in a separate thread
DWORD WINAPI ListenThreadFunction(LPVOID lpParam) {
    HANDLE hSerial = (HANDLE)lpParam;
    //DWORD dwEventMask;

    while (1) {

        //printf("LooP init..\n");
        
        if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) {
            break; 
        }
            HandleArduinoEvent(hSerial);
    }

    return 0;
}

// Function to start the Arduino event listening thread
HANDLE StartListenThread(HANDLE hSerial) {
    hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    return CreateThread(NULL, 0, ListenThreadFunction, hSerial, 0, NULL);
}

// Function to stop the Arduino event listening thread
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

    int dir = 3;
    if (arrayHandler(&tag, 2, &dir)) {
        const char* greenLight = "{";
        SendDataToArduino(hSerial, greenLight);
    } else {
        const char* redLight = "}";
        SendDataToArduino(hSerial, redLight);
    }
}

int InitializeAndStartListenThread() {
    
    InitializeCriticalSection(&messageCriticalSection);

    printf("initiating thread...\n");

    hSerial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    /*
    EscapeCommFunction(hSerial, CLRRTS | SETDTR);
    Sleep(1000);
    EscapeCommFunction(hSerial, SETRTS | SETDTR);
    */
    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);

    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    // Check for errors in opening the serial port
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM3 port\n");
        CleanupResources(); // Clean up on error
        noScanner();
        exit(1);
        return 1;
    }

    // Set serial port parameters
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

    // Set timeouts
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

int ardHandler(char pass[17], char* receivedId) {

    if (hListenThread == NULL) {
        int initResult = InitializeAndStartListenThread();
        if (initResult != 0) {
            return initResult; // Initialization failed
        }
    }

    MESSAGE latestMessage;

    currentState = SEND_PASS;

        while(1){

            if(currentState == SEND_PASS){

                char tempPass[17];
                strcpy(tempPass, pass);
                char prepPass[20];
                strcpy(prepPass, ":");
                strcat(prepPass, tempPass);
                strcat(prepPass, ";");
              
                SendDataToArduino(hSerial, prepPass);

            }else if(currentState == SET_ID){

                GetLatestTag(&latestMessage);
                strncpy(receivedId, latestMessage.id, sizeof(latestMessage.id));
                receivedId[9] = '\0';
                break;
                
            }else if(currentState == START_UP){

                 break;

            }else if(currentState == ERROR_STATE){
                
            }
        
        }

    return 0;
}

void openDoor(){

    const char* doorMessage = "[";

    SendDataToArduino(hSerial, doorMessage);

}