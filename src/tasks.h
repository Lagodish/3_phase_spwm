#include <Arduino.h>
#include "GyverButton.h"

GButton butt1(encBtn); // кнопка подключена сюда (BTN_PIN --- КНОПКА --- GND)

void data();
void onConnect(IPAddress& ipaddr);

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

void SPWM( void * parameter)
{
    Serial.println("SPWM");
    int Cycle = 0;

    while(1){    
    if(!emergency){
    for(Cycle = 0; Cycle <= 480; Cycle=Cycle+2){   

        if((Cycle>0)&&(Cycle<=80)){    //___1
        motor(SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle+160], 0, SINE_LOOKUP_TABLE[Cycle+80], 0); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>80)&&(Cycle<=160)){    //___2 
        motor(SINE_LOOKUP_TABLE[Cycle],0,0,0,SINE_LOOKUP_TABLE[Cycle+80],SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>160)&&(Cycle<=240)){    //___3
        motor(SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle+80],0,0,0,SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>240)&&(Cycle<=320)){    //___4
        motor(0,SINE_LOOKUP_TABLE[Cycle+80],0,SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle-80]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>320)&&(Cycle<=400)){    //___5
        motor(0,SINE_LOOKUP_TABLE[Cycle-160],SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],0,0);  //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>400)&&(Cycle<=480)){    //___6
        motor(0,0,SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle-160],0); //H1 H2 H3 L1 L2 L3
        }
        
        delayMicroseconds(delay_time);//45 mc - 50 Hz
                    
    }
        k=k+step;  
        if(k>1){k=1.0;step=0.0;}
        if(k<0){k=0.0;step=0.0;}   
    }
    else{
        ledcWrite(L1_val, 0);
        ledcWrite(L2_val, 0);
        ledcWrite(L3_val, 0);
        ledcWrite(H1_val, 0);
        ledcWrite(H2_val, 0);
        ledcWrite(H3_val, 0);
        Serial.println("Emergency stop!");
        k=0.2;
        //step = 0.0;
        vTaskDelay(1000);
    } 
    }

    Serial.println("Ending SPWM");
    vTaskDelete(Task1);
}

void isr() {
    butt1.tick();  // опрашиваем в прерывании, чтобы поймать нажатие в любом случае 
}
bool onece = true;
void Servises( void * parameter)
{   
    
    Serial.println("Servises");
    //Wifi_connected = true;
    data();
    Wire.begin();
    u8g2.begin();
    u8g2.enableUTF8Print();	
    u8g2.setFont(fontName);

    attachInterrupt(1, isr, CHANGE);

    butt1.setDebounce(80);      // настройка антидребезга (по умолчанию 80 мс)
    butt1.setTimeout(300);      // настройка таймаута на удержание (по умолчанию 500 мс)
    butt1.setType(HIGH_PULL);

    ESP32Encoder::useInternalWeakPullResistors=UP;
    encoder.attachHalfQuad(encA, encB);
    encoder.clearCount();
        
    //nav.timeOut = 1;
    nav.idleTask=MainScreen;
    nav.idleOn(MainScreen);

    while(1){
    blink++;

    butt1.tick();
    Portal.handleClient();

    if(blink>40){blink=0;}
    V_Print = int(k_Freq*220*k);

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
           // if(onece){onece=false; vTaskDelete(TaskWiFi);}
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

    Serial.println("Ending Servises");
    vTaskDelete(Task2);
}

void WiFiService( void * parameter)
{
    Serial.println("WiFiService");
 
    Config.autoReconnect = true;
    Config.title = "Controller Setup";
    Config.homeUri = "/_ac/config";
    Config.apid = "Controller_Setup";
    Config.psk = "";
    Config.bootUri = AC_ONBOOTURI_HOME;
    Config.menuItems = AC_MENUITEM_CONFIGNEW | AC_MENUITEM_RESET | AC_MENUITEM_DISCONNECT;
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
        preferences.putUInt("Frequency", 50);
        preferences.putUInt("Bright", 30);
        preferences.putUInt("StartTime", 1);
    }
    new_f = preferences.getUInt("Frequency", 0);
    BRT_Disp = preferences.getUInt("Bright", 0);
    k_menu = preferences.getUInt("StartTime", 0);

    if((new_f<10)||(new_f>70)){new_f=50;}
    if((BRT_Disp<0)||(BRT_Disp>100)){BRT_Disp=30;}
    if((k_menu<1)||(k_menu>10)){k_menu=1;}

    step = step*k_menu;
    delay_time = -37.2311 + 4098.9954 / new_f;
    if(delay_time<0){delay_time=0;}
    if(new_f>50){cache_f=50;}
    else{cache_f=new_f;}
    k_Freq = (double(map(cache_f, 10, 50, 20, 100))/100);

    preferences.end();
}

void onConnect(IPAddress& ipaddr) {
  Serial.print("WiFi connected with ");
  Serial.print(WiFi.SSID());
  Serial.print(", IP:");
  Serial.println(ipaddr.toString());
  Wifi_connected = true;
  emergency = false;
}