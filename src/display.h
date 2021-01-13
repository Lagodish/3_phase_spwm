#include <Arduino.h>
#include <Wire.h>
#include <menu.h>
#include <menuIO/u8g2Out.h>
#include <menuIO/chainStream.h>
//#include <menuIO/serialOut.h>
#include <menuIO/serialIn.h>
#include <ESP32Encoder.h>

ESP32Encoder encoder;

serialIn serial(Serial);
MENU_INPUTS(in,&serial);

using namespace Menu;
#define MAX_DEPTH 2

#define fontName u8g2_font_7x13_t_cyrillic //u8g2_font_7x13_mf
#define fontX 7
#define fontY 16
#define offsetX 0
#define offsetY 0
#define U8_Width 128
#define U8_Height 64
#define USE_HWI2C

U8G2_SH1106_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0,22,21,U8X8_PIN_NONE);
typedef u8g2_uint_t u8g_uint_t;

const colorDef<uint8_t> colors[6] MEMMODE={
  {{0,0},{0,1,1}},//bgColor
  {{1,1},{1,0,0}},//fgColor
  {{1,1},{1,0,0}},//valColor
  {{1,1},{1,0,0}},//unitColor
  {{0,1},{0,0,1}},//cursorColor
  {{1,1},{1,0,0}},//titleColor
};

result action1(eventMask e,navNode& nav, prompt &item) {
    delay_time = -37.2311 + 4098.9954 / new_f;
    if(delay_time<0){delay_time=0;}
    if(new_f>50){cache_f=50;}
    else{cache_f=new_f;}
    k_Freq = (double(map(cache_f, 10, 50, 20, 100))/100);

    if((new_f>9)&&(new_f<71)){
    preferences.begin("FrequencyData", false);
    preferences.putUInt("Frequency", new_f);
    preferences.end();
    }

    return proceed;
}

result action2(eventMask e,navNode& nav, prompt &item) {

    if((BRT_Disp>=0)&&(BRT_Disp<=100)){
    preferences.begin("FrequencyData", false);
    preferences.putUInt("Bright", BRT_Disp);
    preferences.end();
    }

    return proceed;
}

result action3(eventMask e,navNode& nav, prompt &item) {

    if((k_menu>=1)&&(k_menu<=10)){
    preferences.begin("FrequencyData", false);
    preferences.putUInt("StartTime", k_menu);
    preferences.end();
    }

    return proceed;
}

MENU(mainMenu, "Menu" ,doNothing,noEvent,noStyle
  ,FIELD(new_f,"Freq"," Hz",10,70,1,0,action1,enterEvent,noStyle)
  ,FIELD(k_menu,"StartTime","X",1,10,1,0,action3,enterEvent,noStyle)
  ,FIELD(BRT_Disp,"Bright"," %",0,100,10,0,action2,enterEvent,noStyle)
  ,EXIT("Back")
);

MENU_OUTPUTS(out,MAX_DEPTH
  ,U8G2_OUT(u8g2,colors,fontX,fontY,offsetX,offsetY,{0,0,U8_Width/fontX,U8_Height/fontY})
  ,NONE
);

NAVROOT(nav,mainMenu,MAX_DEPTH,in,out);
const char Storm_SYMBOL[] = { 0xAA, '\0' };
//when menu is suspended
result MainScreen(menuOut& o,idleEvent e) {
  o.clear();
   switch(e) {
    case idleStart:{break;}
    case idling:{
    if(Wifi_connected){
    u8g2.setFont(u8g2_font_fub20_tf);
    o.setCursor(0,1);
    o.print(new_f); 
    o.setCursor(5,1);
    o.print("Hz");   

    o.setCursor(0,3);
    o.print(V_Print);
    if(V_Print>99){o.setCursor(7,3);} 
    else{o.setCursor(5,3);}
    o.print("V");   
    if(blink<20){
    u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
    u8g2.drawUTF8( (16 * 6), (48 * 1), "\u00AA");}} //x y top left
    else{
    u8g2.setFont(u8g2_font_fub20_tf);
    o.setCursor(1,1);
    o.print("No Wi-Fi");   
    u8g2.setFont(fontName);
    o.setCursor(0,2);
    o.print("Press btn to start");  
    o.setCursor(0,3);
    o.print("or join ESP WiFi");  
    }
    break;}
    case idleEnd:{u8g2.setFont(fontName);break;}
  }

  return proceed;
}