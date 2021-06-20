#include <Arduino.h>

/*
D6 - TRIG, ultrasonic sensor SR04T
D7 - ECHO, ultrasonic sensor SR04T

*/

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

//#define TRIGGER_PIN  12     // D6 Arduino pin tied to trigger pin on the ultrasonic sensor.
//#define ECHO_PIN     13     // D7 Arduino pin tied to echo pin on the ultrasonic sensor.

#define TRIGGER_PIN 13 //!!!!!!!!!!!!
#define ECHO_PIN 12 //!!!!!!!!!!!!
#define USONIC_DIV 58.0
#define MEASURE_SAMPLE_DELAY 5
#define MEASURE_SAMPLES 20
#define MEASURE_DELAY 2

char auth[] = "tQSG6TYMuj33_0OrdRxhvxIkmiNAxrbZ";
char ssid[] = "";
char pass[] = "";

long distance_long = 111;
long distance_single = 222;

void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass); // Запуск Blynk
  Serial.println("setup");
  pinMode(TRIGGER_PIN, OUTPUT); // Initializing Trigger Output and Echo Input
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIGGER_PIN, LOW); // Reset the trigger pin and wait a half a second
  delayMicroseconds(500);
}

long singleMeasurement()
{

  long duration = 0;               // Measure: Put up Trigger...
  digitalWrite(TRIGGER_PIN, HIGH); // Wait for 11 µs ...
  delayMicroseconds(11);           // Put the trigger down ...
  digitalWrite(TRIGGER_PIN, LOW);  // Wait for the echo ...
  duration = pulseIn(ECHO_PIN, HIGH);

  return (long)(((float)duration / USONIC_DIV) * 10.0);
}

long measure()
{
  long measureSum = 0;
  for (int i = 0; i < MEASURE_SAMPLES; i++)
  {
    delay(MEASURE_SAMPLE_DELAY);
    measureSum += singleMeasurement();
  }
  return measureSum / MEASURE_SAMPLES;
}



void sendMeasure()
{
  Serial.print("Distance logng: ");
  Serial.print(distance_long + 22);
  Serial.println();
  Serial.print("Distance singleMeasurement: ");
  Serial.println(distance_single + 22);
  Serial.println();

  Blynk.virtualWrite(V1, distance_long); 
  Blynk.virtualWrite(V2, distance_single); 
}

void loop()
{
  Blynk.run();

  delay(MEASURE_DELAY);
  distance_long = measure();
  distance_single = singleMeasurement();

  sendMeasure();
}

