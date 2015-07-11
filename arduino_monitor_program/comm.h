#include <Arduino.h>
#include "RF24.h"

class Communicator
{
	public:
    Communicator(uint64_t readAddress, uint64_t writeAddress, int cePin, int csnPin);
    
		void setup();

    bool send(char* data);

    bool receive(char* data);
   
  private:
		uint64_t _readAddress;
		uint64_t _writeAddress;
		int _cePin;
		int _csnPin;

    RF24* _radio;
};
