#include <Arduino.h>

const int L1 = 33;  
const int L2 = 26;
const int L3 = 17; 
const int H1 = 16;  
const int H2 = 25;
const int H3 = 27;  
const int shunt = 32;

// setting PWM properties
const int freq = 24000;
const int L1_val = 0;
const int L2_val = 1;
const int L3_val = 2;
const int H1_val = 3;
const int H2_val = 4;
const int H3_val = 5;
const int resolution = 10;

bool oneUse = false;
bool emergency = true;
bool WiFiCtrl = false;
bool PhaseMode = false;
bool BlynkMode = false;
bool Connected2Blynk = false;
bool Connected2Wifi = false;

double k = 0.2;
int k_menu = 1;
int encoredVal = 0;
int encoredVal_old = 0;
bool Wifi_connected = false;
double step = 0.001;

double new_freq = 50;
int frequency = 50;

int Power = 30;
int Power_set = 100;

int delay_time = 44;
double k_Freq = 0;
int BRT_Disp = 30;
int timer_0 = 0;
int timer_1 = 0;
int V_Print = 0;
int cache_Power=0;

//Encoder
#define encA 36 
#define encB 39
//Encoder button
#define encBtn 34

char auth[] = "Vb4KJvlA_4OvjxxblOtyc2Zq4eVC1iJt";
#define FILTER_COEF 0.1
