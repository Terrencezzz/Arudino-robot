#include <Servo.h>
#include <NewPing.h>
// ensure that the nnewping library by tim eckel is installed
// Tools > Manage Libraries > Search for 'newping' > Install

// MOTOR PIN DEFINITIONS
const int ADirectionPin1 = 3;
const int ADirectionPin2 = 4;
const int AEnablePin = 2;
const int BDirectionPin1 = 5;
const int BDirectionPin2 = 6;
const int BEnablePin = 7;

const int echoPin = 10;
const int trigPin = 11;
const int servoPin = 9;

const int farRight = -80; // let the servo scan right
const int farLeft = 80;   // let the servo scan left
const int middle = 0;     // let the servo scan middle

double centimeterValue = 0;

double timeFor1Cm = 0;

double distanceRight = 6;
double distanceMiddle = 6;
double distanceLeft = 6;


Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

// ULTRASONIC CONTROL

#define ITERATIONS 5     // Number of iterations.
#define TRIGGER_PIN 11   // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN 10      // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).


NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);


void servoAngle(double angle)
{
  // applies offset
  int processedAngle = angle + 97;
  myservo.write(processedAngle);
  delay(100);
}

void goStraight() {
  digitalWrite(ADirectionPin1, HIGH);
  digitalWrite(ADirectionPin2, LOW);

  analogWrite(BDirectionPin1, 200);
  analogWrite(BDirectionPin2, 0);

  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  delay(600);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}

void turnLeft() {
  digitalWrite(ADirectionPin1, LOW);
  digitalWrite(ADirectionPin2, HIGH);

  digitalWrite(BDirectionPin1, HIGH);
  digitalWrite(BDirectionPin2, LOW);

  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  delay(150);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}

void turnRight() {
  digitalWrite(ADirectionPin1, HIGH);
  digitalWrite(ADirectionPin2, LOW);

  digitalWrite(BDirectionPin1, LOW);
  digitalWrite(BDirectionPin2, HIGH);

  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  delay(800);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}


void setup()
{
  myservo.attach(9);
  pinMode(ADirectionPin1, OUTPUT);
  pinMode(ADirectionPin2, OUTPUT);
  pinMode(AEnablePin, OUTPUT);
  pinMode(BDirectionPin1, OUTPUT);
  pinMode(BDirectionPin2, OUTPUT);
  pinMode(BEnablePin, OUTPUT);
}

double getDistance() {
  double total = 0;
  int n = 0;
  for(int i = 0; i < 100; i++) {
    double distance = sonar.ping_cm();
    if (distance > 0.5) {
      n++;
      total += distance;
    }
  }
  return total / n;
}

void loop() {

  servoAngle(farRight);
  delay(100);
  distanceRight = getDistance();

  servoAngle(middle);
  delay(100);
  distanceMiddle = getDistance();


  if(distanceRight > 6 && distanceMiddle > 6) {
    goStraight();
  }
  else if(distanceRight > 14 || distanceRight < 0.5) {
    goStraight();
    turnRight();
  }
  else {
    turnLeft();
    
  }
}