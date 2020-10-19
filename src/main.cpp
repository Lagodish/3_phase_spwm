#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

const int L1 = 33;  
const int L2 = 26;
const int L3 = 17; 
const int H1 = 16;  
const int H2 = 25;
const int H3 = 27;   

const int shunt = 32;

// setting PWM properties
const int freq = 6000;
const int L1_val = 0;
const int L2_val = 1;
const int L3_val = 2;
const int H1_val = 3;
const int H2_val = 4;
const int H3_val = 5;
const int resolution = 10;

bool emergency = false;

#include <addons.h>
#include <tasks.h>


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Booting");
  Serial.println("Frequency_3Ph_Gen");
  wifi_set();
  ota_start();

  //disableCore0WDT();
  //disableCore1WDT();

  // configure LED PWM functionalitites
  ledcSetup(L1_val, freq, resolution);
  ledcSetup(L2_val, freq, resolution);
  ledcSetup(L3_val, freq, resolution);
  ledcSetup(H1_val, freq, resolution);
  ledcSetup(H2_val, freq, resolution);
  ledcSetup(H3_val, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(L1, L1_val);
  ledcAttachPin(L2, L2_val);
  ledcAttachPin(L3, L3_val);
  ledcAttachPin(H1, H1_val);
  ledcAttachPin(H2, H2_val);
  ledcAttachPin(H3, H3_val);

  ledcWrite(L1_val, 0);
  ledcWrite(L2_val, 0);
  ledcWrite(L3_val, 0);
  ledcWrite(H1_val, 0);
  ledcWrite(H2_val, 0);
  ledcWrite(H3_val, 0);

  xTaskCreatePinnedToCore(
    Servises,         /* Task function. */
    "Servises",       /* String with name of task. */
    8000,             /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &Task2,           /* Task handle. */
    0);               /* Core 1 */

  xTaskCreatePinnedToCore(
    SPWM,             /* Task function. */
    "SPWM",           /* String with name of task. */
    8000,            /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &Task1,           /* Task handle. */
    1);               /* Core 1 */ 


}

void loop()
{
   vTaskDelete(NULL);
}
