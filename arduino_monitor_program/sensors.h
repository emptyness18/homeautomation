#include <EmonLib.h>                   
#include "types.h"

class Sensor
{
	public: 
    enum MessageType { AcknowledgeOK, Light, Temperature, Door, NoSensor };

    virtual void setup();
    
    virtual void read();

    virtual void processMessage(char* message);
    
		virtual void getTextData(char* buffer);

    virtual bool hasChanged();
    
    static char getMessageTypeChar(Sensor::MessageType messageType);
    static Sensor::MessageType getMessageTypeFromChar(char c);

    MessageType getMessageType();

  protected:
    MessageType _messageType;
};

class LightSensor: public Sensor
{
	public:
    
    LightSensor(int relayPin, int currentSensorPin);
    
    virtual void setup();        

    virtual void read();
    
    virtual void processMessage(char* message);
    
    virtual bool hasChanged();
    
		virtual void getTextData(char* buffer);
    
  private:
    int _lightRelayPin;     
    int _currentSensorPin;     
    bool _lastLightStatus;

    EnergyMonitor *_emon; 

    void updateLight();
};

class TemperatureSensor: public Sensor
{
  public:
    
    TemperatureSensor(int pin);
    
    virtual void setup();        

    virtual void read();
    
    virtual void processMessage(char* message);
    
    virtual bool hasChanged();
    
    virtual void getTextData(char* buffer);
    
  private:
    int _inputPin;     
    float _degrees;    
};

class DoorSensor: public Sensor
{
  public:
    
    DoorSensor(int pin);
    
    virtual void setup();        

    virtual void read();
    
    virtual void processMessage(char* message);
    
    virtual void getTextData(char* buffer);
    
    virtual bool hasChanged();
    
  private:
    const int OPEN_STATUS = 1;
    
    int _inputPin;     
    int _lastStatus;
    bool _changed;

    bool isOpen();
};

