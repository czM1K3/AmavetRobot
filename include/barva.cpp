#include <Arduino.h>
#include "_config.cpp"

int invertColors(int barva)
{ // Prohodí 1 na 0 a obráceně
    if (barva == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

class barva
{
private:
    int pin;

public:
    int hodnota;
    void init(int Pin)
    {
        pin = Pin;
        pinMode(pin, INPUT);
        hodnota = 0;
    }

    void refresh()
    {
#ifdef invertfloor
        hodnota = digitalRead(pin);
#endif
        hodnota = digitalRead(pin);
    }
};