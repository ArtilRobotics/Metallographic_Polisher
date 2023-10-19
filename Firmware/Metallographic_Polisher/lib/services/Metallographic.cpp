#include <Metallographic.h>
#include <Arduino.h>
#include "../constants/pinout.h"

void Metallographic::init()
{
    Serial.begin(9600);
    pinMode(Rele_Amola, OUTPUT);
    pinMode(Rele_Lijas, OUTPUT);
    pinMode(Rele_Refrige, OUTPUT);
    pinMode(Buzzer, OUTPUT);
    pinMode(FC_X, INPUT_PULLUP);
    pinMode(FC_Y, INPUT_PULLUP);
    pinMode(FC_Z, INPUT_PULLUP);
}

void Metallographic::Amoladora(int amo_state)
{
    if (amo_state == 1)
    {
        digitalWrite(Rele_Amola,LOW);
    }
    else if (amo_state == 0)
    {
        digitalWrite(Rele_Amola,HIGH);
    }
}

void Metallographic::Lijas(int lija_state)
{
    if (lija_state == 1)
    {
        digitalWrite(Rele_Lijas,LOW);
    }
    else if (lija_state == 0)
    {
        digitalWrite(Rele_Lijas,HIGH);
    }
}

void Metallographic::Refrigerante(int refri_state)
{
    if (refri_state == 1)
    {
        digitalWrite(Rele_Refrige,LOW);
    }
    else if (refri_state == 0)
    {
        digitalWrite(Rele_Refrige,HIGH);
    }
}