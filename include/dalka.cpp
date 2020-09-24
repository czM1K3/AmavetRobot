#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "_config.cpp"
#include "NewPing.h"

class dalka
{
private:
    NewPing senzor;
    int hodnota;
    int last;

public:
    dalka():senzor(NewPing(0,0)){}

    void init(int Trig, int Echo) // Metoda pro inicializaci ultrazvuku
    {
        senzor = NewPing(Trig,Echo,trashold);
        hodnota = 30;
        last = 30;
    }

    void measure() // Provede měření ale nic nevrátí
    {
        int dist = senzor.ping_cm();
        if (dist != 0)
        {
            last = hodnota;
            hodnota = dist;
        }
    }

    int get() // Vrací hodnotu jak je daleko
    {
        return hodnota;
    }

    int getLast()
    {
        return last;
    }
};
