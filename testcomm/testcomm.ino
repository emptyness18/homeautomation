#include <SPI.h>
#include "RF24.h"
#include <printf.h>

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

const int LED_PIN = 5;

bool isLEDOn = true;

void setup() {
  Serial.begin(115200);

  printf_begin();

  radio.begin();
  
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  // radio.setPALevel(RF24_PA_LOW);
  radio.setPALevel(RF24_PA_MAX);
  
  radio.enableDynamicPayloads();
  
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  
  // Start the radio listening for data
  radio.startListening();
  
  radio.printDetails();
  
  //Setup led
  pinMode(LED_PIN, OUTPUT);
}

void loop() {

    if (isLEDOn)
    {
      digitalWrite(LED_PIN, HIGH);
      Serial.println(F("Light is on"));
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
      Serial.println(F("Light is off"));
    }
    
    radio.stopListening();                                    // First, stop listening so we can talk.

    if (!radio.write( &isLEDOn, sizeof(bool) ))
    {
      Serial.println(F("failed"));
    }
    
    radio.startListening();                                    // Now, continue listening
    
    if (radio.available() )                           // While nothing is received
    {                                             // Describe the results
        char newLEDStatus[100];
        int len = radio.getDynamicPayloadSize();
        Serial.print(F("Len "));
        Serial.println(len);
        radio.read(&newLEDStatus, len );
        newLEDStatus[len] = 0;
        
        // Spew it
        Serial.print(F("Read "));
        Serial.print(newLEDStatus);
        Serial.println(F(" value"));
        
        if (String(newLEDStatus) == "off")
          isLEDOn = false;
          
        if (String(newLEDStatus) == "on")
          isLEDOn = true;
    }

    // Try again 1s later
    delay(100);    
}
