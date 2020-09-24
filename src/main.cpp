#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include "dalka.cpp"
#include "barva.cpp"
#include "_config.cpp"
#include "_definepins.cpp"
#include "rele.cpp"
#include "motor.cpp"
#include "ohen.cpp"

#define ArrayCount(array) (sizeof array / sizeof array[0])

motor motory;
relay rele;
barva barvy[3];
dalka dalky[5];
ohen ohne[4][3];

bool maohen; // Zapnutí focusu na oheň
bool macaru;
bool maprekazku;
int turncounter;
int candlecounter;

void setup()
{
  Serial.begin(9600);

  maohen = false;
  maprekazku = false;
  turncounter = 0;
  candlecounter = 0;

  motory.init(motorlevy, motorpravy); // Určení motorů
  for (int i = 0; i < int(ArrayCount(barvy)); i++)
  { // Určení senzorů na čáru
    barvy[i].init(barvypiny[i]);
  }
  for (int i = 0; i < int(ArrayCount(ohne)); i++)
  { // Určení senzorů na oheň
    for (int j = 0; j < int(ArrayCount(ohne[0])); j++)
    {
      ohne[i][j].init(ohenpiny[i][j]);
    }
  }
  for (int i = 0; i < int(ArrayCount(dalky)); i++)
  { // Určení ultrazvuků
    dalky[i].init(dalkypiny[i][0], dalkypiny[i][1]);
  }
  rele.init(relepiny[0], relepiny[1]);

  motory.runForward(defaultspeed);
}
void loop()
{
  macaru = false;
  for (int i = 0; i < int(ArrayCount(barvy)); i++)
  { // Získávání dat ze senzorů na čáru
    barvy[i].refresh();
    if (barvy[i].hodnota == 1)
    {
      macaru = true;
    }
  }
  for (int i = 0; i < int(ArrayCount(ohenpiny)); i++)
  { // Načtení senzorů na oheň
    for (int j = 0; j < int(ArrayCount(ohenpiny[0])); j++)
    {
      ohne[i][j].refresh();
      if (ohne[i][j].hodnota == 0)
      {
        maohen = true;
      }
    }
  }
  for (int i = 0; i < (int)ArrayCount(dalky); i++)
  { // Načtení hodnot z ultrazvuků
    dalky[i].measure();
    if (dalky[i].get() < distance)
    {
      maprekazku = true;
    }
  }

#ifdef WPFsenderEnabled
  Serial.println(
      String(barvy[0].hodnota) + ";" +
      String(barvy[1].hodnota) + ";" +
      String(barvy[2].hodnota) + ";" +
      String(dalky[0].get()) + ";" +
      String(dalky[1].get()) + ";" +
      String(dalky[2].get()) + ";" +
      String(dalky[3].get()) + ";" +
      String(dalky[4].get()) + ";" +
      String(ohne[0][0].hodnota) + ";" +
      String(ohne[0][1].hodnota) + ";" +
      String(ohne[0][2].hodnota) + ";" +
      String(ohne[1][0].hodnota) + ";" +
      String(ohne[1][1].hodnota) + ";" +
      String(ohne[1][2].hodnota) + ";" +
      String(ohne[2][0].hodnota) + ";" +
      String(ohne[2][1].hodnota) + ";" +
      String(ohne[2][2].hodnota) + ";" +
      String(ohne[3][0].hodnota) + ";" +
      String(ohne[3][1].hodnota) + ";" +
      String(ohne[3][2].hodnota) + ";");
#endif

if (candlecounter == 4)
{
  delay(10000);
  return;
}


#ifdef test
 motory.stop();
// delay(10000);
#else
  if (maohen)
  { // Pokud vidí oheň
    if (ohne[0][1].hodnota == 0 || ohne[1][1].hodnota == 0 || ohne[2][1].hodnota == 0 || ohne[3][1].hodnota == 0)
    { // Pokud vidí svíčku na středu
      if (dalky[0].get() > candledistance)
      { // Jesli je daloko tak ať jede dopředu
        motory.runForward(defaultspeed);
      }
      else
      { // Jinak začne hasit
        motory.stop();
        rele.start();
        while (ohne[0][1].hodnota == 0 || ohne[1][1].hodnota == 0 || ohne[2][1].hodnota == 0 || ohne[3][1].hodnota == 0)
        {
          delay(spintime);
          for (int i = 0; i < int(ArrayCount(ohenpiny)); i++) ohne[i][1].refresh();
        }
        
        rele.stop();
        motory.runBackward(candleback);
        motory.turnLeft(rotate180);
        candlecounter++;
        maohen = false;
      }
    }
    else if (ohne[0][0].hodnota == 0 || ohne[1][0].hodnota == 0 || ohne[2][0].hodnota == 0 || ohne[3][0].hodnota == 0)
    { // Pokud vidí svíčku na levo
      motory.stop();
      motory.turnLeft(rotatetrochu);
    }
    else if (ohne[0][2].hodnota == 0 || ohne[1][2].hodnota == 0 || ohne[2][2].hodnota == 0 || ohne[3][2].hodnota == 0)
    { // Pokud vidí svíčku v pravo
      motory.stop();
      motory.turnLeft(rotatetrochu);
    }
    
    motory.runForward(defaultspeed);
    return;
  }

  if (maprekazku)
  { // Pokud vidí překážku
    if (dalky[0].get() < distance)
    { // Jesli ji vidí přesně před sebou
      motory.stop();
      motory.turnRight(rotate90);
    }
    else if (dalky[1].get() < distance)
    { // Jesli ji vidí na předním levým senzoru 
      motory.stop();
      motory.turnRight(rotate45);
    }
    else if (dalky[2].get() < distance)
    { // Jestli ji vidí na předním pravým senzoru
      motory.stop();
      motory.turnRight(rotate135);
    }
    else if (dalky[3].get() > longdistance && dalky[3].getLast() < longdistance)
    { // Jestli přejel stěnu
      turncounter++;
      if (turncounter > 4)
      { // Pokud ji objel už 4x tak už nebude
        turncounter = 0;
        maprekazku = false;
      }
      else{
        delay(overdist);
        motory.stop();
        motory.turnLeft(rotate90);
      }
    }
    /*else if (dalky[3].get() > dalky[3].getLast())
      motory.littleLeft(rotatelittle);
    else if (dalky[3].get() < dalky[3].getLast())
      motory.littleRight(rotatelittle);*/
    motory.runForward(defaultspeed);
    return;
  }
  
  if (macaru && !ignoreline)
  { // Pokud vidí čáru
    if (barvy[0].hodnota == 1)
    { // Pokud to vidí vlevo tak se otočí doprava
      motory.stop();
      motory.runBackward(linebackward);
      motory.turnRight(rotate90);
    }
    else if (barvy[1].hodnota == 1)
    { // Pokud to vidí pravo tak se otorčí doleva
      motory.stop();
      motory.runBackward(linebackward);
      motory.turnLeft(rotate90);
    }
    motory.runForward(defaultspeed);
    return;
  }
#endif
}