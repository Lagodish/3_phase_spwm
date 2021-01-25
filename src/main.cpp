#include <Arduino.h>
#include <Preferences.h> 
#include <var.h>
#include <display.h>
#include <tasks.h>

void setup() {

  Serial.begin(9600);
  Serial.println("\nBooting");
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

  xTaskCreatePinnedToCore(
    Servises,
    "Servises",
    10000,
    NULL,
    1,
    &ServisesHandle,
    0);       

  xTaskCreatePinnedToCore(
    MathServises,         
    "MathServises",    
    10000,
    NULL,
    1,
    &MathServisesHandle,
    0);

  while(!ready_data){vTaskDelay(500/portTICK_PERIOD_MS);}

  if(PhaseMode){
    xTaskCreatePinnedToCore(
      SPWM3,       
      "SPWM3",        
      8000,          
      NULL,             
      1,             
      &SPWMHandle,           
      1);}
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
