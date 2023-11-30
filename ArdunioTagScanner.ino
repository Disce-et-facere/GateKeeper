#include <SPI.h>
#include <MFRC522.h>
#include <String.h>

#define RST_PIN         9
#define SS_PIN          10

MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

#define GREEN_LED_PIN   8
#define RED_LED_PIN     7

MFRC522::StatusCode status;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    SPI.begin();
    mfrc522.PCD_Init();

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);

    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(500);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    delay(500);
    digitalWrite(GREEN_LED_PIN, LOW);
    
}

void addTag() {

    digitalWrite(RED_LED_PIN, HIGH);
    delay(100);
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(RED_LED_PIN, HIGH);
    delay(100);
    digitalWrite(RED_LED_PIN, LOW);

    if (mfrc522.PICC_IsNewCardPresent()) {

      status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }

      String tagIdString;

      for (byte i = 0; i < mfrc522.uid.size; i++) {
          // send tag ID
          tagIdString += String(mfrc522.uid.uidByte[i], HEX);
      }

      Serial.println(tagIdString);

      while(1){
        if (Serial.available() == 16) {
        Serial.readBytes(dataBlock, sizeof(dataBlock));
        break;
        }
      }

      status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, 1, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }

      status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(4, dataBlock, 16); // write pass bytes to tag
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Write() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
      }

      status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(4, dataBlock, &size); // Reads the written pass bytes
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("MIFARE_Read() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
      }

      byte count = 0;
      for (byte i = 0; i < 16; i++) {
          
          if (dataBlock[i] == dataBlock[i])
              count++;
      }

      if (count == 16) {
        Serial.println("SUCCESS");
      }
       
      mfrc522.PICC_HaltA();

      mfrc522.PCD_StopCrypto1();

    }

}


void remoteOpen() {
    digitalWrite(GREEN_LED_PIN, HIGH); // Turn on the LED (HIGH)
    delay(10000);  // Wait for 1 second
    digitalWrite(GREEN_LED_PIN, LOW);
    return;
}


void loop() {
    if (Serial.available() > 0) {
        String message = Serial.readString();

        if (message.startsWith("ADD_TAG")) {
            addTag();
        } else if (message.startsWith("REMOTE_OPEN")) {
            remoteOpen();
        } else {
            Serial.println("Unknown Message...");
        }
    }

    byte sector = 1;
    byte blockAddr = 4;
    byte dataBlock[16]; 
    byte trailerBlock = 7;
    byte buffer[18];
    byte size = sizeof(buffer);
    static int init = 0;
    if(init == 0){
      Serial.println(F("Normal mode: Scan tag to open door."));
      Serial.println();
      init++;
    }
    
    // Read Mode
   if (mfrc522.PICC_IsNewCardPresent()) {

      // Authenticate using key A
      Serial.println(F("Authenticating again using key B..."));
      status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          Serial.print(F("PCD_Authenticate() failed: "));
          Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }

      status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size); // Read pass from tag
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("MIFARE_Read() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          digitalWrite(RED_LED_PIN, HIGH); 
          delay(100);  
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH); 
          delay(100);  
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH); 
          delay(100);  
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH); 
          delay(100);  
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(RED_LED_PIN, HIGH); 
          delay(100);  
          digitalWrite(RED_LED_PIN, LOW);
      }


      Serial.write(buffer, sizeof(buffer)); // send Pass bytes

      while(1){
        if (Serial.available() == 16) {
        Serial.readBytes(dataBlock, sizeof(dataBlock));
        break;
        }
      }

      while(Serial.available() == 0){

        if (Serial.available() == 4) {
          
          String msg = Serial.readString();

          if (msg.startsWith("YES")) {
              digitalWrite(GREEN_LED_PIN, HIGH); 
              delay(3000);  
              digitalWrite(GREEN_LED_PIN, LOW);
          } else if (msg.startsWith("NOT")) {
              digitalWrite(RED_LED_PIN, HIGH); 
              delay(3000);  
              digitalWrite(RED_LED_PIN, LOW);
          }
        }
      }
      

      mfrc522.PICC_HaltA();

      mfrc522.PCD_StopCrypto1();
   }
}