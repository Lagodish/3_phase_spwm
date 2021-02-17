#include <Arduino.h>
#include <Preferences.h> 
#include <var.h>
#include <display.h>
#include <tasks.h>

void setup() {

  Serial.begin(9600);
  Serial.println("\nBooting");
  disableCore0WDT();

  xTaskCreatePinnedToCore(Servises, "Servises", 10000, NULL, 2, &ServisesHandle, 0);       
  xTaskCreatePinnedToCore(MathServises, "MathServises", 10000, NULL, 1, &MathServisesHandle, 0);

  while(!ready_data){vTaskDelay(500/portTICK_PERIOD_MS);}
  xTaskCreatePinnedToCore(SPWM, "SPWM", 10000, NULL, 2, &SPWMHandle, 1);
}

void loop()
{
  vTaskDelete(NULL);
}
