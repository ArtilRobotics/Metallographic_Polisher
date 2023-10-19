#include <Arduino.h>
#include <Metallographic.h>
#include <CNC.h>
#include <Servo.h>
#include "../constants/pinout.h"

Metallographic Pulidora;
CNC Ejes_Pulidora;
// Servo myservo;
int dato;

String Pos_Dedo;
String Pos_Valor;
String cadena;

String strT = "";
const char separatorT = ',';
const int dataLengthT = 6;
int datoT[dataLengthT];
bool homing_cnc = false;

void corte()
{
    Ejes_Pulidora.linear(0, 0, 5, 0);
    delay(100);
    Ejes_Pulidora.linear(5, 0, 5, 0);
    delay(100);
    Ejes_Pulidora.linear(5, 50, 5, 0);
    delay(100);
    Ejes_Pulidora.linear(5, 50, 100, 0);
    delay(100);
    digitalWrite(Rele_Amola, HIGH);
    Ejes_Pulidora.linear(5, 50, 110, 0);
    delay(300);
    Ejes_Pulidora.linear(5, 50, 100, 0);
    delay(100);
    digitalWrite(Rele_Amola, LOW);
    Ejes_Pulidora.linear(5, 50, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(5, 0, 0, 0);
    // Ejes_Pulidora.linear(10,0,10,1000);
}

void lijado()
{
    Ejes_Pulidora.linear(395, 0, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(395, 10, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(395, 30, 0, 0);
    delay(100);
    digitalWrite(Rele_Lijas, HIGH);
    Ejes_Pulidora.linear(395, 35, 0, 0);
    delay(100);
    digitalWrite(Rele_Lijas, LOW);
    Ejes_Pulidora.linear(395, 20, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(563, 20, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(563, 35, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(563, 20, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(717, 20, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(717, 35, 0, 0);
    delay(100);
    Ejes_Pulidora.linear(717, 0, 0, 0);
    delay(100);
}

void pulido()
{

    digitalWrite(A5, HIGH);
    delay(200);
    digitalWrite(A5, LOW);
    delay(10000);
}

void revision()
{

    digitalWrite(A5, HIGH);
    delay(200);
    digitalWrite(A5, LOW);
    delay(10000);
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
    // myservo.attach(A6);
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

    if (datoT[0] == 0 && homing_cnc == true)
    {
        homing_cnc = false;
    }
    if (datoT[0] == 1 && homing_cnc == false)
    {
        Ejes_Pulidora.home();
        homing_cnc = true;
    }
    if (datoT[0] == 2 && homing_cnc == false)
    {
        corte();
        homing_cnc = true;
    }
    if (datoT[0] == 3 && homing_cnc == false)
    {
        lijado();
        homing_cnc = true;
    }
    if (datoT[0] == 4 && homing_cnc == false)
    {
        Ejes_Pulidora.linear(datoT[1], datoT[2], datoT[3], datoT[4]);
        homing_cnc = true;
    }
    if (datoT[0] == 5 && homing_cnc == false)
    {
    }

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
