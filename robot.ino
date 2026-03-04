#include <SoftwareSerial.h>
#include <DHT.h>

// Bluetooth
SoftwareSerial BT(2, 3); // RX = 2, TX = 3

// Motor driver pins (L293D/L298N IN1–IN4)
int motor1Pin1 = 9;
int motor1Pin2 = 10;
int motor2Pin1 = 11;
int motor2Pin2 = 12;

// Sensors
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int gasPin = A0;
int trigPin = 7;
int echoPin = 6;

void setup() {
  Serial.begin(9600);
  BT.begin(9600);

  dht.begin();

  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  stopMotors(); // ensure motors are off at start
}

void loop() {
  // Bluetooth control
  if (BT.available()) {
    char command = BT.read();
    Serial.print("Received: ");
    Serial.println(command);

    if (command == 'F') forward();
    else if (command == 'B') backward();
    else if (command == 'L') left();
    else if (command == 'R') right();
    else if (command == 'S') stopMotors();
  }

  // Sensor readings
  int gasValue = analogRead(gasPin);
  float temp = dht.readTemperature();

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;

  Serial.print("Gas: ");
  Serial.println(gasValue);
  Serial.print("Temp: ");
  Serial.println(temp);
  Serial.print("Distance: ");
  Serial.println(distance);

  delay(1000);
}

// Motor functions
void forward() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void backward() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void left() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, HIGH);
  digitalWrite(motor2Pin1, HIGH);
  digitalWrite(motor2Pin2, LOW);
}

void right() {
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
}

void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
