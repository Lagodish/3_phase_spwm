#include <Arduino.h>

double k = 1.0;
double step = 0.0;
int delay_time = 4;
int timeout = 0;

void motor(int H1_,int H2_,int H3_, int L1_,int L2_,int L3_){
    H1_=H1_*k; 
    H2_=H2_*k;
    H3_=H3_*k;
    L1_=L1_*k;
    L2_=L2_*k;
    L3_=L3_*k; 
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

    while(1){
    if(!emergency){
    for(int Cycle = 0; Cycle <= 480; Cycle++){   

        if((Cycle>=0)&&(Cycle<=80)){    //___1
        motor(SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle+160], 0, SINE_LOOKUP_TABLE[Cycle+80], 0); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>80)&&(Cycle<=160)){    //___2 
        motor(SINE_LOOKUP_TABLE[Cycle],0,0,0,SINE_LOOKUP_TABLE[Cycle+80],SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        }
        
        if((Cycle>160)&&(Cycle<=240)){    //___3
        motor(SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle+80],0,0,0,SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>240)&&(Cycle<=320)){    //___4
        motor(0,SINE_LOOKUP_TABLE[Cycle+80],0,SINE_LOOKUP_TABLE[Cycle],0,SINE_LOOKUP_TABLE[Cycle+160]); //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>320)&&(Cycle<=400)){    //___5
        motor(0,SINE_LOOKUP_TABLE[Cycle+80],SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],0,0);  //H1 H2 H3 L1 L2 L3
        }

        if((Cycle>=400)&&(Cycle<=480)){    //___6
        motor(0,0,SINE_LOOKUP_TABLE[Cycle-80],SINE_LOOKUP_TABLE[Cycle],SINE_LOOKUP_TABLE[Cycle-160],0); //H1 H2 H3 L1 L2 L3
        }

        delayMicroseconds(delay_time);
        
        if(Cycle==480){
        timeout++;
        if(timeout==500){ step = -0.01; } //Плавно тормозим на 500
        if((k<0.51)&&(timeout<1500)){ step = 0; k=0.5; } // Ждем пока замедляемся
        if(timeout==1500){ k=0; } //На 1500 полная остановка 
        if(timeout==2000){ k = 0.5; } //На 2000 периудах старт резкий до 25 герц (половина напряжения) и бесконечный цикл
        k += step; 
        if(k>1){k=1;}
        if(k<0){k=0;}
        delay_time = map(k*100, 0.0, 100.0, 90, 3);
        }
        
     }
    }
    else{
        ledcWrite(L1_val, 0);
        ledcWrite(L2_val, 0);
        ledcWrite(L3_val, 0);
        ledcWrite(H1_val, 0);
        ledcWrite(H2_val, 0);
        ledcWrite(H3_val, 0);
        Serial.println("Emergency stop!");
        k=0.0;
        step = 0.0;
        vTaskDelay(1000);
    }
    }

    Serial.println("Ending SPWM");
    vTaskDelete(Task1);
}

void Servises( void * parameter)
{
    Serial.println("Servises");
    while(1){
        Serial.print("Servises running on core ");
        Serial.println(xPortGetCoreID());
        ArduinoOTA.handle();
        vTaskDelay(1000);
    }

    Serial.println("Ending Servises");
    vTaskDelete(Task2);
}