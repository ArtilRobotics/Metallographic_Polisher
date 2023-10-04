void Controlador()
{
  //modo = 3;
  switch (modo) {
    case 0:    // Controlador Apagado
      Serial.println("Seleccion Terapia");
      ControladoR = 0;
      break;
    case 1:
      Serial.println("Flexion Extencion");
      ControladoR = 1;
      FlexionExtension();
      homing = true;
      break;
    case 2:
      Serial.println("Radial Cubuirango");
      ControladoR = 1;
      RadialCubuirango();
      homing = true;
      break;
    case 3:
      Serial.println("Pronacion Supinacion");
      ControladoR = 1;
      PronacionSupinacion();
      homing = true;
      break;
  }
}

void FlexionExtension() {
  int te = 0;
  line(dir * -13.89, -1.22, pz, 0); //vuelve 25
  delay(te);
  if (rango >= 2)
  {
    line(dir * -27.36, -4.82, pz, 0); //vuelve de 50
    delay(te);
    line(dir * -40, -10.72, pz, 0);
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * -51.42, -18.72, pz, 0); //vuelve de 75
    delay(te);
    line(dir * -61.28, -28.58, pz, 0);
    delay(te);
  }
  if (rango >= 4)
  {
    line(dir * -69.28, -40, pz, 0); //Vuelve de 100
    delay(te);
    line(dir * -75.18, -52.64, pz, 0);
    delay(te);
    //loop
    line(dir * -78.78, -66.11, pz, 0); //100
    delay(te);
    line(dir * -75.18, -52.64, pz, 0);
    delay(te);
    line(dir * -69.28, -40, pz, 0);
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * -61.28, -28.58, pz, 0); //75
    delay(te);
    line(dir * -51.42, -18.72, pz, 0);
    delay(te);
  }
  if (rango >= 2)
  {
    line(dir * -40, -10.72, pz, 0); //50
    delay(te);
    line(dir * -27.36, -4.82, pz, 0);
    delay(te);
  }
  line(dir * -13.89, -1.22, pz, 0); //25
  delay(te);
  line(0, -0, pz, 0);
  delay(te);
  line(dir * 13.89, -1.22, pz, 0);
  delay(te);
  if (rango >= 2)
  {
    line(dir * 27.36, -4.82, pz, 0); //50
    delay(te);
    line(dir * 40, -10.72, pz, 0);
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * 51.42, - 18.72, pz, 0); //75
    delay(te);
    line(dir * 61.28, -28.58, pz, 0);
    delay(te);
  }
  if (rango >= 4)
  {
    line(dir * 69.28, - 40, pz, 0); //100
    delay(te);
    line(dir * 75.18, -52.64, pz, 0);
    delay(te);

    //vuel pasos
    line(dir * 69.28, -40, pz, 0); //vuelve del 100
    delay(te);
  }

  if (rango >= 3)
  {
    line(dir * 61.28, -28.58, pz, 0); //vuelve de 75
    delay(te);
    line(dir * 51.42, -18.72, pz, 0);
    delay(te);
  }
  if (rango >= 2)
  {
    line(dir * 40, -10.72, pz, 0); //vuelve de 50
    delay(te);
    line(dir * 27.36, -4.82, pz, 0);
    delay(te);
  }
  line(dir * 13.89, -1.22, pz, 0); //vuelve de 25
  delay(te);
  line(0, -0, pz, 0);
  delay(te);
}

void RadialCubuirango() {
  int te = 0;
  line(dir * 20.71, -2.73, pz, 0);//vuelve 25
  delay(te);
  if (rango >= 2)
  {
    line(dir * 40.00, -10.72, pz, 0);//vuelve 50
    delay(te);
    line(dir * 56.57, -23.43, pz, 0);
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * 69.28, -40.00, pz, 0);//vuelve 75
    delay(te);
    line(dir * 77.27, -59.29, pz, 0);
    delay(te);
  }
  if (rango >= 4)
  {
    line(dir * 80.00, -80, pz, 0);//100
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * 77.27, -59.29, pz, 0);//75
    delay(te);
    line(dir * 69.28, -40.00, pz, 0);
    delay(te);
  }
  if (rango >= 2)
  {
    line(dir * 56.57, -23.43, pz, 0);//50
    delay(te);
    line(dir * 40.00, -10.72, pz, 0);
    delay(te);
  }
  line(dir * 20.71, -2.73, pz, 0);//25
  delay(te);
  line(0.00, 0, pz, 0);
  delay(te);
  line(dir * -20.71, -2.73, pz, 0);
  delay(te);
  if (rango >= 2)
  {
    line(dir * -40.00, -10.72, pz, 0);//50
    delay(te);
    line(dir * -56.57, -23.43, pz, 0);
    delay(te);
  }
  if (rango >= 3)
  {
    line(dir * -69.28, -40.00, pz, 0);//75
    delay(te);
    line(dir * -56.57, -23.43, pz, 0);
    delay(te);
  }
  if (rango >= 4)
  {
    line(dir * -40.00, -10.72, pz, 0);//100
    delay(te);
  }
  line(dir * -20.71, -2.73, pz, 0);//25
  delay(te);
  line(0, 0, pz, 0);
  delay(te);
}

void PronacionSupinacion() {
  int te = 0;
  int deltha = 0;
  if (rango = 4)
    deltha = 0;
  if (rango = 3)
    deltha = 270;
  if (rango = 2)
    deltha = 540;
  if (rango = 1)
    deltha = 810;
  line(0, 0, pz, dir * 1080 - deltha * dir); //40
  delay(te);
  line(0, 0, pz, 0);
  delay(te);
  line(0, 0, pz, dir * -2430 + deltha * dir); //-90
  delay(te);
  line(0, 0, pz, 0);
  delay(te);
}
