#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <Preferences.h> 
#include <var.h>

Preferences preferences;
TaskHandle_t Task1;
TaskHandle_t Task2;
TaskHandle_t TaskWiFi;
WebServer Server;
AutoConnect Portal(Server);
AutoConnectConfig Config;    

#include <display.h>
#include <addons.h>
#include <tasks.h>



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Booting");
  Serial.println("Frequency_3Ph_Gen");
  //wifi_set();
  //ota_start();
  
  disableCore0WDT();
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
    10000,             /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &Task2,           /* Task handle. */
    0);               /* Core 1 */

  xTaskCreatePinnedToCore(
    WiFiService,       
    "WiFiService",        
    8000,          
    NULL,             
    1,             
    &TaskWiFi,           
    1);

  vTaskDelay(2000 / portTICK_PERIOD_MS);
  vTaskDelete(TaskWiFi);

  xTaskCreatePinnedToCore(
    SPWM,       
    "SPWM",        
    8000,          
    NULL,             
    1,             
    &Task1,           
    1);               


}

void loop()
{
   vTaskDelete(NULL);
}
