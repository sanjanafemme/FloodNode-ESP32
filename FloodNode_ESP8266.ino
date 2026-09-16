/*
 * Smart Adaptive Urban Flooding and Waterlogging Management System
 * ESP8266 (NodeMCU) -- US-100 ultrasonic + rain sensor (DO) + relay
 *                      + Blynk dashboard + email alert on motor change
 *
 * Wiring:
 *   US-100 VCC -> 3V3      Trig/TX -> D6      Echo/RX -> D7      GND -> GND
 *   Rain DO    -> D5       Relay IN -> D1
 *   US-100 jumper must be FITTED (UART mode). No voltage divider needed.
 *
 * Logic: distance below the threshold means the water has risen -> relay ON.
 * Blynk: V0 = distance, V1 = rain status, V2 = motor status.
 * Email: sent when the motor turns ON, once per run, at most once per 30 s.
 *        The Blynk motor status is pushed out before the email is sent.
 */

#include "myBlynk.h"      // must come first -- it defines the BLYNK_* macros
#include "myEmail.h"
#include <SoftwareSerial.h>

SoftwareSerial us100Serial(D7, D6);   // RX, TX

#define PIN_RAIN_DO   D0
#define PIN_RELAY     D1
#define RELAY_ON      LOW             // opto-isolated relays are active LOW
#define RELAY_OFF     HIGH

const float THRESHOLD_CM = 10.0;      // relay ON when water is closer than this

bool emailSentForThisRun = false;     // one email per pump run, on switch-on

void setup() {
  Serial.begin(115200);
  us100Serial.begin(9600);
  pinMode(PIN_RAIN_DO, INPUT);
  pinMode(PIN_RELAY, OUTPUT);
  digitalWrite(PIN_RELAY, RELAY_OFF);

  myBlynk_initialise();               // connects WiFi + Blynk
  myEmail_initialise();               // confirms WiFi is up
}

void loop() {
  Blynk.run();

  // --- distance from the US-100 (0x55 -> two bytes, millimetres) ---
  us100Serial.flush();
  us100Serial.write(0x55);
  delay(50);

  float distanceCm = -1;
  if (us100Serial.available() >= 2) {
    unsigned int hi = us100Serial.read();
    unsigned int lo = us100Serial.read();
    unsigned int mm = (hi * 256) + lo;
    if (mm >= 20 && mm <= 4500) distanceCm = mm / 10.0;
  }

  // --- rain sensor digital flag (LOW = wet) ---
  bool raining = (digitalRead(PIN_RAIN_DO) == LOW);

  // --- relay: ON below the threshold, OFF above it ---
  bool motorOn = (distanceCm > 0 && distanceCm < THRESHOLD_CM);
  digitalWrite(PIN_RELAY, motorOn ? RELAY_ON : RELAY_OFF);

  // --- push to Blynk ---
  myBlynk_update(distanceCm, raining, motorOn);

  // --- report ---
  Serial.print("Distance: ");
  if (distanceCm > 0) Serial.print(distanceCm, 1); else Serial.print("--");
  Serial.print(" cm | Rain: ");
  Serial.print(raining ? "YES" : "NO");
  Serial.print(" | Motor: ");
  Serial.println(motorOn ? "ON" : "OFF");

  // --- email when the motor turns ON, once per run, at most once per 30 s ---
  if (motorOn && !emailSentForThisRun && emailReady()) {
    myBlynk_flush();                  // dashboard shows MOTOR ON before we block
    String body = "Motor turned ON\n";
    body += "Distance: " + String(distanceCm, 1) + " cm\n";
    body += "Threshold: " + String(THRESHOLD_CM, 1) + " cm\n";
    body += "Rain: " + String(raining ? "YES" : "NO") + "\n";
    sendEmail("Flood Node: MOTOR ON", body);
    emailSentForThisRun = true;
  }
  if (!motorOn) emailSentForThisRun = false;   // re-arm for the next switch-on

  delay(1000);
}
