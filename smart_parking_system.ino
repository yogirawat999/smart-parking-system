#include <Servo.h>

// ---------- Pin Configuration ----------

// Entry sensor
const int entryTrig = 10;
const int entryEcho = 11;

// Parking Slot 1 sensor
const int slot1Trig = 2;
const int slot1Echo = 3;

// Parking Slot 2 sensor
const int slot2Trig = 4;
const int slot2Echo = 5;

// Servo barrier
const int servoPin = 6;

// LEDs
const int slot1LED = 8;
const int slot2LED = 9;

Servo barrier;

// ---------- Settings ----------
const int entryDistance = 15;
const int parkingDistance = 10;


// ---------- Distance Function ----------
long getDistance(int trigPin, int echoPin)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
    return 999;

  return duration * 0.034 / 2;
}


// ---------- Setup ----------
void setup()
{
  Serial.begin(9600);

  pinMode(entryTrig, OUTPUT);
  pinMode(entryEcho, INPUT);

  pinMode(slot1Trig, OUTPUT);
  pinMode(slot1Echo, INPUT);

  pinMode(slot2Trig, OUTPUT);
  pinMode(slot2Echo, INPUT);

  pinMode(slot1LED, OUTPUT);
  pinMode(slot2LED, OUTPUT);

  barrier.attach(servoPin);

  // Start with barrier closed
  barrier.write(0);

  Serial.println("SMART PARKING SYSTEM");
  Serial.println("---------------------");
}


// ---------- Main Program ----------
void loop()
{
  long entry = getDistance(entryTrig, entryEcho);
  long slot1 = getDistance(slot1Trig, slot1Echo);
  long slot2 = getDistance(slot2Trig, slot2Echo);

  // Check parking slots
  bool slot1Occupied = slot1 <= parkingDistance;
  bool slot2Occupied = slot2 <= parkingDistance;

  bool parkingAvailable = !slot1Occupied || !slot2Occupied;

  // ---------- LED indication ----------

  if (slot1Occupied)
  {
    digitalWrite(slot1LED, HIGH);
  }
  else
  {
    digitalWrite(slot1LED, LOW);
  }

  if (slot2Occupied)
  {
    digitalWrite(slot2LED, HIGH);
  }
  else
  {
    digitalWrite(slot2LED, LOW);
  }


  // ---------- Serial Monitor ----------

  Serial.print("Entry: ");
  Serial.print(entry);
  Serial.print(" cm | ");

  Serial.print("Slot 1: ");
  Serial.print(slot1);
  Serial.print(" cm | ");

  Serial.print("Slot 2: ");
  Serial.print(slot2);
  Serial.print(" cm | ");


  if (slot1Occupied)
    Serial.print("Slot 1 FULL | ");
  else
    Serial.print("Slot 1 FREE | ");

  if (slot2Occupied)
    Serial.print("Slot 2 FULL");
  else
    Serial.print("Slot 2 FREE");


  // ---------- Barrier Control ----------

  if (entry <= entryDistance && parkingAvailable)
  {
    Serial.println(" | CAR DETECTED - GATE OPEN");

    barrier.write(90);

    delay(3000);

    barrier.write(0);
  }
  else if (entry <= entryDistance && !parkingAvailable)
  {
    Serial.println(" | PARKING FULL - GATE CLOSED");

    barrier.write(0);
  }
  else
  {
    Serial.println(" | WAITING");

    barrier.write(0);
  }

  delay(500);
}
