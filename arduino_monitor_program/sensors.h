#include "types.h"

class Sensor
{
	public: 
    enum MessageType { AcknowledgeOK, Light, Temperature, OtherS };

    virtual void setup();
    
    virtual void read();
    
		virtual void getTextData(char* buffer);

    virtual bool hasChanged();
    
    char getMessageTypeChar();

  protected:
    MessageType _messageType;
};

class LightSensor: public Sensor
{
	public:
    
    LightSensor(int pin);
    
    virtual void setup();        

    virtual void read();
    
    virtual bool hasChanged();
    
		virtual void getTextData(char* buffer);
    
  private:
    int _lightPin;     
    bool _isOn;
};

class TemperatureSensor: public Sensor
{
  public:
    
    TemperatureSensor(int pin);
    
    virtual void setup();        

    virtual void read();
    
    virtual bool hasChanged();
    
    virtual void getTextData(char* buffer);
    
  private:
    int _inputPin;     
    float _degrees;    
};
