#include <Servo.h>
#include <NewPing.h>

#define TRIGGER_PIN 11                              // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN 10                                 // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200                            // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

Servo myservo;

const int ADirectionPin1 = 3;
const int ADirectionPin2 = 4;
const int AEnablePin = 2;
const int BDirectionPin1 = 5;
const int BDirectionPin2 = 6;
const int BEnablePin = 7;

void setup()
{
    Serial.begin(9600);
    pinMode(ADirectionPin1, OUTPUT);
    pinMode(ADirectionPin2, OUTPUT);
    pinMode(AEnablePin, OUTPUT);
    pinMode(BDirectionPin1, OUTPUT);
    pinMode(BDirectionPin2, OUTPUT);
    pinMode(BEnablePin, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    myservo.attach(9);
    myservo.write(90);
    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
}

void loop()
{
    controlHBridge(ADirectionPin1, ADirectionPin2, AEnablePin, true);
    controlHBridge(BDirectionPin1, BDirectionPin2, BEnablePin, true);
    delay(2000);
    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
    delay(3000);
    controlHBridge(ADirectionPin1, ADirectionPin2, AEnablePin, false);
    controlHBridge(BDirectionPin1, BDirectionPin2, BEnablePin, false);
    delay(2000);
    digitalWrite(AEnablePin, LOW);
    digitalWrite(BEnablePin, LOW);
    delay(3000);
}

void controlHBridge(int directionPin1, int directionPin2, int enablePin, bool forward)
{
    digitalWrite(enablePin, LOW);
    digitalWrite(directionPin1, forward ? HIGH : LOW);
    digitalWrite(directionPin2, forward ? LOW : HIGH);
    digitalWrite(LED_BUILTIN, forward ? HIGH : LOW);
    digitalWrite(enablePin, HIGH);
}