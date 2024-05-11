#define BLYNK_AUTH_TOKEN "t2JRLGH1Wsq-qwwZK4Nj19s7GoaGPvYS"
#define BLYNK_TEMPLATE_ID "TMPL6MQAQ3nrU"
#define BLYNK_TEMPLATE_NAME "Smart Bin"

#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>

const int f_trigPin = D7;
const int f_echoPin = D8;
const int l_trigPin = D5;
const int l_echoPin = D6;
const int indicatorLED = D4;

static WiFiClient blynkWiFiClient;

char ssid[] = "jesseroo";// WIFI SSID
char pass[] = "12345678";// WIFI password

BlynkTimer timer;
int pinValue = 0;

void setup() {
    // initialize serial communication:
    Serial.begin(9600);

    pinMode(f_trigPin, OUTPUT);
    pinMode(f_echoPin, INPUT);
    pinMode(l_trigPin, OUTPUT);
    pinMode(l_echoPin, INPUT);
    pinMode(indicatorLED, OUTPUT);

    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V0) {
  pinValue = param.asInt();
}

BLYNK_WRITE(V1) {
  pinValue = param.asInt();
}

BLYNK_WRITE(V2) {
  pinValue = param.asInt();
}

void loop() {
    Blynk.run();
    fillSensor();
    liftSensor();
}

void fillSensor(){
    digitalWrite(indicatorLED, LOW);
    long duration;
    float cm, percent;

    digitalWrite(f_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(f_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(f_trigPin, LOW);

    duration = pulseIn(f_echoPin, HIGH);
    cm = microsecondsToCentimeters(duration);

    if (cm > 1000){
        cm = 0;
        percent = 100;
        Serial.print(cm);
        Blynk.virtualWrite(V1, cm);
        Serial.print("cm, ");
        Serial.print(percent);
        Blynk.virtualWrite(V0, percent);
        Serial.print("% full");
        Serial.println();
        digitalWrite(indicatorLED, HIGH);
    } 
    else if (cm <= 33){
        percent = 100 - (cm / 33) * 100;
        if (percent < 0) {
            percent = 0;
        }else if (percent > 100) {
            percent = 100;
        }
        Serial.print(cm);
        Blynk.virtualWrite(V1, cm);
        Serial.print("cm, ");
        Serial.print(percent);
        Blynk.virtualWrite(V0, percent);
        if (percent > 85){
          digitalWrite(indicatorLED, HIGH);
        }
        Serial.print("% full");
        Serial.println();
    }
      delay(1000);
}

void liftSensor(){
    long duration;
    digitalWrite(l_trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(l_trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(l_trigPin, LOW);

    duration = pulseIn(l_echoPin, HIGH);
    if (duration > 58000){
      Blynk.virtualWrite(V2, 1);
      Serial.println("on the ground");
    } else{
      Blynk.virtualWrite(V2, 0);
      Serial.println("off the ground");
    }
    delay(2000);
}

long microsecondsToCentimeters(long microseconds) {
    return microseconds / 29 / 2; 
}

