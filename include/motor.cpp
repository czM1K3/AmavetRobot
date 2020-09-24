#include <Arduino.h>
#include <AFMotor.h>

class motor{
private:
    AF_DCMotor levy;
    AF_DCMotor pravy;

public:
    motor():levy(AF_DCMotor(motorlevy)),pravy(AF_DCMotor(motorpravy)){}
    void init(int pinLevy, int pinPravy)
    {
        levy.setSpeed(defaultspeed);
        pravy.setSpeed(defaultspeed);
        levy.run(RELEASE);
        pravy.run(RELEASE);
    }

    void runForward(int speed)
    {
        levy.setSpeed(speed);
        pravy.setSpeed(speed);
        levy.run(FORWARD);
        pravy.run(FORWARD);
    }

    void stop()
    {
        levy.run(RELEASE);
        pravy.run(RELEASE);
        delay(stopdelay);
    }

    void turnRight(int time)
    {
        levy.setSpeed(rotatespeed);
        pravy.setSpeed(rotatespeed);
        levy.run(FORWARD);
        pravy.run(BACKWARD);
        delay(time);
        stop();
    }

    void turnLeft(int time)
    {
        levy.setSpeed(rotatespeed);
        pravy.setSpeed(rotatespeed);
        levy.run(BACKWARD);
        pravy.run(FORWARD);
        delay(time);
        stop();
    }

    void runBackward(int time)
    {
        levy.setSpeed(defaultspeed);
        pravy.setSpeed(defaultspeed);
        levy.run(BACKWARD);
        pravy.run(BACKWARD);
        delay(time);
        stop();
    }

    void littleLeft(int time)
    {
        levy.setSpeed(defaultspeed);
        pravy.setSpeed(defaultspeed);
        levy.run(RELEASE);
        pravy.run(FORWARD);
        delay(time);
    }

    void littleRight(int time)
    {
        levy.setSpeed(defaultspeed);
        pravy.setSpeed(defaultspeed);
        levy.run(FORWARD);
        pravy.run(RELEASE);
        delay(time);
    }
};