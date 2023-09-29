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

#define A_TICKS_ROT 2012
#define B_TICKS_ROT 2680

EncoderEvery encoderA(MOTOR_A_ENC_0, MOTOR_A_ENC_90, 'A');
L298M motorA(MOTOR_A1, MOTOR_A2);

EncoderEvery encoderB(MOTOR_B_ENC_0, MOTOR_B_ENC_90, 'B');
L298M motorB(MOTOR_B1, MOTOR_B2);

// Arrays for data logging

#define LOG_SIZE 400

#define VERSION 1.2

// 4 bytes per ulong
unsigned long time_stamp[LOG_SIZE];
unsigned long a_log[LOG_SIZE];
unsigned long b_log[LOG_SIZE];

unsigned int log_idx = 0;
bool logging = false;

long olda = 0;
long oldb = 0;

// 12 bytes per data entry


void setup() {

  Serial.begin(115200);
  cmdInit(115200);

  cmdAdd("v", version);
  cmdAdd("re", readEncoders);
  cmdAdd("rs", resetEncoders);
  cmdAdd("sm", setMotors);
  cmdAdd("stop", stopMotors);
  cmdAdd("log", startLog);
}

void loop() {
  cmdPoll();

  if (logging) {
    if(log_idx < LOG_SIZE) {
      captureLog();
    } else {
      logging = false;
      log_idx = 0;
      printLog();
      stopMotors(0, nullptr); 
    }
    
  }
}

void version(){
  Serial.println(VERSION);
}

void captureLog() {

  if (olda != encoderA.read() | oldb != encoderB.read()) {

    time_stamp[log_idx] = micros();
    a_log[log_idx] = encoderA.dt();
    b_log[log_idx] = encoderA.dt();

    log_idx++;
    olda = encoderA.read();
    oldb = encoderB.read();
  }
}

void printLog() {
  Serial.println("timestamp,a,b");
  for (unsigned int i = 0; i < LOG_SIZE; i++) {
    Serial.print(time_stamp[i]);
    Serial.print(",");
    Serial.print(a_log[i]);
    Serial.print(",");
    Serial.println(b_log[i]);
  }
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

void startLog(int argc, char **args) {
  if (argc >= 2) {
    motorA.setVector(String(args[1]).toInt());
    motorB.setVector(String(args[1]).toInt());
    motorA.run();
    motorB.run();
    logging = true;
  } else {
    Serial.println("Invalid setpoint");
  }
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



// // Print "hello world" when called from the command line.
// //
// // Usage:
// // hello
// void hello(int arg_cnt, char **args)
// {
//   Serial.println("Hello world.");
// }

// // Display the contents of the args string array.
// //
// // Usage:
// // args 12 34 56 hello gothic baby
// //
// // Will display the contents of the args array as a list of strings
// // Output:
// // Arg 0: args
// // Arg 1: 12
// // Arg 2: 34
// // Arg 3: 56
// // Arg 4: hello
// // Arg 5: gothic
// // Arg 6: baby
// void arg_display(int arg_cnt, char **args)
// {
//   for (int i=0; i<arg_cnt; i++)
//   {
//     Serial.print("Arg ");
//     Serial.print(i);
//     Serial.print(": ");
//     Serial.println(args[i]);
//   }
// }
