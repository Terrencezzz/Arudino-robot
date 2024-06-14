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

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

// ULTRASONIC CONTROL

#define ITERATIONS 5     // Number of iterations.
#define TRIGGER_PIN 11   // Arduino pin tied to trigger pin on ping sensor.
#define ECHO_PIN 10      // Arduino pin tied to echo pin on ping sensor.
#define MAX_DISTANCE 200 // Maximum distance (in cm) to ping.
#define PING_INTERVAL 33 // Milliseconds between sensor pings (29ms is about the min to avoid cross-sensor echo).

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

double distanceFromObjectIntital = 0;
double distanceFromObjectFinal = 0;
int numberOfCalibrationSteps = 3;

// PHYSICAL CONSTANTS
#define PI 3.1415926535897932384626433832795

const double wheelDiameter = 70; //~219.911mm
const double wheelDistranceInner = 100;
const double wheelThickness = 8;

// MOTOR CONTROL SPEED VALUES
double BSpeed = 165;
double ASpeed = 255;

float duration;
float distance;

double degreeValue = 1;
double centimeterValue = 1;

bool didMove = false;
double rightSide = 0;
double frontSide = 0;
double leftSide = 0;
double backSide = 0;

void goStraight()
{
    analogWrite(BEnablePin, BSpeed);
    analogWrite(AEnablePin, ASpeed);

    digitalWrite(ADirectionPin1, HIGH);
    digitalWrite(ADirectionPin2, LOW);

    digitalWrite(BDirectionPin1, HIGH);
    digitalWrite(BDirectionPin2, LOW);

    digitalWrite(AEnablePin, HIGH);
    digitalWrite(BEnablePin, HIGH);

    delay(cmToDuration(8));

    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
}

void turnRight()
{
    analogWrite(BEnablePin, BSpeed);
    analogWrite(AEnablePin, ASpeed);

    digitalWrite(ADirectionPin1, HIGH);
    digitalWrite(ADirectionPin2, LOW);

    digitalWrite(BDirectionPin1, LOW);
    digitalWrite(BDirectionPin2, HIGH);

    digitalWrite(AEnablePin, HIGH);
    digitalWrite(BEnablePin, HIGH);

    delay(angleToDuration(45));

    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
}

void turnLeft()
{
    analogWrite(BEnablePin, BSpeed);
    analogWrite(AEnablePin, ASpeed);

    digitalWrite(ADirectionPin1, LOW);
    digitalWrite(ADirectionPin2, HIGH);

    digitalWrite(AEnablePin, HIGH);
    digitalWrite(BEnablePin, HIGH);

    delay(angleToDuration(35));

    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
}

void servoAngle(double angle)
{
    // applies offset
    int processedAngle = angle + 97;
    myservo.write(processedAngle);
    delay(500);
}

double cmToDuration(double distance)
{
    return distance * 285.714;
}

double angleToDuration(double angle)
{
    return angle * (8300 / 360);
}

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

void checkAround()
{
    servoAngle(farRight);

    rightSide = getDistanceAverage(10);

    servoAngle(middle);
    frontSide = getDistanceAverage(10);

    servoAngle(farLeft);
    leftSide = getDistanceAverage(10);

    myservo.write(middle);
}

void reset()
{
    rightSide = 0;
    leftSide = 0;
    frontSide = 0;
    backSide = 0;
}

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
}

void loop()
{
    goStraight();
    turnRight();
}