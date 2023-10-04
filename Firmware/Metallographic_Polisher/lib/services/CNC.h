#pragma once
#ifndef CNC_h
#define CNC_h

#include <stdint.h>

class CNC
{
public:
    
    int modo = 0;
    void init();
    void home();
    void run();
private:

};
#endif
