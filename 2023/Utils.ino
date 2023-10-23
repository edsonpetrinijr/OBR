bool printCor1()
{
  uint16_t clearcol, red, green, blue;
  float average, r, g, b;
  tcs1.getRawData(&red, &green, &blue, &clearcol);

  average = (red + green + blue) / 3;

  r = red / average;
  g = green / average;
  b = blue / average;

  if ((r < 1) && (g > 1.1) && (b < 1))
  {
    return true;
  }
  else
  {
    return false;
  }
}

bool printCor2()
{
  uint16_t clearcol, red, green, blue;
  float average, r, g, b;
  tcs2.getRawData(&red, &green, &blue, &clearcol);

  average = (red + green + blue) / 3;

  r = red / average;
  g = green / average;
  b = blue / average;

  if ((r < 1) && (g > 1.1) && (b < 1))
  {
    return true;
  }
  else
  {
    return false;
  }
}

void switchSensor()
{
  if (sensorCounter == 0)
  {
    Wire.beginTransmission(0x70);
    Wire.write(1 << 0);
    Wire.endTransmission();

    sensorCounter = 1;
  }
  else
  {
    Wire.beginTransmission(0x70);
    Wire.write(1 << 1);
    Wire.endTransmission();

    sensorCounter = 0;
  }
}

void desviarObstaculo()
{
  if (sonar.ping_cm() < 15)
  {
    turnLeft();
    analogWrite(motorB1, M1max);
    analogWrite(motorA2, M2max);

    delay(3000);
    analogWrite(motorB1, 0);
    analogWrite(motorA2, 0);
    turnRight();
    analogWrite(motorB1, M1max);
    analogWrite(motorA2, M2max);

    delay(3000);
    analogWrite(motorB1, 0);
    analogWrite(motorA2, 0);

    turnRight();

    analogWrite(motorB1, M1max);
    analogWrite(motorA2, M2max);

    delay(3000);
    analogWrite(motorB1, 0);
    analogWrite(motorA2, 0);

    turnLeft();
  }
}

void turnLeft()
{
  analogWrite(motorA2, 1023);

  delay(2500);

  analogWrite(motorA2, 0);
}

void turnRight()
{
  analogWrite(motorB1, 1023);

  delay(2250);

  analogWrite(motorB1, 0);
}