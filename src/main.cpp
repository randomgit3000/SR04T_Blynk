#include <Arduino.h>

/*
D6 - TRIG, ultrasonic sensor SR04T
D7 - ECHO, ultrasonic sensor SR04T

*/

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <NewPing.h>
#include <MedianFilter.h>  // Filter OFF/ON

const float Pi = 3.14;
const float Radius = 0.4;   // Радиус кольца колодца
const float Depth = 5.24;   // Глубина колодца
const float Rod = 1.52;     // Длина штанги, на конце с ультазвуковым датчиком
float measuring;            // Расстояние, измеряется от конца штанги (датчика) до воды в см.
float distance;             // Расстояние, расчитывается от края колодца до воды в м.
float level;                // Уровень воды в м.
float capacity;             // Объем воды в м3
unsigned long last_time;
unsigned long last_time_2;

#define TRIGGER_PIN  12     // D6 Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     13     // D7 Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 450    // Maximum sensor distance is rated at 400-500cm.
#define SEALEVELPRESSURE_HPA (1013.25) // 760мм.рт.ст. = 1013.25 гПа (hPa)

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.
MedianFilter filter(31,0); // Filter OFF/ON

char auth[] = "xxx";
char ssid[] = "xxx";
char pass[] = "xxx";

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(xxx,xxx,xxx,xxx), 8080);  // Запуск Blynk
  Serial.println();

  Blynk.setProperty(V7, "label", "Глубина");
  Blynk.setProperty(V13, "label", "Уровень");
  Blynk.setProperty(V15, "label", "Объем");
}

void loop() {
  Blynk.run();

  if (millis() - last_time_2 > 50) {    // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
    last_time_2 = millis();

    unsigned int oo,uS = sonar.ping(); // Send ping, get ping time in microseconds (uS). Filter ON //-OFF
    filter.in(uS);
    oo = filter.out();

    measuring = (oo / US_ROUNDTRIP_CM);
    distance = measuring / 100 + Rod;
    level = Depth - distance;
    capacity = Pi * Radius * Radius * level;
  }

  if (millis() - last_time > 1*1000) {           // Интервал времени выдачи измерений  в мсек
    last_time = millis();
    sendMeasure();
  }
}

void sendMeasure() {
  Serial.println("    До воды: " + String(measuring) + " см.");
  Serial.println ();
  Blynk.virtualWrite(V7, Depth);      // Глубина колодца
  Blynk.virtualWrite(V13,level);      // Уровень воды
  Blynk.virtualWrite(V15,capacity);   // Объем воды
  Blynk.virtualWrite(V12,measuring);  // Измеренное расстояние
}