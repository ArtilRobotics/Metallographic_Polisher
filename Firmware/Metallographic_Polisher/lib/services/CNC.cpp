#include <CNC.h>
#include <Arduino.h>
#include "../constants/pinout.h"

//------------------------------------------------------------------------------
// CONSTANTS
//------------------------------------------------------------------------------
//#define VERBOSE              (1)       // add to get a lot more serial output.

#define VERSION              (2)                      // firmware version
#define BAUD                 (9600)                 // How fast is the Arduino rangoking?(BAUD Rate of Arduino)
#define MAX_BUF              (64)                     // What is the longest message Arduino can store?
#define STEPS_PER_TURN       (200)                    // depends on your stepper motor.  most are 200.
#define STEPS_PER_MM         (STEPS_PER_TURN/1.8)  // (400*16)/0.8 with a M5 spindle 2mm
#define MAX_FEEDRATE         (1000000)
#define MIN_FEEDRATE         (1)
#define NUM_AXIES            (5)

// for arc directions
#define ARC_CW          (1)
#define ARC_CCW         (-1)
// Arcs are split into many line segments.  How long are the segments?
#define MM_PER_SEGMENT  (10)

//------------------------------------------------------------------------------
// STRUCTS
//------------------------------------------------------------------------------
// for line()
typedef struct {
  long delta;  // number of steps to move
  long absdelta;
  long over;  // for dx/dy bresenham calculations
} Axis;


typedef struct {
  int step_pin;
  int dir_pin;
  int enable_pin;
  int limit_switch_pin;
} Motor;


//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------
Axis a[NUM_AXIES];  // for line()
Axis atemp;  // for line()
Motor motors[NUM_AXIES];

char buffer[MAX_BUF];  // where we store the message until we get a ';'
int sofar;  // how much is in the buffer

// speeds
float fr = 0; // human version
long step_delay;  // machine version

float px, py, pz, pe, pb; // position

// settings
char mode_abs = 1; // absolute mode?

long line_number = 0;

// para
int bandera=0;
extern int modo;
bool homing=false;

void CNC::init()
{

  motors[0].step_pin = MotorX_step_pin;
  motors[0].dir_pin = MotorX_dir_pin;
  motors[0].limit_switch_pin = MotorX_limit_switch_pin;

  motors[1].step_pin = MotorY_step_pin;
  motors[1].dir_pin = MotorY_dir_pin;
  motors[1].limit_switch_pin = MotorY_limit_switch_pin;

  motors[2].step_pin = MotorZ_step_pin;
  motors[2].dir_pin = MotorZ_dir_pin;
  motors[2].limit_switch_pin = MotorZ_limit_switch_pin;

  motors[3].step_pin = MotorE_step_pin;
  motors[3].dir_pin = MotorE_dir_pin;
  motors[3].limit_switch_pin = MotorE_limit_switch_pin;

  motors[4].step_pin = MotorB_step_pin;
  motors[4].dir_pin = MotorB_dir_pin;
  motors[4].limit_switch_pin = MotorB_limit_switch_pin;

  int i;
  for (i = 0; i < NUM_AXIES; ++i) {
    // set the motor pin & scale
    pinMode(motors[i].step_pin, OUTPUT);
    pinMode(motors[i].dir_pin, OUTPUT);
    pinMode(motors[i].enable_pin, OUTPUT);
  }
}


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------

int finales()
{ 
    if (!digitalRead(MotorX_limit_switch_pin) && homing) 
    {
     bandera=1;
     delay(10);
     //modo=0;
     //Serial.println("X-stop");
     //motor_disable();
    } 
    if (!digitalRead(MotorY_limit_switch_pin)&& homing) 
    {
     bandera=2;
     //modo=0;
     delay(10);
     //Serial.println("Y-stop"); 
     //motor_disable();     
    }  
    if (!digitalRead(MotorZ_limit_switch_pin)&& homing) 
    {
    //Serial.println("Z-stop"); 
     bandera=3;
     //modo=0;
     delay(10);
     Serial.println("Z-stop"); 
     //motor_disable();    
    }    
    // if (!digitalRead(RESUME)) 
    // {
    // //Serial.println("Resume"); 
    // bandera=4;
    // modo=0;
    // delay(10);
    // //motor_disable();
    // }
    // else if(bandera==4 && digitalRead(RESUME))
    // {
    //   //motor_enable();
    //   //bandera=0;
    //   homing=false;
    // }
    return(bandera);
}

/**
   delay for the appropriate number of microseconds
   @input ms how many milliseconds to wait
*/
void pause(long ms) {
  delay(ms / 1000);
  delayMicroseconds(ms % 1000); // delayMicroseconds doesn't work for values > ~16k.
}


/**
   Set the feedrate (speed motors will move)
   @input nfr the new speed in steps/second
*/
void feedrate(float nfr) {
  nfr = nfr * STEPS_PER_MM / 60;
  if (fr == nfr) return; // same as last time?  quit now.

  if (nfr > MAX_FEEDRATE || nfr < MIN_FEEDRATE) { // don't allow crazy feed rates
//    Serial1.print(F("New feedrate must be greater than "));
//    Serial1.print(MIN_FEEDRATE);
//    Serial1.print(F("steps/s and less than "));
//    Serial1.print(MAX_FEEDRATE);
//    Serial1.println(F("steps/s."));
    return;
  }
  step_delay = MAX_FEEDRATE / nfr;
  fr = nfr;
}


/**
   Set the logical position
   @input npx new position x
   @input npy new position y
*/
void position(float npx, float npy, float npz, float npe, float npb) {
  // here is a good place to add sanity tests
  px = npx;
  py = npy;
  pz = npz;
  pe = npe;
  pb = npb;
}

/**
   Supports movement with both styles of Motor Shield
   @input newx the destination x position
   @input newy the destination y position
 **/
void onestep(int motor) {
#ifdef VERBOSE
  char *letter = "XYZE";
  Serial.print(letter[motor]);
#endif

  digitalWrite(motors[motor].step_pin, HIGH);
  delay(1);
  digitalWrite(motors[motor].step_pin, LOW);
  delay(1);
}

/**
   write a string followed by a float to the serial line.  Convenient for debugging.
   @input code the string.
   @input val the float.
   void output(char *code, float val) {
*/
void output(char code, float val) {
  Serial1.print(code);
  Serial1.print(val);
  Serial1.print(" ");
}

/**
   print the current position, feedrate, and absolute mode.
*/
void where() {
  output("X", px);
  output("Y", py);
  output("Z", pz);
  output("E", pe);
  output("B", pb);
  output("F", fr / STEPS_PER_MM * 60);
//  Serial1.println(mode_abs ? "ABS" : "REL");
}

/**
   Uses bresenham's line algorithm to move both motors
   @input newx the destination x position
   @input newy the destination y position
 **/
void line(float newx, float newy, float newz, float newe, float newb) {
  a[0].delta = (newx - px) * STEPS_PER_MM;
  a[1].delta = (newy - py) * STEPS_PER_MM;
  a[2].delta = (newz - pz) * STEPS_PER_MM;
  a[3].delta = (newe - pe);
  a[4].delta = (newb - pb);
  //a[3].delta = (newe-pe)*STEPS_PER_MM;
  long i, j, maxsteps = 0;

  Serial.print(newx);
  Serial.print(", ");
  Serial.println(px);

  for (i = 0; i < NUM_AXIES; ++i) {
    a[i].absdelta = abs(a[i].delta);
    a[i].over = 0;
    if ( maxsteps < a[i].absdelta ) maxsteps = a[i].absdelta;
    // set the direction once per movement
    digitalWrite(motors[i].dir_pin, a[i].delta > 0 ? HIGH : LOW);
  }

  long dt = MAX_FEEDRATE / 5000;
  long accel = 1;
  long steps_to_accel = dt - step_delay;
  if (steps_to_accel > maxsteps / 2 )
    steps_to_accel = maxsteps / 2;

  long steps_to_decel = maxsteps - steps_to_accel;

//  Serial1.print("START ");
//  Serial1.println(dt);
//  Serial1.print("STOP ");
//  Serial1.println(step_delay);
//
//  Serial1.print("accel until ");
//  Serial1.println(steps_to_accel);
//  Serial1.print("decel after ");
//  Serial1.println(steps_to_decel);
//  Serial1.print("torango ");
//  Serial1.println(maxsteps);
#ifdef VERBOSE
  Serial.println(F("Start >"));
#endif

  for ( i = 0; i < maxsteps; ++i ) {
    for (j = 0; j < NUM_AXIES; ++j) {
      a[j].over += a[j].absdelta;
      if (a[j].over >= maxsteps) {
        a[j].over -= maxsteps;
        //if(finales()>0)
        //{
         //modo=0;
         //Serial.print("EMG");
         //a[j].over = 10;
         //j=NUM_AXIES*100;
         //i=maxsteps;
         //Serial.println("ELSE");
         //break;
        //}
        //else
        //{
          //Serial.print("MOV");
        digitalWrite(motors[j].step_pin, HIGH);
        delay(1);
        digitalWrite(motors[j].step_pin, LOW);
        delay(1);
        //Serial.println("ELSE");
        //}
      }
    
      
    }

    if (i < steps_to_accel) {
      dt -= accel;
    }
    if (i >= steps_to_decel) {
      dt += accel;
    }
    delayMicroseconds(dt);
  }

#ifdef VERBOSE
  Serial.println(F("< Done."));
#endif

  position(newx, newy, newz, newe, newb);
  Serial.println("Posicion Final");
  where();
}


// returns angle of dy/dx as a value from 0...2PI
static float atan3(float dy, float dx) {
  float a = atan2(dy, dx);
  if (a < 0) a = (PI * 2.0) + a;
  return a;
}






/**
   prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
*/
void ready() {
  sofar = 0; // clear input buffer
//  Serial1.print(F(">"));  // signal ready to receive input
}


void CNC::home()
{
  int maxstepsY = 10000;
  for (int iy = 0; iy < maxstepsY; iy++) {
    digitalWrite(motors[1].dir_pin,  LOW);
    digitalWrite(motors[1].step_pin, HIGH);
    delay(1);
    digitalWrite(motors[1].step_pin, LOW);
    delay(1);
    if (!digitalRead(MotorY_limit_switch_pin))
    {
      iy = maxstepsY + 1;
    }
  }
  digitalWrite(motors[1].dir_pin,  LOW);
  long maxstepsX = 150000;
  for (long ix = 0; ix < maxstepsX; ix++) {
    digitalWrite(motors[0].dir_pin,  LOW);
    digitalWrite(motors[0].step_pin, HIGH);
    delay(1);
    digitalWrite(motors[0].step_pin, LOW);
    delay(1);
    if (!digitalRead(MotorX_limit_switch_pin))
    {
      ix = maxstepsX + 1;
    }
  }

  int maxstepsZ = 10000;
  for (int iz = 0; iz < maxstepsZ; iz++) {
    digitalWrite(motors[2].dir_pin, LOW);
    digitalWrite(motors[2].step_pin, HIGH);
    delay(1);
    digitalWrite(motors[2].step_pin, LOW);
    delay(1);
    if (!digitalRead(MotorZ_limit_switch_pin))
    {
      iz = maxstepsZ + 1;
    }
  }
  homing = false;
  bandera = 0;
  line(0, 0, 5, 0, 0);
  delay(200);
  line(5, 0, 5, 0,0);
  delay(200);
  line(5, 10, 5, 0,0);
  delay(200);
  homing = true;
  position(0, 0, 0, 0,0);
}


void Altura()
{
//   //int numberOfParameters = uduino.getNumberOfParameters();
//   if (numberOfParameters == 1) {
//     int val1 = uduino.charToInt(uduino.getParameter(0));
//     line(px, py, (mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), pe);
//     //Write_to_HERO();
//     delay(10);
//   }
}

void Ancho()
{
//   int numberOfParameters = uduino.getNumberOfParameters();
//   if (numberOfParameters == 1) {
//     int val1 = uduino.charToInt(uduino.getParameter(0));
//     line((mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), py, pz, pe);
//     //Write_to_HERO();
//     delay(10);
//   }
}

void Largo()
{
//   int numberOfParameters = uduino.getNumberOfParameters();
//   if (numberOfParameters == 1) {
//     int val1 = uduino.charToInt(uduino.getParameter(0));
//     line(px, (mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), pz, pe);
//     //Write_to_HERO();
//     delay(10);
//   }
}

void CNC::linear(int l_x, int l_y, int l_z, int l_e, int l_b)
{
  line(l_x, l_y, l_z, l_e,l_b);
}

void CNC::VelMotors(float vel)
{
  feedrate(vel);
}

// Ejemplo de proceso
void Pulido() {
  int te = 0;
  int deltha = 0;
  int dir = 0;
  line(0, 0, pz, dir * 1080 - deltha * dir,0); //40
  delay(te);
  line(0, 0, pz, 0,0);
  delay(te);
  line(0, 0, pz, dir * -2430 + deltha * dir,0); //-90
  delay(te);
  line(0, 0, pz, 0,0);
  delay(te);
}

void Controlador()
{
  //modo = 3;
  switch (modo) {
    case 0:    // Controlador Apagado
      Serial.println("Seleccion Proceso");
      //ControladoR = 0;
      break;
    case 1:
      Serial.println("Proceso de Corte");
      //ControladoR = 1;
      //Corte();
      homing = true;
      break;
    case 2:
      Serial.println("Proceso de Lijado");
      //ControladoR = 1;
      //Lijado();
      homing = true;
      break;
    case 3:
      Serial.println("Proceso de Pulido");
      //ControladoR = 1;
      Pulido();
      homing = true;
      break;
     case 4:
      Serial.println("Proceso de Inspeccion");
      //ControladoR = 1;
      //Inspeccion();
      homing = true;
      break;
  }
}

void CNC::run()
{
  //
//  Serial.print(digitalRead(MotorX_limit_switch_pin));
//  Serial.print("   ");
//  Serial.print(digitalRead(MotorY_limit_switch_pin));
//  Serial.print("   ");
//  Serial.print(digitalRead(MotorZ_limit_switch_pin));
    //Serial.print("   ");
    //Serial.println(bandera);
    finales();
  //uduino.update(); receptor de mensajes desde serie
  delay(100);
//   if (uduino.isConnected())
//   {
//    if (ControladoR)
//     { 
//       Controlador();
//       //bandera=0;
//     }
//   }
}


