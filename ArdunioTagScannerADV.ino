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

    byte sector2 = 1;
    byte blockAddr2 = 4;
    byte dataBlock2[16]; 
    byte trailerBlock2 = 7;
    byte buffer2[18];
    byte size2 = sizeof(buffer2); 
    byte idSent = 0;

  digitalWrite(GREEN_LED_PIN, HIGH);
        delay(50);
        digitalWrite(GREEN_LED_PIN, LOW);
        digitalWrite(GREEN_LED_PIN, HIGH);
        delay(50);
        digitalWrite(GREEN_LED_PIN, LOW);

    
while(1){

  if (mfrc522.PICC_IsNewCardPresent()){

    if (mfrc522.PICC_ReadCardSerial()){

      
          
          status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock2, &key, &(mfrc522.uid));
          if (status != MFRC522::STATUS_OK) {
              //Serial.print(F("PCD_Authenticate() failed: "));
              //Serial.println(mfrc522.GetStatusCodeName(status));
              return;
          }

          if(idSent == 0){
            for (byte i = 0; i < mfrc522.uid.size; i++) {
                char hexChar[3];
                sprintf(hexChar, "%02X", mfrc522.uid.uidByte[i]);
                Serial.write(hexChar);
                if (i < mfrc522.uid.size - 1) {
                    Serial.write(" ");
                }
            }
            idSent = 1;
          }
          
          while(1){
            if (Serial.available() >= 16) {
                    Serial.readBytes(dataBlock2, sizeof(dataBlock2));  
                    break;
            }  
          }
          
          status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock2, &key, &(mfrc522.uid));
          if (status != MFRC522::STATUS_OK) {
              //Serial.print(F("PCD_Authenticate() failed: "));
              //Serial.println(mfrc522.GetStatusCodeName(status));
              return;
          }
              
          status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(4, dataBlock2, 16); // write pass bytes to tag
          if (status != MFRC522::STATUS_OK) {
              //Serial.print(F("MIFARE_Write() failed: "));
              //Serial.println(mfrc522.GetStatusCodeName(status));
          }
         
          status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(4, buffer2, &size2); // Reads the written pass bytes
          if (status != MFRC522::STATUS_OK) {
              //Serial.print(F("MIFARE_Read() failed: "));
              //Serial.println(mfrc522.GetStatusCodeName(status));
              return;
          }

          byte count = 0;
          for (byte i = 0; i < 16; i++) {
              
            if (dataBlock2[i] == buffer2[i]){

              count++;
            } 
          }

          if (count == 16) {
            Serial.println("SUCCESS");
          }
          
          mfrc522.PICC_HaltA();

          mfrc522.PCD_StopCrypto1();

          break;
      }
    }
  }
}


void remoteOpen() {
    digitalWrite(GREEN_LED_PIN, HIGH); // Turn on the LED (HIGH)
    delay(3000);  // Wait for 1 second
    digitalWrite(GREEN_LED_PIN, LOW);
    return;
}


void loop() {
    if (Serial.available() > 0) {
        String message = Serial.readString();

        if (message.startsWith("ADDTAG")) {

            addTag();

        } else if (message.startsWith("REMOTEOPEN")) {

            remoteOpen();

        } else if(message.startsWith("GRANTED")){

            digitalWrite(GREEN_LED_PIN, HIGH); 
            delay(3000);  
            digitalWrite(GREEN_LED_PIN, LOW);
            
        } else if(message.startsWith("DENIED")){

            digitalWrite(RED_LED_PIN, HIGH); 
            delay(3000);  
            digitalWrite(RED_LED_PIN, LOW);

        }
    }
  
    byte sector = 1;
    byte blockAddr = 4;
    byte dataBlock[16]; 
    byte trailerBlock = 7;
    byte buffer[18];
    byte size = sizeof(buffer);
    
    // Read Mode
    if (!mfrc522.PICC_IsNewCardPresent())
    return;

    if (!mfrc522.PICC_ReadCardSerial())
        return;

      // Authenticate using key A
      status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("PCD_Authenticate() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return;
      }

      status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size); // Read pass from tag
      if (status != MFRC522::STATUS_OK) {
          //Serial.print(F("MIFARE_Read() failed: "));
          //Serial.println(mfrc522.GetStatusCodeName(status));
          return;

      }else{
 
            Serial.write("A:", 2);
            Serial.write(buffer, sizeof(buffer)); // send Pass bytes add prefix A:

      }
      
      mfrc522.PICC_HaltA();

      mfrc522.PCD_StopCrypto1();
   
}