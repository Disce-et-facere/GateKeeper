#include"arduinoHandler.h"


int readAndWriteTag(int option, char pass[17], unsigned char *receivedId, size_t idSize) {

    int mode = 1;

    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[MAX_BUFFER_SIZE];

    // Open a handle to the COM3 port
    hSerial = CreateFile(("COM3"), GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM3 port\n");
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

    if(option == 1){ // registrate new tag
                     // 1. send ADD_TAG 
                     // 2. recieve Tag id
                     // 3 send id to GUI
                     // 4. send pass as bytes
                     // 5. recieve int as byte 1 or 0;
                     // 6. send int to GUI
    mode = 0;

    int initMsg = 0;
    int recID = 0;
    int passSent = 0;
    int aOk = 0;

        while(1){
            
            const char* initMessage = "ADD_TAG";

            if(initMsg == 0){
                DWORD bytesWritten;
                if (!WriteFile(hSerial, initMessage, strlen(initMessage), &bytesWritten, NULL)) {
                    fprintf(stderr, "Error writing to serial port\n");
                    break;
                } else {
                    printf("Sent message: %s\n", initMessage);
                    initMsg++;
                }
            }

            if(recID == 0){
                DWORD bytesRead;
                if (ReadFile(hSerial, receivedId, idSize, &bytesRead, NULL)) {
                   if (bytesRead > 0) {
                        printf("Received ID in hex: ");
                        for (size_t i = 0; i < bytesRead; i++) {
                            printf("0x%02X ", receivedId[i]);
                            recID++;
                        }
                        printf("\n");
                    
                    } else {
                        fprintf(stderr, "Error reading from serial port\n");

                    }   
                }

            }

            if(passSent == 0){
                DWORD bytesWritten;
                if (!WriteFile(hSerial, pass, strlen(pass), &bytesWritten, NULL)) {
                    fprintf(stderr, "Error writing to serial port\n");
                    break;
                } else {
                    printf("Sent message: %s\n", pass);
                    passSent++;
                }
            }

            if(aOk == 0){
                int response;
                DWORD bytesRead;
                if (ReadFile(hSerial, &response, sizeof(response), &bytesRead, NULL)) {
                   if (bytesRead == sizeof(response)) {
                        if (response == 1) {
                            printf("Success :-) received from Arduino\n");
                            return 1;
                            break;
                        } else if (response == 0) {
                            printf("Failure :-( received from Arduino\n");
                            break;
                            return 0;
                        } else {
                            printf("Unexpected response received from Arduino: %d\n", response);
                            break;
                        }
                    } else {
                        fprintf(stderr, "Incomplete data read\n");
                        break;
                    }
                } else {
                    fprintf(stderr, "Error reading from serial port\n");
                    break;
                }

            }


        }
        mode = 1;
    }else if(option == 2){  // Remote Open Door

        mode = 0;

        const char* doorMessage = "REMOTE_OPEN";

                DWORD bytesWritten;
                if (!WriteFile(hSerial, doorMessage, strlen(doorMessage), &bytesWritten, NULL)) {
                    fprintf(stderr, "Error writing to serial port\n");
                } else {
                    printf("Sent message: %s\n", doorMessage);
                }

        mode = 1;

    }else if(option == 3){  // close Serial one exit

        mode = 0;
        CloseHandle(hSerial);

    }

    while(mode){
    // send pass to arrayHandler(); if return is 1 send 1 to arduino and if 0, the same
        char tempPass[17];
        byte receivedPass[16];
        TAG tag;
        DWORD bytesRead;

        if (ReadFile(hSerial, receivedPass, sizeof(receivedPass), &bytesRead, NULL)) {
            if (bytesRead == sizeof(receivedPass)) {
            
                // Convert the received bytes to a string
                for (size_t i = 0; i < 17; i++) {
                    tempPass[i] = (char)receivedPass[i];
                }
                tempPass[sizeof(tempPass) - 1] = '\0';

                printf("Received pass: %s\n", tempPass);
            } else {
                fprintf(stderr, "Incomplete data read\n");
            }
        } else {
            fprintf(stderr, "Error reading from serial port\n");
        }
        strncpy(tag.pass, tempPass, sizeof(tag.pass) - 1);
        tag.pass[sizeof(tag.pass) - 1] = '\0';

        if(arrayHandler(&tag, 8, 3)){

        }

        
    }

    return 0;
}