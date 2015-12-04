#include <SPI.h>
#include <printf.h>
#include <EmonLib.h>                   
#include "types.h"
#include "sensors.h"
#include "comm.h"


const int endLoopDelay = 2500;
const int CURRENT_SENSOR_PIN = 1;
const int LIGHT_RELAY_PIN = 2;
const int TEMPERATURE_SENSOR_PIN = 0;
const int DOOR_SENSOR_PIN = 3;

Sensor* sensors[10];
int sensorsCount = 0;

Communicator comm((uint64_t)0xF0F0F0F0E1LL, (uint64_t )0xF0F0F0F0D2LL, 7, 8);

void setup() {
  Serial.begin(115200);
  printf_begin();

  comm.setup();

  LightSensor* lightSensor = new LightSensor(LIGHT_RELAY_PIN, CURRENT_SENSOR_PIN);
  registerSensor(lightSensor);

  TemperatureSensor* temperatureSensor = new TemperatureSensor(TEMPERATURE_SENSOR_PIN);
  registerSensor(temperatureSensor);
  
  DoorSensor* doorSensor = new DoorSensor(DOOR_SENSOR_PIN);
  //registerSensor(doorSensor);
  
  for (int i = 0; i < sensorsCount; i++)
  {
    Sensor *s = sensors[i];
    s->setup();
  }  
}

void loop() {

  char buffer[30];
  
  for (int i = 0; i < sensorsCount; i++)
  {
    Sensor *s = sensors[i];
    s->read();

    if (s->hasChanged())
    {
      s->getTextData(buffer);

      comm.send(buffer);
    }
  }

  if (comm.receive(buffer))
  {
    for (int i = 0; i < sensorsCount; i++)
    {
      Sensor *s = sensors[i];      
  
      if (s->getMessageType() == Sensor::getMessageTypeFromChar(buffer[0]))
      {
        s->processMessage(buffer);
      }
    }    
  }

  printMessageLn(F("\n"));
  // Try again 1s later
  delay(endLoopDelay);    
}

void registerSensor(Sensor *s)
{
  sensors[sensorsCount] = s;
  sensorsCount++;
}

