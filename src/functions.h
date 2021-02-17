#include <Arduino.h>
const int NUM_READ = 20;  // количество усреднений для средних арифм. фильтров

float filter(float newVal) {
  static int t = 0;
  static int vals[NUM_READ];
  static int average = 0;
  if (++t >= NUM_READ) t = 0; // перемотка t
  average -= vals[t];         // вычитаем старое
  average += newVal;          // прибавляем новое
  vals[t] = newVal;           // запоминаем в массив
  return ((float)average / NUM_READ);
}

void data(){
    preferences.begin("FrequencyData", false);
    Power_set = preferences.getUInt("Power", 100); 
    BRT_Disp = preferences.getUInt("Bright", 50);
    k_menu = preferences.getUInt("StartTime", 1);
    PhaseMode = preferences.getBool("PhaseMode", true);
    SinMode = preferences.getBool("SinMode", true);
    preferences.end();

    if((BRT_Disp<0)||(BRT_Disp>100)){BRT_Disp=30;}
    if((k_menu<1)||(k_menu>4)){k_menu=1;}
    step = step*k_menu;
    ready_data = true;
}

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
    ledcWrite(H1_val, H1_); //H1
    ledcWrite(H2_val, H2_); //H2
}