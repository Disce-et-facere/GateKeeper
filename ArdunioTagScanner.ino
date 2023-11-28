#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9 
#define SS_PIN          10

MFRC522 mfrc522(SS_PIN, RST_PIN);   

MFRC522::MIFARE_Key key;

#define GREEN_LED_PIN 3
#define RED_LED_PIN 4

void setup() {
    Serial.begin(9600);
    while (!Serial);    
    SPI.begin();        
    mfrc522.PCD_Init(); 

    for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("Scan a MIFARE Classic PICC to demonstrate read and write."));
    Serial.print(F("Using key (for A and B):"));
    dump_byte_array(key.keyByte, MFRC522::MF_KEY_SIZE);
    Serial.println();

    Serial.println(F("BEWARE: Data will be written to the PICC, in sector #1"));

    pinMode(GREEN_LED_PIN, OUTPUT);
    pinMode(RED_LED_PIN, OUTPUT);
}

void loop() {

    if (Serial.available() > 0) {
    // Read the incoming message
    String message = Serial.readString();

    // Process the message
    if (message.startsWith("ADD_TAG")) {
      addTag();
    } else {
      Serial.println("REMOTE_OPEN");
      //remoteOpen();
    }
  }

  byte sector         = 1;
  byte blockAddr      = 4;
  byte dataBlock[]    = {};
  byte trailerBlock   = 7;
  MFRC522::StatusCode status;
  byte buffer[18];
  byte size = sizeof(buffer);

  void addTag(){

    Serial.println("Add Mode: Scan and registrate Tag.");
    Serial.println();

    if ( ! mfrc522.PICC_IsNewCardPresent())
            return;

        
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

        MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
        Serial.println(mfrc522.PICC_GetTypeName(piccType));

        // Check for compatibility
        if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
            &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
            Serial.println(F("This sample only works with MIFARE Classic cards."));
            return;
        }

        // Authenticate using key A
        Serial.println(F("Authenticating using key A..."));
        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }

        for (byte i = 0; i < rfid.uid.size; i++) { // send tag ID
            Serial.print(rfid.uid.uidByte[i], HEX);
          }
          Serial.println();

        
        if (Serial.available() => 2) { //incoming pass bytes
      
          Serial.readBytes(dataBlock, sizeof(dataBlock));
        }

        status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("PCD_Authenticate() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
            return;
        }

        status = (MFRC522::StatusCode) mfrc522.MIFARE_Write(blockAddr, dataBlock, 16); // write pass bytes to tag
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Write() failed: "));

            Serial.println(mfrc522.GetStatusCodeName(status));
        }
        Serial.println();

        status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size); // Reads the written pass bytes
        if (status != MFRC522::STATUS_OK) {
            Serial.print(F("MIFARE_Read() failed: "));
            Serial.println(mfrc522.GetStatusCodeName(status));
        }

        byte count = 0;
        for (byte i = 0; i < 16; i++) {
            // Compare buffer (= what we've read) with dataBlock (= what we've written)
            if (buffer[i] == dataBlock[i])
                count++;
        }

        if (count == 16) {
            Serial.println(F("Success :-)"));
            int msgTrue = 1;
            Serial.write((byte*)&msgTrue, sizeof(msgTrue));
        } else {
            int msgFalse = 0;
            Serial.write((byte*)&msgFalse, sizeof(msgFalse));
        }
        Serial.println();

        mfrc522.PICC_HaltA();
        
        mfrc522.PCD_StopCrypto1();

  }

  Serial.println(F("Normal mode: Scan tag to open door."));
  Serial.println();
  // Read Mode
  if ( ! mfrc522.PICC_IsNewCardPresent())
              return;

          
  if ( ! mfrc522.PICC_ReadCardSerial())
      return;

    MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Check for compatibility
    if (    piccType != MFRC522::PICC_TYPE_MIFARE_MINI
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_1K
        &&  piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
        Serial.println(F("This sample only works with MIFARE Classic cards."));
        return;
    }

    // Authenticate using key A
    Serial.println(F("Authenticating again using key B..."));
    status = (MFRC522::StatusCode) mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("PCD_Authenticate() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
        return;
    }
    
    status = (MFRC522::StatusCode) mfrc522.MIFARE_Read(blockAddr, buffer, &size); // Read pass from tag
    if (status != MFRC522::STATUS_OK) {
        Serial.print(F("MIFARE_Read() failed: "));
        Serial.println(mfrc522.GetStatusCodeName(status));
    }

    Serial.write(buffer, sizeof(buffer)); // send Pass bytes

    if (Serial.available() > 0) {
      // Read the incoming message
      String msg = Serial.readString();

      // Process the message
      if (message.startsWith("TRUE")) {
        // green light for 3 sec
        digitalWrite((GREEN_LED_PIN, HIGH); // Turn on the LED (HIGH)
        delay(3000);  // Wait for 1 second
        digitalWrite((GREEN_LED_PIN, LOW);
      } else ifmessage.startsWith("FALSE")){
        // red light for 3 sec
        digitalWrite((RED_LED_PIN, HIGH); // Turn on the LED (HIGH)
        delay(3000);  // Wait for 1 second
        digitalWrite((RED_LED_PIN, LOW);
      }

    }

  mfrc522.PICC_HaltA();
  
  mfrc522.PCD_StopCrypto1();