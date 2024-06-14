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

const int farRight = -90; // let the servo scan right
const int farLeft = 90;   // let the servo scan left
const int middle = 0;     // let the servo scan middle

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

// ULTRASONIC CONTROL

Servo myservo; // NewPing setup of pins and maximum distance.

double distanceFromObjectIntital = 0;
double distanceFromObjectFinal = 0;
int numberOfCalibrationSteps = 3;

// PHYSICAL CONSTANTS
#define PI 3.1415926535897932384626433832795

const double wheelDiameter = 70 * PI; //~219.911mm
const double wheelDistranceInner = 100;
const double wheelThickness = 8;

// MOTOR CONTROL SPEED VALUES
double BSpeed = 215;

float duration;
float distance;

double degreeValue = 1;
double centimeterValue = 1;

bool didMove = false;
double rightSide = 0;
double frontSide = 0;
double leftSide = 0;
double backSide = 0;

// This part contains the logic for robot movement and servo spin, we can call each
// function to implement the behaviour.

void goStraight() {
  //motor a -> the slow one
  digitalWrite(AEnablePin, LOW);
  analogWrite(ADirectionPin1, 225);
  analogWrite(ADirectionPin2, 0);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(AEnablePin, HIGH);

  //motor b -> the fast one

  digitalWrite(BEnablePin, LOW);
  digitalWrite(BDirectionPin1, HIGH);
  digitalWrite(BDirectionPin2, LOW);
  digitalWrite(BEnablePin, HIGH);

  delay(3600 / 21 * 18);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}

void turn(double angle)
{
  Serial.print("---- TURN ----\nAngle:");
  Serial.println(angle);
  Serial.print("Turn Duration: ");
  Serial.print(degreeValue * angle);
  Serial.println("ms");

  // shutdown motors
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);

  // set speeds
  // motor a -> the slow one
  digitalWrite(ADirectionPin1, angle > 0 ? LOW : HIGH);
  digitalWrite(ADirectionPin2, angle > 0 ? HIGH : LOW);
  // motor b -> the fast one
  analogWrite(BDirectionPin1, angle > 0 ? 215 : 0); // This could be replaced by other enhanced methods
  analogWrite(BDirectionPin2, angle > 0 ? 0 : 215);

  // enable motors
  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  // wait for distance to be travelled
  delay(abs(degreeValue * angle));

  // shutdown motors
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);

  Serial.println("---- FINISHED TURN ----");
}

void servoAngle(double angle)
{
  // applies offset
  int processedAngle = angle + 97;
  myservo.write(processedAngle);
  delay(100);
}

// End of basic robot and servo movement logic part

// This part is for the getDistance logic, we will use getDistanceAverage() to get
// the distance
double getDistance()
{
  distance = 0;           // Initialize this to 0 so it will not contain the value from last check
  return sonar.ping_cm(); // Send ping, get distance in cm and return result (0 = outside set distance range)
}

double getDistanceAverage(int iterations)
{
  double distance = 0;
  for (int i = 0; i < iterations; i++) // loop through the number of calibration steps
  {
    distance += getDistance(); // get the distance from the nearest object in cm, and add to the total
  }
  distance /= iterations; // get the average distance from the nearest object
  // Serial.print("distance measured average:", distance, "across", iterations, "iterations");
  return distance; // return the average distance from the nearest object
}

// End of distance logic part

void checkAround()
{
  servoAngle(farRight);

  rightSide = getDistanceAverage(10);

  servoAngle(middle);
  frontSide = getDistanceAverage(10);

  servoAngle(farLeft);
  leftSide = getDistanceAverage(10);

  myservo.write(middle);
  Serial.println("check around");
  Serial.print("/  right: ");
  Serial.println(rightSide);
  Serial.print("/  front: ");
  Serial.println(frontSide);
  Serial.print("/  left: ");
  Serial.println(leftSide);
}

void performCalibration()
{
  Serial.println("---- START CALIBRATION ----");
  // Stop the rover
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);

  Serial.println("get average distance");

  // Get the initial distance from the nearest object
  double initialDistance = getDistanceAverage(50);

  Serial.print("initial distance average: ");
  Serial.println(initialDistance);

  digitalWrite(ADirectionPin1, HIGH);
  digitalWrite(ADirectionPin2, LOW);

  // motor b -> the fast one
  analogWrite(BDirectionPin1, 215); // This could be replaced by other enhanced methods
  analogWrite(BDirectionPin2, 0);

  // Move forward for a known duration (e.g., 1000ms)
  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);
  delay(1000);

  // Stop the rover
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);

  // Get the final distance from the nearest object
  double finalDistance = getDistanceAverage(50);

  Serial.print("final distance average: ");
  Serial.println(finalDistance);

  // Calculate the distance moved
  double distanceMoved = initialDistance - finalDistance;

  Serial.print("moved: ");
  Serial.println(distanceMoved);
  // Calculate the speed of the rover (distance/time)
  double speed = distanceMoved / 1.0; // Speed in cm/ms

  // Calculate the time needed to travel 1cm
  double timeFor1cm = 1.0 / speed; // Time in ms

  // Calculate the radius of the circle traced out by the wheel when the rover turns
  double radius = (wheelDistranceInner / 2.0) + (wheelThickness / 2.0);

  // Calculate the distance the wheel needs to travel to turn 1 degree
  double distanceFor1Degree = (2.0 * PI * radius) * (1.0 / 360.0);

  // Calculate the time needed to turn 1 degree
  double timeFor1Degree = distanceFor1Degree / speed; // Time in ms

  // Return the time needed to travel 1cm and turn 1 degree
  centimeterValue = timeFor1cm * 1000;
  degreeValue = timeFor1Degree * 1000;

  Serial.print("cm val: ");
  Serial.println(centimeterValue);
  Serial.print("deg val: ");
  Serial.println(degreeValue);
  Serial.println("---- END CALIBRATION ----");
}

// End of the calibration logic part

// This part is for the robot behaviour
void setup()
{
  Serial.begin(115200);
  myservo.attach(9);
  pinMode(ADirectionPin1, OUTPUT);
  pinMode(ADirectionPin2, OUTPUT);
  pinMode(AEnablePin, OUTPUT);
  pinMode(BDirectionPin1, OUTPUT);
  pinMode(BDirectionPin2, OUTPUT);
  pinMode(BEnablePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  // Perform calibration
  performCalibration();
}
void loop()
{
  checkAround();
  if (rightSide > 18.0)
  {
    turn(-90);
    ;
  }
  else if (frontSide > 18.0)
  {
  }
  else if (leftSide > 18.0)
  {
    turn(90);
    ;
  }
  else
  {
    turn(-180);
    ;
  }
  goStraight(18);
}
// End of the behaviour part

void turnLeft() {
  digitalWrite(ADirectionPin1, LOW);
  digitalWrite(ADirectionPin2, HIGH);

  analogWrite(BDirectionPin1, 225);
  analogWrite(BDirectionPin2, 0);

  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  delay(1300);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}

void turnRight() {
  digitalWrite(ADirectionPin1, HIGH);
  digitalWrite(ADirectionPin2, LOW);

  analogWrite(BDirectionPin1, 0);
  analogWrite(BDirectionPin2, 225);

  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);

  delay(1250);

  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
}