void UduinoSetUP()
{
  Serial.begin(9600); //1382400 115200 Abrimos la comunicación serie con el PC y establecemos velocidad
#if defined (__AVR_ATmega32U4__) // Leonardo
  while (!Serial) {}
#elif defined(__PIC32MX__)
  delay(1000);
#endif
  uduino.addCommand("az", Altura);
  uduino.addCommand("ax", Ancho);
  uduino.addCommand("ay", Largo);
  uduino.addCommand("h", Home);
  uduino.addCommand("l", Gp);
  uduino.addCommand("m", SelecionaModoNew);
}

void Gp()
{
  delay(10);
  Serial.print("Gp");
  Serial.print(" ");
  Serial.print(px);
  Serial.print(" ");
  Serial.print(py);
  Serial.print(" ");
  Serial.print(pz);
  Serial.print(" "); //<- Todo : Change that with Uduino delimiter
  Serial.println(pe);
}

bool Power = 1;
void EncenderMaquina() {
  Power = !Power;
  //digitalWrite(verde, Power);
}

void EnableMotors() {
  //v1 = !v1;
  //Write_to_HERO();
}

void SelecionaModoNew()
{
  int numberOfParameters = uduino.getNumberOfParameters();
  if (numberOfParameters == 4) {
    int val1 = uduino.charToInt(uduino.getParameter(0));
    int val2 = uduino.charToInt(uduino.getParameter(1));
    int val3 = uduino.charToInt(uduino.getParameter(2));
    int val4 = uduino.charToInt(uduino.getParameter(3));
    modo = val1;
    dir = val2;
    vel = val3;
    rango = val4;
    delay(10);
  }
  if (vel == 4)
    vel = 1000000;
  if (vel == 3)
    vel = 10000;
  if (vel == 2)
    vel = 1000;
  if (vel == 1)
    vel = 10;
  feedrate(vel);
  ControladoR = !ControladoR;
  delay(10);
}

void Altura()
{
  int numberOfParameters = uduino.getNumberOfParameters();
  if (numberOfParameters == 1) {
    int val1 = uduino.charToInt(uduino.getParameter(0));
    line(px, py, (mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), pe);
    //Write_to_HERO();
    delay(10);
  }
}

void Ancho()
{
  int numberOfParameters = uduino.getNumberOfParameters();
  if (numberOfParameters == 1) {
    int val1 = uduino.charToInt(uduino.getParameter(0));
    line((mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), py, pz, pe);
    //Write_to_HERO();
    delay(10);
  }
}

void Largo()
{
  int numberOfParameters = uduino.getNumberOfParameters();
  if (numberOfParameters == 1) {
    int val1 = uduino.charToInt(uduino.getParameter(0));
    line(px, (mode_abs ? val1 : 0) + (mode_abs ? 0 : val1), pz, pe);
    //Write_to_HERO();
    delay(10);
  }
}

void Home()
{
  delay(10);
  //  Serial.print("Gp");
  //  Serial.print(" ");
  //  Serial.print(px);
  //  Serial.print(" ");
  //  Serial.print(py);
  //  Serial.print(" ");
  //  Serial.print(pz);
  //  Serial.print(" "); //<- Todo : Change that with Uduino delimiter
  //  Serial.println(pe);
  motor_enable();
  int maxstepsY = 10000;
  for (int iy = 0; iy < maxstepsY; iy++) {
    digitalWrite(motors[1].dir_pin,  HIGH);
    digitalWrite(motors[1].step_pin, HIGH);
    delay(1);
    digitalWrite(motors[1].step_pin, LOW);
    delay(1);
    if (!digitalRead(Y_ENDSTOP))
    {
      iy = maxstepsY + 1;
    }
  }
  digitalWrite(motors[1].dir_pin,  LOW);
  int maxstepsX = 20000;
  for (int ix = 0; ix < maxstepsX; ix++) {
    digitalWrite(motors[0].dir_pin,  LOW);
    digitalWrite(motors[0].step_pin, HIGH);
    delay(1);
    digitalWrite(motors[0].step_pin, LOW);
    delay(1);
    if (!digitalRead(X_ENDSTOP))
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
    if (!digitalRead(Z_ENDSTOP))
    {
      iz = maxstepsZ + 1;
    }
  }
  homing = false;
  bandera = 0;
  line(0, 0, 10, 0);
  delay(100);
  line(85, 0, 10, 0);
  delay(100);
  line(85, -5, 10, 0);
  delay(100);
  homing = true;
  position(0, 0, 0, 0);
}
