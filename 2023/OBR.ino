#include <QTRSensors.h>
#include <NewPing.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

Adafruit_TCS34725 tcs1 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
Adafruit_TCS34725 tcs2 = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

int sensorCounter = 0;

#define motorA1 2
#define motorB1 3
#define motorA2 4
#define motorB2 5

#define NUM_SENSORS 8
#define TIMEOUT 4
#define EMITTER_PIN 13

QTRSensors qtra;

const uint8_t SensorCount = 8;
uint16_t sensorValues[SensorCount];

#define TRIGGER_PIN 10
#define ECHO_PIN 11
#define MAX_DISTANCE 400

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

void setup()
{
    Serial.begin(9600);

    pinMode(motorA1, OUTPUT);
    pinMode(motorB1, OUTPUT);
    pinMode(motorA2, OUTPUT);
    pinMode(motorB2, OUTPUT);

    qtra.setTypeAnalog();
    qtra.setSensorPins((const uint8_t[]){A2, A4, A6, A7, A1, A0, A5, A3}, SensorCount);
    qtra.setEmitterPin(13);

    pinMode(EMITTER_PIN, OUTPUT);
    digitalWrite(EMITTER_PIN, HIGH);

    for (int i = 0; i < 100; i++)
    {
        delay(5);
        qtra.calibrate();

        Serial.println(i);
    }

    Serial.println("Calibração Linha Concluída");

    switchSensor();
    Serial.println("Sensor de Cor 01");

    if (tcs1.begin())
    {
        Serial.println("Sensor 01 Encontrado");
    }
    else
    {
        Serial.println("TCS34725 01 não encontrado...");
    }

    delay(1000);

    switchSensor();
    Serial.println("Sensor de Cor 02");

    if (tcs2.begin())
    {
        Serial.println("Sensor 02 Encontrado");
    }
    else
    {
        Serial.println("TCS34725 02 não encontrado...");
    }

    delay(1000);

    Serial.println("Calibração Cor Concluída");
}

int error = 0;
int lastError = 0;
float KP = 0.7;
float KD = 0.7;
int motorSpeed = 0;
int position = 0;
int m1Speed = 0;
int m2Speed = 0;
int M1 = 50;
int M2 = 50;
int M1max = 700;
int M2max = 700;
int M1min = 0;
int M2min = 0;

void loop()
{
    uint16_t position = qtra.readLineBlack(sensorValues);

    bool allValuesBiggerThan950 = true;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensorValues[i] <= 950)
        {
            allValuesBiggerThan950 = false;
            break;
        }
    }

    if (allValuesBiggerThan950)
    {
        analogWrite(motorB1, 0);
        analogWrite(motorA2, 0);

        switchSensor();
        bool temVerdeNaDireita = printCor1();
        switchSensor();
        bool temVerdeNaEsquerda = printCor2();

        if (temVerdeNaDireita)
        {
            analogWrite(motorA1, M1max / 2);
            analogWrite(motorB2, M2max / 2);
            delay(750);
            analogWrite(motorA1, 0);
            analogWrite(motorB2, 0);
            turnRight();
        }
        else if (temVerdeNaEsquerda)
        {
            analogWrite(motorA1, M1max / 2);
            analogWrite(motorB2, M2max / 2);
            delay(750);
            analogWrite(motorA1, 0);
            analogWrite(motorB2, 0);
            turnLeft();
        }
        else
        {
            analogWrite(motorB1, M1max);
            analogWrite(motorA2, M2max);
        }
    }
    else
    {
        if ((position >= 3450 && position <= 3550) || position == 0)
        {
            analogWrite(motorB1, M1max);
            analogWrite(motorA2, M2max);
        }
        else
        {
            error = position - 3500;

            motorSpeed = KP * error + KD * (error - lastError);
            lastError = error;

            m1Speed = M2 + motorSpeed;
            m2Speed = M1 - motorSpeed;

            if (m1Speed < M1min)
                m1Speed = M1min;
            if (m1Speed > M1max)
                m1Speed = M1max;

            if (m2Speed < M2min)
                m2Speed = M2min;
            if (m2Speed > M2max)
                m2Speed = M2max;

            analogWrite(motorB1, m1Speed);
            analogWrite(motorA2, m2Speed);
        }
    }
}
