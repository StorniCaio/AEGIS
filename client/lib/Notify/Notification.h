#ifndef n
#define n
#include "Arduino.h"

using namespace std;

class n{

    private:
        int led;
        int slepTime = 500;

    public:
        n();
        void setLedPin(int pin);
        void wifiNotConnected();
        void mqttNotFound();
};

#endif