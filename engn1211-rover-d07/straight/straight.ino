const int ADirectionPin1 = 3;
const int ADirectionPin2 = 4;
const int AEnablePin = 2;
const int BDirectionPin1 = 5;
const int BDirectionPin2 = 6;
const int BEnablePin = 7;

void setup() {
  pinMode(ADirectionPin1, OUTPUT);
  pinMode(ADirectionPin2, OUTPUT);
  pinMode(AEnablePin, OUTPUT);
  pinMode(BDirectionPin1, OUTPUT);
  pinMode(BDirectionPin2, OUTPUT);
  pinMode(BEnablePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
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

  delay(1000);
}