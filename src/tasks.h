#include <Arduino.h>
#include "KalmanFilter.h"
#include "GyverButton.h"


GButton butt1(encBtn); // кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)

void data();
void onConnect(IPAddress& ipaddr);
void CheckConnection();

void motor(int H1_,int H2_,int H3_, int L1_,int L2_,int L3_){
    H1_=H1_*k*k_Freq; 
    H2_=H2_*k*k_Freq;
    H3_=H3_*k*k_Freq;
    L1_=L1_*k*k_Freq;
    L2_=L2_*k*k_Freq;
    L3_=L3_*k*k_Freq; 
    ledcWrite(L1_val, L1_); //L1
    ledcWrite(L2_val, L2_); //L2
    ledcWrite(L3_val, L3_); //L3
    ledcWrite(H1_val, H1_); //H1
    ledcWrite(H2_val, H2_); //H2
    ledcWrite(H3_val, H3_); //H3
}

void motor2(int H1_,int H2_,int H3_, int L1_,int L2_,int L3_){
    L1_=L1_*k*k_Freq;
    L2_=L2_*k*k_Freq;
    ledcWrite(L1_val, L1_); //L1
    ledcWrite(L2_val, L2_); //L2
    ledcWrite(L3_val, 0); //L3
    ledcWrite(H1_val, H1_); //H1
    ledcWrite(H2_val, H2_); //H2
    ledcWrite(H3_val, 0); //H3
}

void SPWM3( void * parameter)
{
    Serial.println("SPWM_3");
    int Cycle = 0;

    while(1){    
    if(!emergency&&!opennedMenu){
    for(Cycle = 0; Cycle <= 480; Cycle=Cycle+2){   

        if((Cycle>0)&&(Cycle<=80)){    //___1
        motor(SINE_TABLE[SinMode][Cycle],0,SINE_TABLE[SinMode][Cycle+160], 0, SINE_TABLE[SinMode][Cycle+80], 0); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>80)&&(Cycle<=160)){    //___2 
        motor(SINE_TABLE[SinMode][Cycle],0,0,0,SINE_TABLE[SinMode][Cycle+80],SINE_TABLE[SinMode][Cycle+160]); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>160)&&(Cycle<=240)){    //___3
        motor(SINE_TABLE[SinMode][Cycle],SINE_TABLE[SinMode][Cycle+80],0,0,0,SINE_TABLE[SinMode][Cycle+160]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>240)&&(Cycle<=320)){    //___4
        motor(0,SINE_TABLE[SinMode][Cycle+80],0,SINE_TABLE[SinMode][Cycle],0,SINE_TABLE[SinMode][Cycle-80]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>320)&&(Cycle<=400)){    //___5
        motor(0,SINE_TABLE[SinMode][Cycle-160],SINE_TABLE[SinMode][Cycle-80],SINE_TABLE[SinMode][Cycle],0,0);  //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>400)&&(Cycle<=480)){    //___6
        motor(0,0,SINE_TABLE[SinMode][Cycle-80],SINE_TABLE[SinMode][Cycle],SINE_TABLE[SinMode][Cycle-160],0); //H1 H2 H3 L1 L2 L3
        }
        
        delayMicroseconds(delay_time);//45 mc - 50 Hz
                    
    }
        k=k+step;  
        if(k>1){k=1.0;step=0.0;}
        if(k<0){k=0.0;step=0.0;}   
    }
    else{
        motor2(0,0,0,0,0,0); //H1 H2 H3 L1 L2 L3
        k=0.2;
        step = 0.001;
        vTaskDelay(3000/portTICK_PERIOD_MS);  
    } 
    }

}

void SPWM2( void * parameter)
{
    Serial.println("SPWM_2");
    int Cycle = 0;
    while(1){    
    if(!emergency&&!opennedMenu){
    for(Cycle = 0; Cycle <= 480; Cycle=Cycle+2){   

        if((Cycle>0)&&(Cycle<=80)){    //___1
        motor2(0,1023,0,SINE_TABLE[SinMode][Cycle],0,0); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>80)&&(Cycle<=160)){    //___2 
        motor2(0,1023,0,SINE_TABLE[SinMode][Cycle],0,0); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>160)&&(Cycle<=240)){    //___3
        motor2(0,1023,0,SINE_TABLE[SinMode][Cycle],0,0); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>240)&&(Cycle<=320)){    //___4
        motor2(1023,0,0,0,SINE_TABLE[SinMode][Cycle],0); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>320)&&(Cycle<=400)){    //___5
        motor2(1023,0,0,0,SINE_TABLE[SinMode][Cycle],0);  //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>400)&&(Cycle<=480)){    //___6
        motor2(1023,0,0,0,SINE_TABLE[SinMode][Cycle],0); //H1 H2 H3 L1 L2 L3
        }
        
        delayMicroseconds(delay_time);//45 mc - 50 Hz
                    
    }
        k=k+step;  
        if(k>1){k=1.0;step=0.0;}
        if(k<0){k=0.0;step=0.0;}   
    }
    else{
        motor2(0,0,0,0,0,0); //H1 H2 H3 L1 L2 L3
        Serial.println("Stopped!");
        k=0.2;
        step = 0.001;
        vTaskDelay(3000/portTICK_PERIOD_MS);  
    } 
    }


}

BLYNK_WRITE(V0){
    emergency = bool(param.asInt());}

BLYNK_WRITE(V1){
    Power_set = param.asInt(); 
    if((Power_set<30)||(Power_set>130)){
        Power_set=100;
    }}

void MathServises( void * parameter)
{  
String out_data = "";
while(1){

if((Power_set<30)||(Power_set>130)){  Power_set = 100;}
if((Power<30)||(Power>130)){  Power = 100;}

    if(PhaseMode){ // 3ph
      frequency = map(Power, 30,130, 15, 65);

      delay_time = -37.2311 + 4098.9954 / frequency;
      if(delay_time<0){delay_time=0;}


      if(Power>100){cache_Power=100;}
      else{cache_Power=Power;}
      k_Freq = double(cache_Power)/100;
    }
    else{ // 2ph
      if(Power>100){
          frequency = map(Power, 30,130, 15, 65);
          cache_Power = 100;}
      else{
          cache_Power=Power;
          frequency = 50;}

      k_Freq = double(cache_Power)/100;
      delay_time = -37.2311 + 4098.9954 / frequency;
      if(delay_time<0){delay_time=0;}
    }

if(BlynkMode&&Blynk.connected()){
if(PhaseMode){out_data="3-ph";}
else{out_data="1-ph";}
if(SinMode){out_data+="   SPWM";}
else{out_data+="   THIPWM";}
Blynk.virtualWrite(V4, out_data);
Blynk.virtualWrite(V2, frequency);
if(emergency||opennedMenu){Blynk.virtualWrite(V3, 0);}
else{Blynk.virtualWrite(V3, int(k_Freq*k*220));} 
}

Power = filter(Power_set+1);

vTaskDelay(300/portTICK_PERIOD_MS);  
}
}

void Servises( void * parameter)
{   
    
    Serial.println("Servises");

    data();
    Wire.begin();
    u8g2.begin();
    u8g2.enableUTF8Print();	
    u8g2.setFont(fontName);

    butt1.setDebounce(80);
    butt1.setTimeout(300);
    butt1.setType(HIGH_PULL);

    ESP32Encoder::useInternalWeakPullResistors=UP;
    encoder.attachHalfQuad(encA, encB);
    encoder.clearCount();

    nav.idleTask=MainScreen;
    nav.idleOn(MainScreen);

    while(1){

    timer_0++;
    timer_1++;
    if(timer_0>40){timer_0=0;}
    if(timer_1>150){timer_1=0; CheckConnection();}

    butt1.tick();

    if(WiFiCtrl){Portal.handleClient();}
    if(BlynkMode&&Blynk.connected()){Blynk.run();}
    if(Wifi_connected&&oneUse){vTaskDelete(TaskWiFi);oneUse = false;}   

    if(emergency){V_Print = 0;}
    else{V_Print = int(k_Freq*k*220);}    

    encoredVal = encoder.getCount();
    if(encoredVal > encoredVal_old){
        nav.doNav(upCmd);
    }
    if(encoredVal < encoredVal_old){
        nav.doNav(downCmd);
    }
    encoredVal_old = encoredVal;    
    
    if(butt1.isClick()){
        if(Wifi_connected){
            nav.doNav(enterCmd);
        }
        else{
            Wifi_connected = true;
            emergency = false;
        }
    }

    if(abs(encoredVal)>200000000){
        encoredVal = 0; encoredVal_old = 0;
    }

    //nav.doInput();
    nav.poll();
    u8g2.setContrast(map(BRT_Disp, 0, 100, 10, 190));
    
    u8g2.firstPage();
    do nav.doOutput(); while(u8g2.nextPage());
    vTaskDelay(50/portTICK_PERIOD_MS);  
    

    }
}

void WiFiService( void * parameter)
{
    Serial.println("WiFiService");
    Config.autoReconnect = true;
    Config.ota = AC_OTA_BUILTIN;
    Config.title = "Controller Setup";
    Config.homeUri = "/_ac/config";
    Config.apid = "Controller_Setup";
    Config.psk = "";
    Config.bootUri = AC_ONBOOTURI_HOME;
    Config.menuItems = AC_MENUITEM_CONFIGNEW | AC_MENUITEM_RESET | AC_MENUITEM_DISCONNECT | AC_MENUITEM_UPDATE;
    Portal.config(Config);
    Portal.onConnect(onConnect);  // Register the ConnectExit function

    if (Portal.begin()) {
    Serial.println("HTTP server:" + WiFi.localIP().toString());
    }

    while (1)
    {
        vTaskDelay(1000);
    }
    
    Serial.println("Ending WiFiService");
}

void data(){
    preferences.begin("FrequencyData", false);
    if(preferences.getUInt("FirstStart", 0)!=3){
        preferences.putUInt("FirstStart", 3);
        preferences.putUInt("Power", 100);
        preferences.putUInt("Bright", 30);
        preferences.putUInt("StartTime", 1);
        preferences.putBool("WiFiCtrl", true);
        preferences.putBool("PhaseMode", true);
        preferences.putBool("BlynkMode", false);
        preferences.putBool("SinMode", true);
    }
    Power_set = preferences.getUInt("Power", 100);
    BRT_Disp = preferences.getUInt("Bright", 50);
    k_menu = preferences.getUInt("StartTime", 1);
    WiFiCtrl = preferences.getBool("WiFiCtrl", true);
    PhaseMode = preferences.getBool("PhaseMode", true);
    BlynkMode = preferences.getBool("BlynkMode", false);
    SinMode = preferences.getBool("SinMode", true);
    preferences.end();

    if(!WiFiCtrl){Wifi_connected = true;emergency = false;}

    if((BRT_Disp<0)||(BRT_Disp>100)){BRT_Disp=30;}
    if((k_menu<1)||(k_menu>5)){k_menu=1;}
    step = step*k_menu;
    
}

void onConnect(IPAddress& ipaddr) {
    Serial.print("WiFi connected with ");
    Serial.print(WiFi.SSID());
    Serial.print(", IP:");
    Serial.println(ipaddr.toString());
    Wifi_connected = true;
    emergency = false;
    // if(BlynkMode){
    Blynk.config(auth);
    Blynk.connect(3333);
    CheckConnection();
}

void CheckConnection(){ 
  if(!Blynk.connected()){
    Connected2Blynk = false;
    if(BlynkMode){Blynk.connect(); }
  }
  else{
      if(BlynkMode){Connected2Blynk = true;}
  }

  if((WiFi.status() == WL_CONNECTED)&&WiFiCtrl){
      Connected2Wifi = true;
  }
  else{
      Connected2Wifi = false;
  }
}