#pragma once
#ifndef Metallographic_h
#define Metallographic_h

#include <stdint.h>

class Metallographic
{
public:
    
    void init();
    void run();
    void Amoladora(int amo_state);
    void Lijas(int lija_state);
    void Refrigerante(int refri_state);
private:

};
#endif
