#include <Arduino.h>
#include "_config.cpp"

class relay{
private:
    int pin1;
    int pin2;

public:
    void init(int Pin1, int Pin2)
    {
        pin1 = Pin1;
        pin2 = Pin2;
        pinMode(pin1,OUTPUT);
        pinMode(pin2,OUTPUT);
        stop();
    }

    void start()
    {
        digitalWrite(pin1,LOW);
        digitalWrite(pin2,LOW);
    }

    void stop()
    {
        digitalWrite(pin1,HIGH);
        digitalWrite(pin2,HIGH);
    }
};