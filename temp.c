#include <Windows.h>
#include <stdio.h>
#include <string.h>

#define MAX_BUFFER_SIZE 256

// Define your TAG struct
typedef struct {
    char id[12];
    char pass[17];
    char success[7];
    // Add other fields if needed
} MESSAGE;

// Global variables for shared data
volatile MESSAGE sharedMessage;
CRITICAL_SECTION messageCriticalSection;
HANDLE hListenThread = NULL;
HANDLE hSerial;
HANDLE hExitEvent;

// Function to handle Arduino events and update shared data
void HandleArduinoEvent(HANDLE hSerial) {
    DWORD bytesRead;
    char buffer[MAX_BUFFER_SIZE];

    // Read tag ID
    if (ReadFile(hSerial, buffer, 11, &bytesRead, NULL) && bytesRead == 11) {
        buffer[11] = '\0';
        EnterCriticalSection(&messageCriticalSection);
        strncpy(sharedMessage.id, buffer, sizeof(sharedMessage.id));
        sharedMessage.id[sizeof(sharedMessage.id) - 1] = '\0';
        LeaveCriticalSection(&messageCriticalSection);
    }

    // Read tag pass
    if (ReadFile(hSerial, buffer, 50, &bytesRead, NULL) && bytesRead == 50) {
        buffer[50] = '\0';
        const char* prefix = "A:";
        char* passStart = strstr(buffer, prefix);
        char* formatedPass[17];

        if (passStart != NULL) {
        
            passStart += strlen(prefix);

            hexStringToCharArray(passStart, formatedPass);
            
        } else {
            
        }
        EnterCriticalSection(&messageCriticalSection);
        strncpy(sharedMessage.pass, formatedPass, sizeof(sharedMessage.pass));
        sharedMessage.pass[sizeof(sharedMessage.pass) - 1] = '\0';
        LeaveCriticalSection(&messageCriticalSection);
    }

    // Read success message
    if (ReadFile(hSerial, buffer, 7, &bytesRead, NULL) && bytesRead == 7) {
        buffer[6] = '\0';
        EnterCriticalSection(&messageCriticalSection);
        strncpy(sharedMessage.success, buffer, sizeof(sharedMessage.success));
        sharedMessage.success[sizeof(sharedMessage.success) - 1] = '\0';
        LeaveCriticalSection(&messageCriticalSection);
    }
}

// Function to get the latest shared data from the main thread
void GetLatestTag(MESSAGE* message) {
    EnterCriticalSection(&messageCriticalSection);
    // Copy the shared data to the provided TAG struct
    *message = sharedMessage;
    LeaveCriticalSection(&messageCriticalSection);
}

void SendDataToArduino(HANDLE hSerial, const char* data) {
    DWORD bytesWritten;
    if (!WriteFile(hSerial, data, strlen(data), &bytesWritten, NULL)) {
        fprintf(stderr, "Error writing to serial port\n");
    }
}

// Function to listen for Arduino events in a separate thread
DWORD WINAPI ListenThreadFunction(LPVOID lpParam) {
    HANDLE hSerial = (HANDLE)lpParam;
    DWORD dwEventMask;

    while (1) {

        
        if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) {
            break; 
        }

        if (WaitCommEvent(hSerial, &dwEventMask, NULL)) {
            if (dwEventMask & EV_RXCHAR) {
        
                HandleArduinoEvent(hSerial);

            }
            
        } else {
            fprintf(stderr, "Error waiting for communication event\n");
        }
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

}

int InitializeAndStartListenThread() {
    // Initialize the critical section for shared data
    InitializeCriticalSection(&messageCriticalSection);

    // Open a handle to the COM3 port
    hSerial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };

    // Check for errors in opening the serial port
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM3 port\n");
        CleanupResources(); // Clean up on error
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

    return 0;
}

void hexStringToCharArray(const char* hexString, char* charArray) {
    size_t len = strlen(hexString);
    size_t charArrayIndex = 0;

    for (size_t i = 0; i < len; i += 2) {
        // Convert each pair of hexadecimal characters to a byte
        sscanf(&hexString[i], "%2hhX", &charArray[charArrayIndex]);
        charArrayIndex++;
    }

    // Null-terminate the char array
    charArray[charArrayIndex] = '\0';
}

int readAndWriteTag(int option, char pass[17], char* receivedId, int* check) {

    if (hListenThread == NULL) {
        int initResult = InitializeAndStartListenThread();
        if (initResult != 0) {
            return initResult; // Initialization failed
        }
    }

    MESSAGE latestMessage;
    TAG tag;

    if(option == 1){

        const char* newTagMessage = "ADDTAG";

        SendDataToArduino(hSerial, newTagMessage);

        GetLatestTag(&latestMessage);

        strncpy(receivedId, latestMessage.id, sizeof(latestMessage.id));
        receivedId[sizeof(latestMessage.id)] = '\0';

        unsigned char passBytes[16];
        for (int i = 0; i < 16; ++i) {
        passBytes[i] = (unsigned char)pass[i];
        }

        SendDataToArduino(hSerial, passBytes);

        GetLatestTag(&latestMessage);

        if (strcmp(latestMessage.success, "SUCCESS") == 0) {
            *check = 1;
        }else{
            *check = 0;
        }
    }

    else if(option == 2){

        const char* doorMessage = "REMOTEOPEN";

        SendDataToArduino(hSerial, doorMessage);

    }

    else if(option == 3){

         CleanupResources();


    }


    while (1) {
        // Your existing logic

        // Check for new data from the second thread
       
        GetLatestTag(&latestMessage);

        strcpy(tag.pass, latestMessage.pass);

        if(arrayHandler(&tag, 2, 3)){

                const char* greenLight = "YES";
                
                SendDataToArduino(hSerial, greenLight);

            }else{

                const char* redLight = "NOT";

                SendDataToArduino(hSerial, redLight);
            }

        Sleep(10); 
    }

    return 0;
}
