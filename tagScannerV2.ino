#include <SPI.h>
#include <MFRC522.h>
#include <arduino.h>
#include <string.h>

#define RST_PIN         9
#define SS_PIN          10
#define GREEN_LED_PIN   8
#define RED_LED_PIN     7
#define YELLOW_LED_PIN  4
#define DOOR '['
#define GRANTED '{'
#define DENIED '}'

MFRC522 mfrc522(SS_PIN, RST_PIN);

byte blockAddr = 4;
byte sector = 1;
byte trailerBlock = 7;
const char prefix = ':';
const char suffix = ';';
const byte passwordLength = 16;
const int maxBufferSize = 256;
byte buffer[100];
byte serialBuffer[maxBufferSize];
byte pass[passwordLength];
byte size = sizeof(buffer);
byte passIndex = 0;
byte serialBufferIndex = 0;
bool passAvailable = false;
bool prefixDetected = false;
bool suffixDetected = false;



MFRC522::StatusCode status;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);

  memset(pass, 0, sizeof(pass));
}

bool isPassEmpty() {
  for (int i = 0; i < sizeof(pass); ++i) {
    if (pass[i] != 0) {
      return false; // Array is not empty
    }
  }
  return true; // Array is empty
}

bool startsWith(const byte *buffer, const char *prefix, size_t length) {
  return memcmp(buffer, prefix, length) == 0;
}

void resetPass() {
  memset(pass, 0, sizeof(pass));
}

void loop() {

  byte access[3];

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
  
 while (Serial.available() > 0 && serialBufferIndex < maxBufferSize) {
    serialBuffer[serialBufferIndex++] = Serial.read();
  }

  
  for (int i = 0; i < serialBufferIndex; ++i) {
    char currentChar = serialBuffer[i];

    switch (currentChar) {
      case prefix:
        
        passIndex = 0;
        prefixDetected = true;
        break;

      case suffix:
        
        if (passIndex > 0 && passIndex == 16) {
          suffixDetected = true;
          digitalWrite(YELLOW_LED_PIN, HIGH);
        }
        break;

      case DOOR:
          digitalWrite(GREEN_LED_PIN, HIGH);
          delay(3000);
          digitalWrite(GREEN_LED_PIN, LOW);
          break;
      case GRANTED:
          digitalWrite(GREEN_LED_PIN, HIGH);
          delay(3000);
          digitalWrite(GREEN_LED_PIN, LOW);
          break;
      case DENIED:
          digitalWrite(RED_LED_PIN, HIGH);
          delay(3000);
          digitalWrite(RED_LED_PIN, LOW);
          break;
      default:
        
        if (passIndex < 16) {
          pass[passIndex++] = currentChar;
        }
    }
  }

  // Set passAvailable only if both prefix and suffix are detected
  if (prefixDetected && suffixDetected) {
    passAvailable = true;
    // Reset the flags for the next password
    prefixDetected = false;
    suffixDetected = false;
  }

  // Reset passIndex only if a complete password has been processed
  if (passAvailable) {
    passIndex = 0;
  }

  // Reset serialBufferIndex after processing the buffer
  serialBufferIndex = 0;


  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  
  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  if (!passAvailable) {
  
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    } else {
      status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
      } else {
        Serial.print("TAGPASS:");
        for (byte i = 0; i < 16; i++) {
          Serial.print((char)buffer[i]);
        }
        Serial.print(":TAGPASS");

        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD

        return;
      }
    }
  } else {

    digitalWrite(YELLOW_LED_PIN, LOW);
    
    status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
      Serial.print(F("PCD_Authenticate() failed: "));
      Serial.println(mfrc522.GetStatusCodeName(status));
    } else {

      status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, pass, 16);
      if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Write() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
      } else {
        
        Serial.print("ID:");
        for (byte i = 0; i < mfrc522.uid.size; i++) {
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        }
        Serial.print(":ID");
        Serial.flush();
        resetPass();
        passAvailable = false;
  
        mfrc522.PICC_HaltA(); // Halt PICC
        mfrc522.PCD_StopCrypto1(); // Stop encryption on PCD

        return;
      }
    }
  }
}
