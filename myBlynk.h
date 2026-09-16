#pragma once

#define BLYNK_TEMPLATE_ID "TMPL31gBD-qAr"
#define BLYNK_TEMPLATE_NAME "Flood"
#define BLYNK_AUTH_TOKEN "E7WqWHALORqGpw9s9eJSYRADQtcFnW2v"

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
char ssid[] = "ManjuGuest";
char pass[] = "Manju@123";

/* Virtual datastreams -- create these in the Blynk console:
     V0  Distance      Double   0-450   cm   -> Gauge / Value display
     V1  Rain status   Integer  0-1          -> LED (0 = dry, 1 = raining)
     V2  Motor status  Integer  0-1          -> LED (0 = OFF, 1 = ON)
*/
#define VPIN_DISTANCE  V0
#define VPIN_RAIN      V1
#define VPIN_MOTOR     V2

void myBlynk_initialise()
{
  // Connect to Blynk server (this also brings up the WiFi connection)
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

// Push the three live values to the dashboard.
// Distance is sent as -1 when the sensor gave no valid reply.
void myBlynk_update(float distanceCm, bool raining, bool motorOn)
{
  Blynk.virtualWrite(VPIN_DISTANCE, distanceCm);
  Blynk.virtualWrite(VPIN_RAIN, raining ? 1 : 0);
  Blynk.virtualWrite(VPIN_MOTOR, motorOn ? 1 : 0);
}

// virtualWrite() only queues the value -- Blynk.run() is what actually pushes
// it onto the wire. Call this before any long blocking operation (the SMTP
// send takes 5-10 s) so the dashboard is already up to date first.
void myBlynk_flush()
{
  for (uint8_t i = 0; i < 15; i++) {
    Blynk.run();
    delay(20);
  }
}
