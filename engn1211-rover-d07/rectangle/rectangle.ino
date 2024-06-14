#define ADirectionPin1 5
#define ADirectionPin2 6
#define AEnablePin 4
#define BDirectionPin1 8
#define BDirectionPin2 7
#define BEnablePin 9

double BSpeed = 200;
double ASpeed = 255;

void setup()
{
  pinMode(ADirectionPin1, OUTPUT);
  pinMode(ADirectionPin2, OUTPUT);
  pinMode(AEnablePin, OUTPUT);
  pinMode(BDirectionPin1, OUTPUT);
  pinMode(BDirectionPin2, OUTPUT);
  pinMode(BEnablePin, OUTPUT);
  delay(2000);

}

void loop()
{   
    
    
    goDistance(100,true);
    

}

void controlHBridge(bool turnOnSpotEnable, bool turnOnSpotDirection, bool forward, bool stop)
{
  digitalWrite(AEnablePin, LOW);
  digitalWrite(BEnablePin, LOW);
  analogWrite(BEnablePin, BSpeed);
  analogWrite(AEnablePin, ASpeed);
  

  // Set the direction pins based on the desired direction
  if (stop == true)
  {
    return;
  }
  else if (turnOnSpotEnable == true)
  {
    digitalWrite(ADirectionPin1, turnOnSpotDirection ? HIGH : LOW);
    digitalWrite(ADirectionPin2, turnOnSpotDirection ? LOW : HIGH);
    digitalWrite(BDirectionPin1, turnOnSpotDirection ? LOW : HIGH);
    digitalWrite(BDirectionPin2, turnOnSpotDirection ? HIGH : LOW);
  }
  else
  {
    digitalWrite(ADirectionPin1, forward ? HIGH : LOW);
    digitalWrite(ADirectionPin2, forward ? LOW : HIGH);
    digitalWrite(BDirectionPin1, forward ? HIGH : LOW);
    digitalWrite(BDirectionPin2, forward ? LOW : HIGH);
  }
  // Enable the H-bridge
  
  return;
}

void turn90degrees(bool counterClockwise)
{
  // perform a 90 degree turn
  controlHBridge(true, counterClockwise, false, false);
  delay(angleToDuration(90));
  controlHBridge(false, false, false, true);
}

void turnAngle(bool counterClockwise, double angle){
  // perform a n degree turn
  controlHBridge(true, counterClockwise, false, false);
  delay(angleToDuration(angle));
  controlHBridge(false, false, false, true);
}


void goDistance(double distance, bool direction)
{
  controlHBridge(false, false, direction, false);
  delay(cmToDuration(distance));
  controlHBridge(false, false, false, true);
}

double cmToDuration(double distance)
{
  return distance * 285.714;
}

double angleToDuration(double angle)
{
  return angle * (8300 / 360);
}