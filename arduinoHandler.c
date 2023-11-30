#include"arduinoHandler.h"


int readAndWriteTag(int option, char pass[17], char* receivedId, size_t idSize) {

    static int mode = 1;
    static int portOpen = 0;

    HANDLE hSerial;
    DCB dcbSerialParams = { 0 };
    COMMTIMEOUTS timeouts = { 0 };
    char buffer[MAX_BUFFER_SIZE];

    if(portOpen == 0){

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

        portOpen++;
    }

    if(option == 1){

        mode = 0;

        int initMsg = 0;

        DWORD bytesRead;
        DWORD bytesWritten;

        if(initMsg == 0){

            const char* initMessage = "ADD_TAG";

            if (!WriteFile(hSerial, initMessage, strlen(initMessage), &bytesWritten, NULL)) {
                fprintf(stderr, "Error writing to serial port\n");
            } else {
                printf("Sent message: %s\n", initMessage);
                initMsg++;
            }

        }

        while(1){

            if (ReadFile(hSerial, receivedId, idSize, &bytesRead, NULL)) {

                if (bytesRead == 11) {
                    receivedId[bytesRead] = '\0'; 
                    printf("Received ID as string: %s\n", receivedId);
                    break;
                } else {
                    fprintf(stderr, "Receiving ID Error, received: %s\n", receivedId);
                }
            }
        }

        unsigned char passBytes[16];
        for (int i = 0; i < 16; ++i) {
        passBytes[i] = (unsigned char)pass[i];
        }
        
        if (!WriteFile(hSerial, passBytes, sizeof(passBytes), &bytesWritten, NULL)) {
            fprintf(stderr, "Error writing Pass to serial port\n");
        } else {
            printf("Sent message: ");
            for (int i = 0; i < sizeof(passBytes); ++i) {
                printf("%02X ", passBytes[i]);
            }
            printf("\n");
        }

        while(1){
            
            char success[8];

            if (ReadFile(hSerial, receivedId, idSize, &bytesRead, NULL)) {

                if (bytesRead == 7) {
                    success[sizeof(success) - 1 ] = '\0'; 
                    printf("Received Success as string: %s\n", receivedId);
                    return 1;
                    break;
                } else {
                    fprintf(stderr, "Receiving Success Error, received: %s\n", receivedId);
                    return 0;
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

    char tempPass[17];
    byte receivedPass[16];
    TAG tag;

    while(mode){
    // send pass to arrayHandler(); if return is 1 send 1 to arduino and if 0, the same
       
        DWORD bytesRead;
        DWORD bytesWritten;

        if (ReadFile(hSerial, receivedPass, sizeof(receivedPass), &bytesRead, NULL)) {
            if (bytesRead == 16) {

                // Convert the received bytes to a string
                for (size_t i = 0; i < bytesRead; i++) {
                    tempPass[i] = (char)receivedPass[i];
                }
                tempPass[sizeof(tempPass) - 1] = '\0';

                printf("Received pass: %s\n", tempPass);
            } else {
                fprintf(stderr, "Incomplete data read, normal mode\n");
            }
        } else {
            fprintf(stderr, "Error reading from serial port\n");
        }
        strncpy(tag.pass, tempPass, sizeof(tag.pass) - 1);
        tag.pass[sizeof(tag.pass) - 1] = '\0';

        if(arrayHandler(&tag, 2, 3)){

            const char* greenLight = "YES";
            
            if (!WriteFile(hSerial, greenLight, strlen(greenLight), &bytesWritten, NULL)) {

                fprintf(stderr, "Error writing to serial port\n");

            }

        }else{

            const char* redLight = "NOT";

            if (!WriteFile(hSerial, redLight, strlen(redLight), &bytesWritten, NULL)) {
                fprintf(stderr, "Error writing to serial port\n");
            }
        }
        
    }

    return 3;
}