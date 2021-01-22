#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <BlynkSimpleEsp32.h>
#include <AutoConnect.h>
#include <Preferences.h> 
#include <var.h>

Preferences preferences;
TaskHandle_t ServisesHandle;
TaskHandle_t MathServisesHandle;
TaskHandle_t SPWMHandle;
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
  disableCore0WDT();
  //disableCore1WDT();
  ledcSetup(L1_val, freq, resolution);
  ledcSetup(L2_val, freq, resolution);
  ledcSetup(L3_val, freq, resolution);
  ledcSetup(H1_val, freq, resolution);
  ledcSetup(H2_val, freq, resolution);
  ledcSetup(H3_val, freq, resolution);

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

  for(int i = 0; i < SINE_TABLE_SIZE; i++){
  SINE_TABLE[0][i]=SINE_LOOKUP_TABLE[i];}

  for(int i = 0; i < SINE_TABLE_SIZE; i++){
  SINE_TABLE[1][i]=SINE_LOOKUP_TABLE_2[i];}

  xTaskCreatePinnedToCore(
    Servises,         /* Task function. */
    "Servises",       /* String with name of task. */
    10000,             /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &ServisesHandle,           /* Task handle. */
    0);               /* Core 0 */

  xTaskCreatePinnedToCore(
    MathServises,         /* Task function. */
    "MathServises",       /* String with name of task. */
    10000,             /* Stack size in bytes. */
    NULL,             /* Parameter passed as input of the task */
    1,                /* Priority of the task. */
    &MathServisesHandle,           /* Task handle. */
    0);               /* Core 0 */

  //vTaskDelay(1000/portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(
    WiFiService,       
    "WiFiService",        
    8000,          
    NULL,             
    1,             
    &TaskWiFi,           
    1);

while(!Wifi_connected){vTaskDelay(1000);}
  if(PhaseMode){
  xTaskCreatePinnedToCore(
    SPWM3,       
    "SPWM3",        
    8000,          
    NULL,             
    1,             
    &SPWMHandle,           
    1); }
    else{
  xTaskCreatePinnedToCore(
    SPWM2,       
    "SPWM2",        
    8000,          
    NULL,             
    1,             
    &SPWMHandle,           
    1);}              


}

void loop()
{
   vTaskDelete(NULL);
}
