#include <QTRSensors.h>

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

    Serial.println("Calibração Concluída");
}

int error = 0;
int lastError = 0;
float KP = -1;
float KD = 1;
int motorSpeed = 0;
int position = 0;
int m1Speed = 0;
int m2Speed = 0;
int M1 = 50;
int M2 = 50;
int M1max = 1023;
int M2max = 1023;
int M1min = 100;
int M2min = 100;

void loop()
{
    uint16_t position = qtra.readLineBlack(sensorValues);

    error = position - 3500;

    motorSpeed = KP * error + KD * (error - lastError);
    lastError = error;

    m1Speed = M1 - motorSpeed;
    m2Speed = M2 + motorSpeed;

    if (m1Speed < M1min)
        m1Speed = M1min;
    if (m2Speed < M2min)
        m2Speed = M2min;
    if (m1Speed > M1max)
        m1Speed = M1max;
    if (m2Speed > M2max)
        m2Speed = M2max;

    analogWrite(motorB1, m1Speed);
    analogWrite(motorA2, m2Speed);
}
