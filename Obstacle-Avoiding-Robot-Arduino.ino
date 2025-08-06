#include <Servo.h>

// Ultrasonic
const int trigPin = A1;
const int echoPin = A2;

// Servo
Servo myServo;
const int servoPin = A0;

// Motor 1 (L293D #1)
const int ENA1 = 3;
const int IN1_1 = 2;
const int IN1_2 = 4;

// Motor 2 (L293D #1)
const int ENB1 = 5;
const int IN2_1 = 6;
const int IN2_2 = 7;

// Motor 3 (L293D #2)
const int ENA2 = 9;
const int IN3_1 = 8;
const int IN3_2 = 10;

// Motor 4 (L293D #2)
const int ENB2 = 11;
const int IN4_1 = 12;
const int IN4_2 = 13;

long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return pulseIn(echoPin, HIGH);
}

void setup() {
  Serial.begin(9600);

  // إعداد المخارج
  int motorPins[] = {ENA1, IN1_1, IN1_2, ENB1, IN2_1, IN2_2,
                     ENA2, IN3_1, IN3_2, ENB2, IN4_1, IN4_2};
  for (int i = 0; i < 12; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  myServo.attach(servoPin);
  myServo.write(0);
}

void moveAllForward() {
  digitalWrite(IN1_1, HIGH); digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_1, HIGH); digitalWrite(IN2_2, LOW);
  digitalWrite(IN3_1, HIGH); digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_1, HIGH); digitalWrite(IN4_2, LOW);
  analogWrite(ENA1, 200); analogWrite(ENB1, 200);
  analogWrite(ENA2, 200); analogWrite(ENB2, 200);
}

void moveAllBackward() {
  digitalWrite(IN1_1, LOW); digitalWrite(IN1_2, HIGH);
  digitalWrite(IN2_1, LOW); digitalWrite(IN2_2, HIGH);
  digitalWrite(IN3_1, LOW); digitalWrite(IN3_2, HIGH);
  digitalWrite(IN4_1, LOW); digitalWrite(IN4_2, HIGH);
  analogWrite(ENA1, 200); analogWrite(ENB1, 200);
  analogWrite(ENA2, 200); analogWrite(ENB2, 200);
}

void turnLeft() {
  digitalWrite(IN1_1, LOW); digitalWrite(IN1_2, HIGH);
  digitalWrite(IN2_1, LOW); digitalWrite(IN2_2, HIGH);
  digitalWrite(IN3_1, HIGH); digitalWrite(IN3_2, LOW);
  digitalWrite(IN4_1, HIGH); digitalWrite(IN4_2, LOW);
}

void turnRight() {
  digitalWrite(IN1_1, HIGH); digitalWrite(IN1_2, LOW);
  digitalWrite(IN2_1, HIGH); digitalWrite(IN2_2, LOW);
  digitalWrite(IN3_1, LOW); digitalWrite(IN3_2, HIGH);
  digitalWrite(IN4_1, LOW); digitalWrite(IN4_2, HIGH);
}

void stopAllMotors() {
  analogWrite(ENA1, 0); analogWrite(ENB1, 0);
  analogWrite(ENA2, 0); analogWrite(ENB2, 0);
}

void loop() {
  moveAllForward();
  checkObstacleDuring(30000);

  moveAllBackward();
  checkObstacleDuring(60000);

  for (int i = 0; i < 6; i++) {
    turnLeft();  checkObstacleDuring(5000);
    turnRight(); checkObstacleDuring(5000);
  }
}

void checkObstacleDuring(unsigned long duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    long distTime = readUltrasonicDistance(trigPin, echoPin);
    float cm = distTime * 0.01723;
    Serial.print("Distance: "); Serial.println(cm);

    if (cm < 10.0) {
      stopAllMotors();
      Serial.println("Obstacle Detected!");

      myServo.write(90);
      delay(1000);
      myServo.write(0);

      moveAllBackward();
      delay(2000);
      break;
    }

    delay(100);
  }
}
