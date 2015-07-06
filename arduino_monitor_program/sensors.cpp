#include <Arduino.h>
#include "sensors.h"

//-------------------------------------------------
// Sensor
//-------------------------------------------------

char Sensor::getMessageTypeChar()
{  
  switch(_messageType) 
  {
    case Light:
      return 'L';
    case Temperature:
      return 'T';
    default:
      return '?';
  }
}

//-------------------------------------------------
// Light sensor
//-------------------------------------------------
/**
 * 
 */
LightSensor::LightSensor(int pin)
{
  _lightPin = pin;
  _isOn = false;
  _messageType = Light;
}

void LightSensor::setup()
{  
  pinMode(_lightPin, OUTPUT);

  if (_isOn)
  {
    digitalWrite(_lightPin, HIGH);
  }
  else
  {
    digitalWrite(_lightPin, LOW);    
  }
}

void LightSensor::read()
{
  if (_isOn)
  {
    printMessageLn("Light is on");
  }
  else
  {
    printMessageLn("Light is off");
  }  
}

bool LightSensor::hasChanged()
{
  return true;
}

void LightSensor::getTextData(char* buffer)
{
  buffer[0] = getMessageTypeChar();
  if (_isOn)
  {
    strcpy(buffer+1, "on");
  }
  else
  {
    strcpy(buffer+1, "off");
  }
  
  printMessage(F("Message "));
  printMessageLn(buffer);
}

//-------------------------------------------------
// Temperature sensor
//-------------------------------------------------

TemperatureSensor::TemperatureSensor(int pin)
{
  _inputPin = pin;
  _messageType = Temperature;
}

void TemperatureSensor::setup()
{  
  
}

void TemperatureSensor::read()
{
  int temperatureReadDelay = 20;
  // This equation converts the 0 to 1023 value that analogRead()
  // returns, into a 0.0 to 5.0 value that is the true voltage
  // being read at that pin.
  float voltage1 = analogRead(_inputPin) * 0.004882814;
  delay(temperatureReadDelay);
  float voltage2 = analogRead(_inputPin) * 0.004882814;
  delay(temperatureReadDelay);
  float voltage3 = analogRead(_inputPin) * 0.004882814;

  float voltage = (voltage1 + voltage2 + voltage3) / 3;
  
  printMessage(F("Read "));
  printMessageLn(voltage);

  // Now we'll convert the voltage to degrees Celsius.
  // This formula comes from the temperature sensor datasheet:

  _degrees = (voltage - 0.5) * 100.0;
  
  // While we're at it, let's convert degrees Celsius to Fahrenheit.
  // This is the classic C to F conversion formula:  
  //float degreesF = degreesC * (9.0/5.0) + 32.0;

  printMessage(_degrees);
  printMessageLn("C");
}

bool TemperatureSensor::hasChanged()
{
  return true;
}

void TemperatureSensor::getTextData(char* buffer)
{
  char temperature[10];
  buffer[0] = getMessageTypeChar();
  dtostrf(_degrees, 4, 2, temperature);
  strcpy(buffer+1, temperature);  

  printMessage(F("Message "));
  printMessageLn(buffer);  
}

