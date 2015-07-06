#include <SPI.h>
#include <printf.h>
#include "types.h"
#include "sensors.h"
#include "comm.h"


const int endLoopDelay = 1000;
const int LIGHT_PIN = 9;
const int TEMPERATURE_PIN = 0;

Sensor* sensors[10];
int sensorsCount = 0;

Communicator comm((uint64_t)0xF0F0F0F0E1LL, (uint64_t )0xF0F0F0F0D2LL, 7, 8);

void setup() {
  Serial.begin(115200);
  printf_begin();

  comm.setup();

  LightSensor* lightSensor = new LightSensor(LIGHT_PIN);
  registerSensor(lightSensor);

  TemperatureSensor* temperatureSensor = new TemperatureSensor(TEMPERATURE_PIN);
  registerSensor(temperatureSensor);
  
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
    }
  }
  
  // Try again 1s later
  delay(endLoopDelay);    
}

void registerSensor(Sensor *s)
{
  sensors[sensorsCount] = s;
  sensorsCount++;
}

