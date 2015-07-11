#include "types.h"

class Sensor
{
	public: 
    enum MessageType { AcknowledgeOK, Light, Temperature, NoSensor };

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
    
    LightSensor(int pin);
    
    virtual void setup();        

    virtual void read();
    
    virtual void processMessage(char* message);
    
    virtual bool hasChanged();
    
		virtual void getTextData(char* buffer);
    
  private:
    int _lightPin;     
    bool _isOn;

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
