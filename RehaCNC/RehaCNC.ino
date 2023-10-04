#include "Uduino.h"  // Incluye la libreria Unity
Uduino uduino("CNC");
#define xMS1 26
#define xMS2 28
#define xMS3 30

#define yMS1 38
#define yMS2 40
#define yMS3 42

#define zMS1 44
#define zMS2 46
#define zMS3 48

#define aMS1 32
#define aMS2 34
#define aMS3 36

#define X_ENDSTOP 9   /* X axis endstop input pin */
#define Y_ENDSTOP 10  /* Y axis endstop input pin */
#define Z_ENDSTOP 11  /* Z axis endstop input pin */
#define ABORT A0  /* Abort input pin */
#define HOLD A1   /* Hold input pin */
#define RESUME A2 /* Resume input pin */

int modo = 0;
int dir = 1;  //1 derecha -1 izquierda
int vel = 4; 
int rango = 4;
bool homing=false;
/**
   First thing this machine does on startup.  Runs only once.
*/
void setup() {
  UduinoSetUP();    //Inicializa la comunicacion con Unity
  //Serial1.begin(BAUD);
  // open coms
  pinMode(xMS1, OUTPUT);
  pinMode(xMS2, OUTPUT);
  pinMode(xMS3, OUTPUT);
  pinMode(yMS1, OUTPUT);
  pinMode(yMS2, OUTPUT);
  pinMode(yMS3, OUTPUT);
  pinMode(zMS1, OUTPUT);
  pinMode(zMS2, OUTPUT);
  pinMode(zMS3, OUTPUT);
  pinMode(aMS1, OUTPUT);
  pinMode(aMS2, OUTPUT);
  pinMode(aMS3, OUTPUT);
  bool ms1 = HIGH;
  bool ms2 = LOW;
  bool ms3 = LOW;


  digitalWrite(xMS1, ms1);
  digitalWrite(xMS2, ms2);
  digitalWrite(xMS3, ms3);

  digitalWrite(yMS1, ms1);
  digitalWrite(yMS2, ms2);
  digitalWrite(yMS3, ms3);

  digitalWrite(zMS1, ms1);
  digitalWrite(zMS2, ms2);
  digitalWrite(zMS3, ms3);

  digitalWrite(aMS1, ms1);
  digitalWrite(aMS2, ms2);
  digitalWrite(aMS3, ms3);

  /* Configure the control pins as inputs with pullups */
  pinMode(X_ENDSTOP, INPUT_PULLUP);
  pinMode(Y_ENDSTOP, INPUT_PULLUP);
  pinMode(Z_ENDSTOP, INPUT_PULLUP);

  pinMode(ABORT, INPUT_PULLUP);
  pinMode(HOLD, INPUT_PULLUP);
  pinMode(RESUME, INPUT_PULLUP);

  motor_setup();
  motor_enable();

  //where();  // for debugging purposes
  //help();  // say hello
  position(0, 0, 0, 0); // set starting position
  feedrate(1000000);  // set default speed
  ready();
}


/**
   After setup() this machine will repeat loop() forever.
*/
bool ControladoR = 0;
int bandera=0;
int finales()
{ 
    if (!digitalRead(X_ENDSTOP) && homing) 
    {
     bandera=1;
     delay(10);
     modo=0;
     //Serial.println("X-stop");
     motor_disable();
    } 
    if (!digitalRead(Y_ENDSTOP)&& homing) 
    {
     bandera=2;
     modo=0;
     delay(10);
     //Serial.println("Y-stop"); 
     motor_disable();     
    }  
    if (!digitalRead(Z_ENDSTOP)&& homing) 
    {
    //Serial.println("Z-stop"); 
     bandera=3;
     modo=0;
     delay(10);
     Serial.println("Z-stop"); 
     motor_disable();    
    }    
    if (!digitalRead(RESUME)) 
    {
    //Serial.println("Resume"); 
    bandera=4;
    modo=0;
    delay(10);
    motor_disable();
    }
    else if(bandera==4 && digitalRead(RESUME))
    {
      motor_enable();
      //bandera=0;
      homing=false;
    }
    return(bandera);
}

void loop()
{
  //
//  Serial.print(digitalRead(X_ENDSTOP));
//  Serial.print("   ");
//  Serial.print(digitalRead(Y_ENDSTOP));
//  Serial.print("   ");
//  Serial.print(digitalRead(Z_ENDSTOP));
    //Serial.print("   ");
    //Serial.println(bandera);
    finales();
  uduino.update();
  delay(100);
  if (uduino.isConnected())
  {
   if (ControladoR)
    { 
      Controlador();
      //bandera=0;
    }
  }
}
