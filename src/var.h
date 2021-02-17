#include <Arduino.h>

Preferences preferences;
TaskHandle_t Main_Handle;
TaskHandle_t Servise_Handle;
TaskHandle_t PCA9557_Handle;
TaskHandle_t SPWM_Handle;


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

//Encoder
const uint8_t encA = 36; 
const uint8_t encB = 39;
const uint8_t encBtn = 34;

const double FILTER_COEF = 0.1;

//var

bool emergency = false;
bool opennedMenu = false;
bool reboot = false;
bool SinMode = false;
bool PhaseMode = false;
bool ready_data = false; //DOTO

double k = 0.2;
uint8_t k_menu = 1;
int encoredVal = 0;
int encoredVal_old = 0;
double step = 0.001;

uint8_t frequency = 20;

uint8_t Power = 40;
uint8_t Power_set = 100;
uint8_t cache_Power=0;

int delay_time = 44;
double k_Freq = 0;
uint8_t BRT_Disp = 30;
uint8_t timer_0 = 0;
int V_Print = 0;

byte O1=1;
byte O2=1;
byte O3=2;
byte O4=3;
byte O5=3;
byte O6=4;

uint8_t flap_1 = 25;
uint8_t flap_2 = 25;
uint8_t flap_3 = 50;
uint8_t flap_4 = 75;
uint8_t flap_5 = 75;
uint8_t flap_6 = 100;

static uint16_t SINE_TABLE[2][483] =
{
{0,0,1,2,3,4,6,9,11,14,17,21,
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
3,2,1,0,0,0,0},
{
0,25,51,76,101,126,151,176,201,226,250,
274,299,322,346,370,393,416,438,461,483,
504,526,547,567,588,607,627,646,664,683,
700,718,734,751,767,782,797,811,825,839,
852,864,876,888,899,909,919,929,938,946,
954,962,969,975,982,987,992,997,1002,1005,
1009,1012,1015,1017,1019,1020,1022,1022,1023,
1023,1023,1023,1022,1021,1020,1018,1016,1014,
1012,1010,1007,1005,1002,999,996,993,990,986,
983,980,976,973,969,966,962,959,955,952,949,
946,943,940,937,934,931,929,926,924,922,920,
918,917,915,914,913,912,911,911,911,910,
911,911,911,912,913,914,915,917,918,920,
922,924,926,929,931,934,937,940,943,946,
949,952,955,959,962,966,969,973,976,980,
983,986,990,993,996,999,1002,1005,1007,
1010,1012,1014,1016,1018,1020,1021,1022,
1023,1023,1023,1023,1022,1022,1020,1019,1017,
1015,1012,1009,1005,1002,997,992,987,982,975,
969,962,954,946,938,929,919,909,899,888,876,
864,852,839,825,811,797,782,767,751,734,718,
700,683,664,646,627,607,588,567,547,526,504,
483,461,438,416,393,370,346,322,299,274,250,
226,201,176,151,126,101,76,51,25,0,
0,25,51,76,101,126,151,176,201,226,250,
274,299,322,346,370,393,416,438,461,483,
504,526,547,567,588,607,627,646,664,683,
700,718,734,751,767,782,797,811,825,839,
852,864,876,888,899,909,919,929,938,946,
954,962,969,975,982,987,992,997,1002,1005,
1009,1012,1015,1017,1019,1020,1022,1022,1023,
1023,1023,1023,1022,1021,1020,1018,1016,1014,
1012,1010,1007,1005,1002,999,996,993,990,986,
983,980,976,973,969,966,962,959,955,952,949,
946,943,940,937,934,931,929,926,924,922,920,
918,917,915,914,913,912,911,911,911,910,
911,911,911,912,913,914,915,917,918,920,
922,924,926,929,931,934,937,940,943,946,
949,952,955,959,962,966,969,973,976,980,
983,986,990,993,996,999,1002,1005,1007,
1010,1012,1014,1016,1018,1020,1021,1022,
1023,1023,1023,1023,1022,1022,1020,1019,1017,
1015,1012,1009,1005,1002,997,992,987,982,975,
969,962,954,946,938,929,919,909,899,888,876,
864,852,839,825,811,797,782,767,751,734,718,
700,683,664,646,627,607,588,567,547,526,504,
483,461,438,416,393,370,346,322,299,274,250,
226,201,176,151,126,101,76,51,25,0,0
}
};

