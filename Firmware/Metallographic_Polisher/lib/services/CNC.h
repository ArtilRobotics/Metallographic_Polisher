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
    void linear(int l_x, int l_y, int l_z, int l_b);
    void VelMotors(float vel);
    void run();
private:

};
#endif
