#include "comm.h"
#include "types.h"

Communicator::Communicator(uint64_t readAddress, uint64_t writeAddress, int cePin, int csnPin)
{
  _readAddress = readAddress;
  _writeAddress = writeAddress;
  _cePin = cePin;
  _csnPin = csnPin;
}

void Communicator::setup()
{
  _radio = new RF24(_cePin, _csnPin);

  _radio->begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  // radio.setPALevel(RF24_PA_LOW);
  _radio->setPALevel(RF24_PA_MAX);
  
  _radio->enableDynamicPayloads();
  
  _radio->openWritingPipe(_writeAddress);
  _radio->openReadingPipe(1,_readAddress);
  
  // Start the radio listening for data
  _radio->startListening();
  
  _radio->printDetails();
  
  _radio->stopListening();
}

bool Communicator::send(char* data)
{
  // First, stop listening so we can talk.
  _radio->stopListening();                                    

  if (!_radio->write(data, strlen(data)))
  {
    printMessageLn(F("Failed to send data."));
    return false;
  }
  else
  {
    printMessage(F("Sent "));
    printMessageLn(data);
    return true;
  }
}

bool Communicator::receive(char* data)
{
  // Now, continue listening
  _radio->startListening();
    
  if (_radio->available() )                           
  {                                             
    int len = _radio->getDynamicPayloadSize();
    Serial.print(F("Len "));
    Serial.println(len);
    _radio->read(data, len );
    data[len] = 0;
    
    // Spew it
    Serial.print(F("Read "));
    Serial.print(data);
    Serial.println(F(" value"));
    
    return true;
  }
  else
  {
    return false;   
  }
}


/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
/*
RF24 radio(7,8);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup() {

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
  
}

void loop() {

    char buffer[30];
    
    if (isLEDOn)
    {
      digitalWrite(LED_PIN, HIGH);
      Serial.println(F("Light is on"));
      createMessage(Sensor::Light, "1123", buffer);
    }
    else
    {
      digitalWrite(LED_PIN, LOW);
      Serial.println(F("Light is off"));
      createMessage(Sensor::Light, "0abc", buffer);
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
    delay(1000);    
}

void createMessage(Sensor::MessageType type, char data[], char buffer[]) {
    switch (type)
    {
      case Sensor::Light:
        buffer[0] = MessageTypeLight;
        strcpy(buffer+1, data);
        break;
    }
}

void printMessage(char msg[])
{
  Serial.println(msg);
}*/
