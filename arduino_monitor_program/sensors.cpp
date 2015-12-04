#include <Arduino.h>
#include "sensors.h"

//-------------------------------------------------
// Sensor
//-------------------------------------------------

char Sensor::getMessageTypeChar(Sensor::MessageType messageType)
{  
  switch(messageType) 
  {
    case Light:
      return 'L';
    case Temperature:
      return 'T';
    default:
      return '?';
  }
}

Sensor::MessageType Sensor::getMessageTypeFromChar(char c)
{
  switch(c) 
  {
    case 'L':
      return Light;
    case 'T':
      return Temperature;
    default:
      return NoSensor;
  } 
}

Sensor::MessageType Sensor::getMessageType()
{
  return _messageType;
}

//-------------------------------------------------
// Light sensor
//-------------------------------------------------
/**
 * 
 */
LightSensor::LightSensor(int relayPin, int currentSensorPin)
{
  _lightRelayPin = relayPin;
  _currentSensorPin = currentSensorPin;
  _lastLightStatus = false;

  _emon = new EnergyMonitor();
  
  _messageType = Light;
}

void LightSensor::setup()
{  
  pinMode(_lightRelayPin, OUTPUT);

  _emon->current(_currentSensorPin, 17);

  updateLight();
}

void LightSensor::read()
{
  double irms = _emon->calcIrms(1480) * 120.0;  

  printMessageLn(irms);
  
  bool currentStatus = irms > 10; //Suponiendo un bombillo mayor a 10 watts
  
  if (_lastLightStatus != currentStatus)
  {
    _lastLightStatus = currentStatus;
    
    if (_lastLightStatus)
    {
      printMessageLn("Light is on");
    }
    else
    {
      printMessageLn("Light is off");
    }    
  }
}

void LightSensor::processMessage(char* message)
{
  if (strcmp(message+1, ("on")) == 0)
  {
    _lastLightStatus = true;
    updateLight();
  }
  else if (strcmp(message+1, ("off")) == 0)
  {
    _lastLightStatus = false;
    updateLight();
  }
  else 
  {
    printMessage("Cannot process message ");
    printMessageLn(message);
  }
}

bool LightSensor::hasChanged()
{
  return true;
}

void LightSensor::getTextData(char* buffer)
{
  buffer[0] = getMessageTypeChar(_messageType);
  if (_lastLightStatus)
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

void LightSensor::updateLight()
{
  if (_lastLightStatus)
  {
    digitalWrite(_lightRelayPin, LOW);
  }
  else
  {
    digitalWrite(_lightRelayPin, HIGH);    
  }
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

void TemperatureSensor::processMessage(char* message)
{
  printMessageLn("Process Temperature");
}

bool TemperatureSensor::hasChanged()
{
  return true;
}

void TemperatureSensor::getTextData(char* buffer)
{
  char temperature[10];
  buffer[0] = getMessageTypeChar(_messageType);
  dtostrf(_degrees, 4, 2, temperature);
  strcpy(buffer+1, temperature);  

  printMessage(F("Message "));
  printMessageLn(buffer);  
}

//-------------------------------------------------
// Door sensor
//-------------------------------------------------

DoorSensor::DoorSensor(int pin)
{
  _inputPin = pin;
  _messageType = Door;
  _lastStatus = -1;
}

void DoorSensor::setup()
{  
  pinMode(_inputPin, INPUT);
  Serial.print("setup ");
  Serial.println(_inputPin);
}

void DoorSensor::read()
{
  int status = digitalRead(_inputPin);
  if (_lastStatus != status)
  {
    _lastStatus = status;
    _changed = true;
    Serial.println("changed");
  }
  else
  {
    _changed = false;
  }
  Serial.print("read ");
  Serial.println(_lastStatus);
}

void DoorSensor::processMessage(char* message)
{
  
}

bool DoorSensor::hasChanged()
{
  return _changed;
}

void DoorSensor::getTextData(char* buffer)
{
  buffer[0] = getMessageTypeChar(_messageType);
  if (isOpen())
  {
    strcpy(buffer+1, "open");
  }
  else
  {
    strcpy(buffer+1, "close");
  }
  
  printMessage(F("Message "));
  printMessageLn(buffer);
  
}

bool DoorSensor::isOpen()
{
  return _lastStatus == OPEN_STATUS;
}

