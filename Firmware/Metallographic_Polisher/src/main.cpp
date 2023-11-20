#include <Arduino.h>
#include <Metallographic.h>
#include <CNC.h>
#include <Servo.h>
#include "../constants/pinout.h"

Metallographic Pulidora;
CNC Ejes_Pulidora;
Servo myservo;
int dato;
int map_vel;

int longitud = 0;

int k;
int j;
int l;
int tiempo_material;
int tiempo_diametro;

String Pos_Dedo;
String Pos_Valor;

String cadena;
String strT = "";
const char separatorT = ',';
const int dataLengthT = 7;
int datoT[dataLengthT];
bool homing_cnc = false;

void corte()
{
    Serial.write("4,3,0,0,0.1");
    Serial.write('\n');
    Serial.write("6,0,0,0,0.2");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, longitud, 0, 0, 0);
    delay(200);
    Serial.write("6,0,0,0,0.3");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, longitud, 0, 0, 0);
    delay(200);
    Serial.write("6,0,0,0,0.4");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, longitud, 100, 0, 0);
    digitalWrite(Rele_Amola, HIGH);
    delay(200);
    for (int n = 115; n < 138; n++)
    {
        Serial.write("6,0,0,0,0.5");
        Serial.write('\n');
        k = n;
        Ejes_Pulidora.linear(1, longitud, n, l, 0);

        for (j; j > -1275 + l; j = j - 15)
        {

            Ejes_Pulidora.linear(1, longitud, k, j, 0);
            delay(5);
        }
        l = j;
        delay(500);
    }
    delay(300);
    Serial.write("6,0,0,0,0.6");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, longitud, 100, l, 0);
    delay(200);
    digitalWrite(Rele_Amola, LOW);
    Serial.write("6,0,0,0,0.7");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, longitud, 0, l, 0);
    delay(200);
    Serial.write("6,0,0,0,1");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, 0, 0, l, 0);
    delay(200);
    Serial.write("6,0,0,0,8");
    Serial.write('\n');
    Ejes_Pulidora.linear(1, 0, 0, 0, 0);
    delay(200);
    Serial.write("");
    Serial.write('\n');
}

void lijado()
{
    Serial.write("4,4,0,0,0");
    Serial.write('\n');
    delay(100);
    Serial.write("6,0,0,0,0");
    Serial.write('\n');
    Ejes_Pulidora.linear(196, 0, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.07");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(196, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.14");
    Serial.write('\n');
    delay(200);
    digitalWrite(Rele_Lijas, HIGH);
    Ejes_Pulidora.linear(196, longitud + 7, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.21");
    Serial.write('\n');
    delay(5000);
    Ejes_Pulidora.linear(196, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.28");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(196, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.35");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(281, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.42");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(281, longitud + 3, 0, 400, 0);
    delay(100);
    Serial.write("6,0,0,0,0.49");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(281, longitud + 7, 0, 400, 0);
    delay(100);
    Serial.write("6,0,0,0,0.56");
    Serial.write('\n');
    delay(5000);
    Ejes_Pulidora.linear(281, longitud + 3, 0, 400, 0);
    delay(100);
    Serial.write("6,0,0,0,0.63");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(359, longitud + 3, 0, 400, 0);
    delay(100);
    Serial.write("6,0,0,0,0.7");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(359, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.77");
    Serial.write('\n');
    delay(200);
    Ejes_Pulidora.linear(359, longitud + 7, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.8");
    Serial.write('\n');
    delay(5000);
    Ejes_Pulidora.linear(359, longitud + 3, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.9");
    Serial.write('\n');
    delay(200);
    digitalWrite(Rele_Lijas, LOW);
    Ejes_Pulidora.linear(359, 0, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,1");
    Serial.write('\n');
    delay(100);
    Serial.write("");
    Serial.write('\n');
}

void pulido()
{
    Serial.write("4,5,0,0,0");
    Serial.write('\n');
    Serial.write("6,0,0,0,0");
    Serial.write('\n');
    Ejes_Pulidora.linear(0, 0, 0, 0, 0);
    delay(100);
    Serial.write("6,0,0,0,0.1");
    Serial.write('\n');
    Ejes_Pulidora.linear(0, longitud + 5, 0, 0, 0);
    Serial.write("6,0,0,0,0.2");
    Serial.write('\n');
    delay(100);
    for (int i = 0; i < 1000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.3");
    Serial.write('\n');
    Ejes_Pulidora.linear(0, longitud + 13, 0, 0, 0);
    delay(100);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.4");
    Serial.write('\n');
    Ejes_Pulidora.linear(0, longitud + 13, 0, 400, 0);
    delay(100);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.5");
    Serial.write('\n');
    Ejes_Pulidora.linear(10, longitud + 13, 0, 400, 0);
    delay(100);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.6");
    Serial.write('\n');
    Ejes_Pulidora.linear(10, longitud + 13, 0, 800, 0);
    delay(100);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.7");
    Serial.write('\n');
    Ejes_Pulidora.linear(15, longitud + 13, 0, 800, 0);
    delay(100);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.8");
    Serial.write('\n');
    Ejes_Pulidora.linear(15, longitud + 13, 0, 1200, 0);
    for (int i = 0; i < 3000; i++)
    {
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
    }
    Serial.write("6,0,0,0,0.9");
    Serial.write('\n');
    Ejes_Pulidora.linear(15, 0, 0, 1200, 0);
    delay(100);
    Serial.write("6,0,0,0,1");
    Serial.write('\n');
    Ejes_Pulidora.linear(15, 0, 0, 0, 0);
    delay(100);
}

void revision()
{
    Serial.write("4,6,0,0,0");
    Serial.write('\n');
    Serial.write("6,0,0,0,0");
    Serial.write('\n');
    Ejes_Pulidora.linear(110, 0, 0, 0, 0);
    Serial.write("6,0,0,0,0.2");
    Serial.write('\n');
    delay(100);
    Ejes_Pulidora.linear(110, longitud + 28, 0, 0, 0);
    Serial.write("6,0,0,0,0.5");
    Serial.write('\n');
}

void aprob_revision()
{
    Ejes_Pulidora.linear(110, 0, 0, 0, 0);
    Serial.write("6,0,0,0,0.7");
    Serial.write('\n');
    delay(100);
    Ejes_Pulidora.linear(0, 0, 0, 0, 0);
    Serial.write("6,0,0,0,1");
    Serial.write('\n');
}

void setup()
{
    Pulidora.init();
    Ejes_Pulidora.init();
    delay(1000);
    // Ejes_Pulidora.home();

    // Low rele apagado
    digitalWrite(A3, LOW);
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);
    digitalWrite(12, LOW);
    Ejes_Pulidora.VelMotors(1000000);
    myservo.attach(A6);
    myservo.write(60);
}

void loop()
{
    digitalWrite(12, LOW);

    cadena = "";
    while (Serial.available())
    {
        cadena = Serial.readStringUntil('\n');

        for (int i = 0; i < dataLengthT; i++)
        {
            int index = cadena.indexOf(separatorT);
            datoT[i] = cadena.substring(0, index).toInt();
            cadena = cadena.substring(index + 1);
        }
    }

    switch (datoT[0])
    {

    case 2:

        switch (datoT[1])
        {
        case 1:
            tiempo_material = 100;
            datoT[1] = 0;
            break;

        case 2:
            tiempo_material = 200;
            datoT[1] = 0;
            break;

        case 3:
            tiempo_material = 300;
            datoT[1] = 0;
            break;

        default:
            break;
        }

        if (datoT[3] == 1)
        {
            corte();
        }
        if (datoT[4] == 1)
        {
            lijado();
        }
        if (datoT[5] == 1)
        {
            pulido();
        }
        if (datoT[6] == 1)
        {
            revision();
        }
        datoT[0] = 0;
        break;

    case 3:
        Ejes_Pulidora.linear(datoT[1], datoT[2], datoT[3], datoT[4], datoT[5]);
        datoT[0] = 0;
        break;

    case 4:
        Ejes_Pulidora.home();
        Serial.write("40,0,0,0,0");
        Serial.write('\n');
        datoT[0] = 0;
        break;

    case 5:
        digitalWrite(Rele_Amola, HIGH);
        datoT[0] = 0;
        break;

    case 6:
        digitalWrite(Rele_Amola, LOW);
        datoT[0] = 0;
        break;

    case 7:
        digitalWrite(Rele_Lijas, HIGH);
        datoT[0] = 0;
        break;

    case 8:
        digitalWrite(Rele_Lijas, LOW);
        datoT[0] = 0;
        break;

    case 9:
        map_vel = map(datoT[1], 0, 100, 60, 160);
        myservo.write(map_vel);
        datoT[0] = 0;
        break;

    case 11:
        aprob_revision();
        datoT[0] = 0;
        break;

    case 12:
        lijado();
        delay(1000);
        pulido();
        datoT[0] = 0;
        break;

    default:
        break;
    }

    switch (datoT[5])
    {
    case 2:
        digitalWrite(10, HIGH);
        delay(1);
        digitalWrite(10, LOW);
        delay(1);
        break;

    default:
        break;
    }

    if (datoT[0]==21)
    {
        longitud=datoT[1];
    }
    
    // if (datoT[5] == 2)
    // {
    // }
    // if (datoT[5] == 3)
    // {
    // }

    // digitalWrite(A4, LOW);
    // delay(5000);
    // digitalWrite(A4, HIGH);
    // delay(5000);

    // if (datoT[0] == 0 && homing_cnc == true)
    // {
    //     homing_cnc = false;
    // }
    // if (datoT[0] == 1 && homing_cnc == false)
    // {
    //     Ejes_Pulidora.home();
    //     homing_cnc = true;
    // }
    // if (datoT[0] == 2 && homing_cnc == false)
    // {
    //     corte();
    //     homing_cnc = true;
    // }
    // if (datoT[0] == 3 && homing_cnc == false)
    // {
    //     lijado();
    //     homing_cnc = true;
    // }
    // if (datoT[0] == 4 && homing_cnc == false)
    // {
    //     Ejes_Pulidora.linear(datoT[1], datoT[2], datoT[3], datoT[4]);
    //     homing_cnc = true;
    // }
    // if (datoT[0] == 5 && homing_cnc == false)
    // {
    // }

    // if (datoT[5] == 0)
    // {
    //     digitalWrite(A5, LOW);
    // }
    // else if (datoT[5] == 1)
    // {
    //     digitalWrite(A5, HIGH);
    // }

    // Serial.print(datoT[0]);
    // Serial.print(", ");
    // Serial.print(datoT[1]);
    // Serial.print(", ");
    // Serial.print(datoT[2]);
    // Serial.print(", ");
    // Serial.println(datoT[3]);

    // Serial.print(digitalRead(14));
    // delay(2);
    // Serial.print("   ");
    // Serial.print(digitalRead(15));
    // delay(2);
    // Serial.print("   ");
    // Serial.println(digitalRead(16));
    // delay(10);
}
