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
#define NUM_AXIES            (4)

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

float px, py, pz, pe; // position

// settings
char mode_abs = 1; // absolute mode?

long line_number = 0;


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------


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
void position(float npx, float npy, float npz, float npe) {
  // here is a good place to add sanity tests
  px = npx;
  py = npy;
  pz = npz;
  pe = npe;
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
   Uses bresenham's line algorithm to move both motors
   @input newx the destination x position
   @input newy the destination y position
 **/
void line(float newx, float newy, float newz, float newe) {
  a[0].delta = (newx - px) * STEPS_PER_MM;
  a[1].delta = (newy - py) * STEPS_PER_MM;
  a[2].delta = (newz - pz) * STEPS_PER_MM;
  a[3].delta = (newe - pe);
  //a[3].delta = (newe-pe)*STEPS_PER_MM;
  long i, j, maxsteps = 0;

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
        if(finales()>0)
        {
         modo=0;
         //Serial.print("EMG");
         //a[j].over = 10;
         //j=NUM_AXIES*100;
         //i=maxsteps;
         break;
        }
        else
        {
          //Serial.print("MOV");
        digitalWrite(motors[j].step_pin, HIGH);
        delay(1);
        digitalWrite(motors[j].step_pin, LOW);
        delay(1);
        }
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

  position(newx, newy, newz, newe);

  where();
}


// returns angle of dy/dx as a value from 0...2PI
static float atan3(float dy, float dx) {
  float a = atan2(dy, dx);
  if (a < 0) a = (PI * 2.0) + a;
  return a;
}



/**
   write a string followed by a float to the serial line.  Convenient for debugging.
   @input code the string.
   @input val the float.
*/
void output(char *code, float val) {
//  Serial1.print(code);
//  Serial1.print(val);
//  Serial1.print(" ");
}

/**
   print the current position, feedrate, and absolute mode.
*/
void where() {
  output("X", px);
  output("Y", py);
  output("Z", pz);
  output("E", pe);
  output("F", fr / STEPS_PER_MM * 60);
//  Serial1.println(mode_abs ? "ABS" : "REL");
}

/**
   prepares the input buffer to receive a new message and tells the serial connected device it is ready for more.
*/
void ready() {
  sofar = 0; // clear input buffer
//  Serial1.print(F(">"));  // signal ready to receive input
}


/**
   set up the pins for each motor
   Pins fits a CNCshieldV3.xx
*/
void motor_setup() {
  motors[0].step_pin = 2;
  motors[0].dir_pin = 5;
  motors[0].enable_pin = 8;
  motors[0].limit_switch_pin = 9;

  motors[1].step_pin = 4;
  motors[1].dir_pin = 7;
  motors[1].enable_pin = 8;
  motors[1].limit_switch_pin = 10;

  motors[2].step_pin = 12;
  motors[2].dir_pin = 13;
  motors[2].enable_pin = 8;
  motors[2].limit_switch_pin = 11;

  motors[3].step_pin = 3;
  motors[3].dir_pin = 6;
  motors[3].enable_pin = 8;
  motors[3].limit_switch_pin = 11;

  int i;
  for (i = 0; i < NUM_AXIES; ++i) {
    // set the motor pin & scale
    pinMode(motors[i].step_pin, OUTPUT);
    pinMode(motors[i].dir_pin, OUTPUT);
    pinMode(motors[i].enable_pin, OUTPUT);
  }
}


void motor_enable() {
  int i;
  for (i = 0; i < NUM_AXIES; ++i) {
    digitalWrite(motors[i].enable_pin, LOW);
  }
}


void motor_disable() {
  int i;
  for (i = 0; i < NUM_AXIES; ++i) {
    digitalWrite(motors[i].enable_pin, HIGH);
  }
}
