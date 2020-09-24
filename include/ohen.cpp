#include <Arduino.h>
#include "_config.cpp"

class ohen
{
private:
    int pin;

public:
    int hodnota;
    void init(int Pin)
    {
        pin = Pin;
        pinMode(pin, INPUT);
    }

    void refresh()
    {
        hodnota = digitalRead(pin);
    }
};