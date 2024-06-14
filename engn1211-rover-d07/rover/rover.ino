#include <Servo.h>

Servo sensorServo;  // create servo object to control a servo

const int ADirectionPin1 = 3;
const int ADirectionPin2 = 4;
const int AEnablePin = 2;
const int BDirectionPin1 = 5;
const int BDirectionPin2 = 6;
const int BEnablePin = 7;
const int echoPin = 8;
const int trigPin = 9;
const int servoPin = 10;

const int lookLeftValue = 0;
const int lookAheadValue = 0;
const int lookRightValue = 0; 

const int nintyDegreeTurn = 100;
const int squareDuration = 0;
const int distanceFromObjectMargin = 5;

long distanceOnLeft = 0;
long distanceAhead = 0;
long distanceOnRight = 0;

void setup() {
  sensorServo.attach(servoPin);
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ADirectionPin1, OUTPUT);
  pinMode(ADirectionPin2, OUTPUT);
  pinMode(AEnablePin, OUTPUT);
  pinMode(BDirectionPin1, OUTPUT);
  pinMode(BDirectionPin2, OUTPUT);
  pinMode(BEnablePin, OUTPUT);
  pinMode(servoPin, OUTPUT);
}

void loop() {
  controlHBridge(false,false,false,true);
  // servo looks in all cardinal directions and stores the values from it
  sensorServo.write(lookLeftValue);
  distanceOnLeft = distanceFromObject();
  sensorServo.write(lookAheadValue);
  distanceAhead = distanceFromObject();
  sensorServo.write(lookRightValue);
  distanceOnRight = distanceFromObject();
  sensorServo.write(lookAheadValue);

  /*
  If we are able to move forward, go forward.
  If we can't move forward, but can go left, go left.
  If we can't go left, go right.
  If we can't go right, turn around and recheck values from ultrasonic.
  */

  // checks distance against specified threshold
  if (distanceAhead >= distanceFromObjectMargin) {
    // move forward 1 sqaure
    controlHBridge(false, false, true, false);
    delay(squareDuration);
  } else if (distanceOnLeft >= distanceFromObjectMargin) {
    //turn left and move forward 1 square
    controlHBridge(true, true, false, false);
    delay(nintyDegreeTurn);
    controlHBridge(false,false,true,false);
    delay(squareDuration);
  } else if (distanceOnRight >= distanceFromObjectMargin) {
    // turn right and move forward 1 square
    controlHBridge(true, false, false, false);
    delay(nintyDegreeTurn);
    controlHBridge(false,false,true,false);
    delay(squareDuration);
  } else {
    // turn around 180 degress
    controlHBridge(true, true, false, false);
    delay(2 * nintyDegreeTurn);
    controlHBridge(false,false,false,true);
  }
}

void controlHBridge(bool turnOnSpotEnable, bool turnOnSpotDirection, bool forward, bool stop) {
  // turn off all motors
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);

  // if we want to stop, end the function
  if (stop == true) {
    return;
  } else if (turnOnSpotEnable == true) {
    //if we want to turn on the spot, set one forward and one backwards
    digitalWrite(ADirectionPin1, turnOnSpotDirection ? HIGH : LOW);
    digitalWrite(ADirectionPin2, turnOnSpotDirection ? LOW : HIGH);
    digitalWrite(BDirectionPin1, turnOnSpotDirection ? LOW : HIGH);
    digitalWrite(BDirectionPin2, turnOnSpotDirection ? HIGH : LOW);
  } else {
    // if we want to go straight, set both to forward or both to backwards
    digitalWrite(ADirectionPin1, forward ? HIGH : LOW);
    digitalWrite(ADirectionPin2, forward ? LOW : HIGH);
    digitalWrite(BDirectionPin1, forward ? HIGH : LOW);
    digitalWrite(BDirectionPin2, forward ? LOW : HIGH);
  }
  // re-enable the h-bridge
  digitalWrite(AEnablePin, HIGH);
  digitalWrite(BEnablePin, HIGH);
  return;
}

/*
Code provided by Ping example (File > Examples > 06.Sensors > Ping)
*/

long distanceFromObject() {
  long duration;
  int distance;
  // Clear the trigPin by setting it LOW:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);

  // Trigger the sensor by setting the trigPin high for 10 microseconds:
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the echoPin, pulseIn() returns the duration (length of the pulse) in microseconds:
  duration = pulseIn(echoPin, HIGH);
  // Calculate the distance:
  distance = duration * 0.034 / 2;
  return distance;
}