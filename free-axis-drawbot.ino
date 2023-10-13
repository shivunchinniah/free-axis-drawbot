#include "Cmd.h"
#include "EncoderEvery.h"
#include "L298M.h"

#define MOTOR_A1 5
#define MOTOR_A2 9

#define MOTOR_B1 6
#define MOTOR_B2 10

#define MOTOR_A_ENC_0 2
#define MOTOR_A_ENC_90 7

#define MOTOR_B_ENC_0 8
#define MOTOR_B_ENC_90 3


#define A_TICKS_ROT 2058 // 12 * 171
#define B_TICKS_ROT 2736 // 16 * 171

// Rolling buffer for available cpu time
RollingAverage<unsigned long> cpuStats(128);
unsigned long previous = micros();

inline void updateStats(){
  unsigned long now = micros();
  cpuStats.push(now - previous);
  previous = now;
}

void printStats(){
  Serial.print(cpuStats.avg());
  Serial.print(" us, min: ");
  Serial.print(cpuStats.min());
  Serial.print(" us, max: ");
  Serial.println(cpuStats.max());
}

EncoderEvery encoderA(MOTOR_A_ENC_0, MOTOR_A_ENC_90, 'A', 12);
L298M motorA(MOTOR_A1, MOTOR_A2);

EncoderEvery encoderB(MOTOR_B_ENC_0, MOTOR_B_ENC_90, 'B', 16);
L298M motorB(MOTOR_B1, MOTOR_B2);

#define VERSION 1.4



void setup() {

  Serial.begin(115200);
  cmdInit(115200);

  cmdAdd("v", version);
  cmdAdd("re", readEncoders);
  cmdAdd("rs", resetEncoders);
  cmdAdd("sm", setMotors);
  cmdAdd("stop", stopMotors);
  cmdAdd("cpu", printStats);
}

void loop() {
  cmdPoll();
  updateStats();
}

void version(){
  Serial.println(VERSION);
}


void stopMotors(int argc, char **args) {
  Serial.println("Stop");

  motorA.setHold(false);
  motorA.stop();

  motorB.setHold(false);
  motorB.stop();
}

void setMotors(int argc, char **args) {
  if (argc >= 3) {
    motorA.setVector(String(args[1]).toInt());
    motorB.setVector(String(args[2]).toInt());

    motorA.run();
    motorB.run();

    Serial.print("Motor Vectors: ");
    printlnMatrix(motorA.getVector(), motorB.getVector());
  }
}

void resetEncoders(int argc, char **args) {
  encoderA.write(0l);
  encoderB.write(0l);

  Serial.println("Encoders Reset");
}

void readEncoders(int argc, char **args) {
  Serial.println(micros());
  Serial.println("Encoders:");
  Serial.print("Rot:\t");
  printlnMatrix(rawToRotA(encoderA.read()), rawToRotB(encoderB.read()));
  Serial.print("Raw:\t");
  printlnMatrix(encoderA.read(), encoderB.read());
}



template<typename T>
void printlnMatrix(T a, T b) {
  Serial.print("[");
  Serial.print(a);
  Serial.print(", ");
  Serial.print(b);
  Serial.println("]");
}


double rawToRotA(long raw) {
  return (double)raw / A_TICKS_ROT;
}

double rawToRotB(long raw) {
  return (double)raw / B_TICKS_ROT;
}


