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

bool emergency = false;

double k = 0.2;
int k_menu = 1;
int encoredVal = 0;
int encoredVal_old = 0;

double step = 0.001;
int new_f = 50;
int delay_time = 44;
double k_Freq = 1;
int BRT_Disp = 30;
int blink = 0;
int V_Print = 0;
int cache_f=0;

//Encoder
#define encA 36 
#define encB 39
//Encoder button
#define encBtn 34

