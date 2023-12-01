#include "arduinoHandler.h"


// Globals
volatile MESSAGE sharedMessage;
CRITICAL_SECTION messageCriticalSection;
HANDLE hListenThread = NULL;
HANDLE hSerial;
HANDLE hExitEvent;
enum State currentState = SEND_PASS;

// Function for handling Arduino events and update data
void HandleArduinoEvent(HANDLE hSerial) {
    DWORD bytesRead;
    char buffer[MAX_BUFFER_SIZE];

     // Read tag pass
    if (ReadFile(hSerial, buffer, 32, &bytesRead, NULL) && bytesRead == 32) {
        // Null-terminate the received data
        buffer[bytesRead] = '\0';
        printf("NAPPAR");
        // Check if the received data starts and ends with the expected prefix and suffix
        const char* prefix = "TAGPASS:";
        const char* suffix = ":TAGPASS";
        size_t prefixLength = strlen(prefix);
        size_t suffixLength = strlen(suffix);

        if (strncmp(buffer, prefix, prefixLength) == 0 &&
            strncmp(buffer + bytesRead - suffixLength, suffix, suffixLength) == 0) {

            // Extract the password from the received data
            char formattedPass[17];
            strncpy(formattedPass, buffer + prefixLength, 16);
            formattedPass[16] = '\0';

            // Process the received password
            ProcessReceivedPass(formattedPass);

            // Update shared message
            EnterCriticalSection(&messageCriticalSection);
            strncpy((char*)sharedMessage.pass, formattedPass, sizeof(sharedMessage.pass));
            sharedMessage.pass[sizeof(sharedMessage.pass) - 1] = '\0';
            LeaveCriticalSection(&messageCriticalSection);
        } else {
            
            printf("Invalid message format\n");
            currentState = ERROR_STATE;
        }
    }
   
    // Read tag ID
   else if (ReadFile(hSerial, buffer, 8, &bytesRead, NULL) && bytesRead == 8) {
            buffer[8] = '\0'; // Null-terminate at the "#" position
            EnterCriticalSection(&messageCriticalSection);
            strncpy((char*)sharedMessage.id, buffer, sizeof(sharedMessage.id));
            sharedMessage.id[sizeof(sharedMessage.id) - 1] = '\0';
            LeaveCriticalSection(&messageCriticalSection);
            printf("ID: %s\n", buffer);
            currentState = SET_ID;
    }

    // Read success message
    else if (ReadFile(hSerial, buffer, 7, &bytesRead, NULL) && bytesRead == 7) {
        buffer[7] = '\0';
        printf("Buffer 3: %s\n", buffer);
        EnterCriticalSection(&messageCriticalSection);
        strncpy((char*)sharedMessage.success, buffer, sizeof(sharedMessage.success));
        sharedMessage.success[sizeof(sharedMessage.success) - 1] = '\0';
        LeaveCriticalSection(&messageCriticalSection);
        //currentState = RECEIVE_OK;
    }
    //printf("Still Running...\n");
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
        //printf("Sent message: %s\n", data);

        if (!FlushFileBuffers(hSerial)) {
    fprintf(stderr, "Error flushing file buffers. Error code: %lu\n", GetLastError());
}

    }
}

// Function to listen for Arduino events in a separate thread
DWORD WINAPI ListenThreadFunction(LPVOID lpParam) {
    HANDLE hSerial = (HANDLE)lpParam;
    DWORD dwEventMask;

    while (1) {

        //printf("LooP init..\n");
        if (WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) {
            break; 
        }
            HandleArduinoEvent(hSerial);
        /*if (WaitCommEvent(hSerial, &dwEventMask, NULL)) {
            if (dwEventMask & EV_RXCHAR) {
        
                

            }
            
        } else {
            fprintf(stderr, "Error waiting for communication event\n");
        }*/
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

void ProcessReceivedPass(const char* pass) {

    MESSAGE latestMessage;
    TAG tag;

    hexStringToCharArray(pass, latestMessage.pass);

    int dir = 3;
    if (arrayHandler(&tag, 2, &dir)) {
        const char* greenLight = "GRANTED";
        SendDataToArduino(hSerial, greenLight);
    } else {
        const char* redLight = "DENIED";
        SendDataToArduino(hSerial, redLight);
    }
}

int InitializeAndStartListenThread() {
    // Initialize the critical section for shared data
    InitializeCriticalSection(&messageCriticalSection);
    printf("initiating thread...\n");
    // Open a handle to the COM3 port
    hSerial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    EscapeCommFunction(hSerial, CLRRTS | SETDTR);
    Sleep(100);  // Wait for Arduino to reset
    EscapeCommFunction(hSerial, SETRTS | SETDTR);
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

int readAndWriteTag(int option, char pass[17], char* receivedId) {

    if (hListenThread == NULL) {
        int initResult = InitializeAndStartListenThread();
        if (initResult != 0) {
            return initResult; // Initialization failed
        }
    }

    MESSAGE latestMessage;

    if(option == 1){

        while(1){


            if(currentState == SEND_PASS){
                Sleep(3000);                //char tempPass[18];
              
                //snprintf(tempPass, sizeof(tempPass), "%s#", pass);
                SendDataToArduino(hSerial, pass);
                //printf("PASS SENT: %s\n",pass);

                currentState = IDLE_STATE;

            }else if(currentState == SET_ID){

                GetLatestTag(&latestMessage);

                strncpy(receivedId, latestMessage.id, sizeof(latestMessage.id));
                receivedId[sizeof(receivedId) - 1] = '\0';
                printf("ID SET! %s\n", latestMessage.id);
                break;

            }else if(currentState == IDLE_STATE){

                

            }else if(currentState == START_UP){

                break;

            }
        }
    }

    else if(option == 2){

        const char* doorMessage = "REMOTEOPEN";

        SendDataToArduino(hSerial, doorMessage);

    }

    else if(option == 3){

         CleanupResources();

    }

    else if(option == 4){
        // start up purpose without mixing with state
        return;
    }

    return 0;
}