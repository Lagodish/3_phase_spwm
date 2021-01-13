/*
const char* ssid = "WiFi-DOM.ru-6111";  //Название сети! WiFi-DOM.ru-6111 Psin(a)
const char* password = "611rdddRDD"; //Пароль! 611rdddRDD Hey!28R0om!

void wifi_set(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
    }

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void ota_start(){
      // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
     ArduinoOTA.setHostname("Frequency_3Ph_Gen");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
        emergency = true;
    })s
    .onEnd([]() {s
      Serial.println("\nEnds");
    })s
    .onProgress([](unsigneds int progress, unsigned int total) {
      Serial.printf("Progresss: %u%%\r", (progress / (total / 100)));
     // emergency = true;s
    })s
    .onError([](ota_error_ts error) {
      Serial.printf("Error[s%u]: ", error);
      if (error == OTA_AUTHs_ERROR) Serial.println("Auth Failed");
      else if (error == OTAs_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTAs_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTAs_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
}
*/
#define SINE_TABLE_SIZE 483
static uint16_t SINE_LOOKUP_TABLE[SINE_TABLE_SIZE] =
{
0,0,1,2,3,4,6,9,11,14,17,21,
25,29,34,39,44,50,56,62,
69,75,83,90,98,106,114,123,
131,140,150,159,169,179,190,200,
211,222,233,244,256,267,279,291,
303,316,328,341,353,366,379,392,
405,418,431,445,458,471,485,498,
512,525,538,552,565,578,592,605,
618,631,644,657,670,682,695,707,
720,732,744,756,767,779,790,801,
812,823,833,844,854,864,873,883,
892,900,909,917,925,933,940,948,
954,961,967,973,979,984,989,994,
998,1002,1006,1009,1012,1014,1017,1019,
1020,1021,1022,1023,1023,1023,1022,1021,
1020,1019,1017,1014,1012,1009,1006,1002,
998,994,989,984,979,973,967,961,
954,948,940,933,925,917,909,900,
892,883,873,864,854,844,833,823,
812,801,790,779,767,756,744,732,
720,707,695,682,670,657,644,631,
618,605,592,578,565,552,538,525,
512,498,485,471,458,445,431,418,
405,392,379,366,353,341,328,316,
303,291,279,267,256,244,233,222,
211,200,190,179,169,159,150,140,
131,123,114,106,98,90,83,75,
69,62,56,50,44,39,34,29,
25,21,17,14,11,9,6,4,
3,2,1,0,0,0,1,2,3,4,6,9,11,14,17,21,
25,29,34,39,44,50,56,62,
69,75,83,90,98,106,114,123,
131,140,150,159,169,179,190,200,
211,222,233,244,256,267,279,291,
303,316,328,341,353,366,379,392,
405,418,431,445,458,471,485,498,
512,525,538,552,565,578,592,605,
618,631,644,657,670,682,695,707,
720,732,744,756,767,779,790,801,
812,823,833,844,854,864,873,883,
892,900,909,917,925,933,940,948,
954,961,967,973,979,984,989,994,
998,1002,1006,1009,1012,1014,1017,1019,
1020,1021,1022,1023,1023,1023,1022,1021,
1020,1019,1017,1014,1012,1009,1006,1002,
998,994,989,984,979,973,967,961,
954,948,940,933,925,917,909,900,
892,883,873,864,854,844,833,823,
812,801,790,779,767,756,744,732,
720,707,695,682,670,657,644,631,
618,605,592,578,565,552,538,525,
512,498,485,471,458,445,431,418,
405,392,379,366,353,341,328,316,
303,291,279,267,256,244,233,222,
211,200,190,179,169,159,150,140,
131,123,114,106,98,90,83,75,
69,62,56,50,44,39,34,29,
25,21,17,14,11,9,6,4,
3,2,1,0,0,0,0
};