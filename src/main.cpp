#include <Arduino.h>
#include <Preferences.h> 
#include <var.h>
#include <display.h>
#include <tasks.h>

void setup() {
  Serial.begin(9600);
  Serial.println(F("\nBooting"));
  disableCore0WDT();
      
  
  xTaskCreatePinnedToCore(Main, "Main", 10000, NULL, 2, &Main_Handle, 0); 

  while(!ready_data){vTaskDelay(500/portTICK_PERIOD_MS);}
  xTaskCreatePinnedToCore(Servise, "Servise", 5000, NULL, 1, &Servise_Handle, 0);
  xTaskCreatePinnedToCore(Flaps, "Flaps", 5000, NULL, 1, &Flaps_Handle, 0); 
  xTaskCreatePinnedToCore(SPWM, "SPWM", 5000, NULL, 2, &SPWM_Handle, 1);
}

void loop()
{
  vTaskDelete(NULL);
}
