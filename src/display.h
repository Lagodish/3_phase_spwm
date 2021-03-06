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
    if((Power_set<40)||(Power_set>130)){  Power_set = 100;}

    preferences.begin("FrequencyData", false);
    preferences.putUInt("Power", Power_set);
    preferences.end();
    

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
    if((k_menu>=1)&&(k_menu<=4)){
    preferences.begin("FrequencyData", false);
    preferences.putUInt("StartTime", k_menu);
    preferences.end();
    }
    return proceed;
}


result action5(eventMask e,navNode& nav, prompt &item) {

    preferences.begin("FrequencyData", false);
    preferences.putBool("PhaseMode", PhaseMode);
    preferences.end();
    reboot = true;

    return proceed;
}


result action7(eventMask e,navNode& nav, prompt &item) {
    
    preferences.begin("FrequencyData", false);
    preferences.putBool("SinMode", SinMode);
    preferences.end();
    
    return proceed;
}

result action8(eventMask e,navNode& nav, prompt &item) {
    O1 = flap_1 / 25;
    O2 = flap_2 / 25;
    O3 = flap_3 / 25;
    O4 = flap_4 / 25;
    O5 = flap_5 / 25;
    O6 = flap_6 / 25;    
    return proceed;
}

TOGGLE(PhaseMode,setPhase,"Phase: ",action5,enterEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("3",HIGH,doNothing,noEvent)
  ,VALUE("1",LOW,doNothing,noEvent)
);


TOGGLE(SinMode,setSin,"Sin Mode: ",action7,enterEvent,noStyle//,doExit,enterEvent,noStyle
  ,VALUE("SPWM",HIGH,doNothing,noEvent)
  ,VALUE("THIPWM",LOW,doNothing,noEvent)
);

MENU(subMenu, "Flaps", doNothing, anyEvent, noStyle
  ,FIELD(flap_1,"Out 1"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  ,FIELD(flap_2,"Out 2"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  ,FIELD(flap_3,"Out 3"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  ,FIELD(flap_4,"Out 4"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  ,FIELD(flap_5,"Out 5"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  ,FIELD(flap_6,"Out 6"," %",0,100,25,0,action8, enterEvent, wrapStyle)
  , EXIT("<Back")
);

MENU(mainMenu, "Menu" ,doNothing,noEvent,noStyle
  ,FIELD(Power_set,"Power"," %",40,130,1,0,action1,enterEvent,noStyle)
  ,FIELD(k_menu,"StartTime","X",1,4,1,0,action3,enterEvent,noStyle)
  ,SUBMENU(setPhase)
  ,SUBMENU(setSin)
  ,SUBMENU(subMenu)
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
    case idleStart:{opennedMenu = true;break;}
    case idling:{
    opennedMenu = false;
    
    u8g2.setFont(u8g2_font_fub20_tf);
    o.setCursor(0,1);
    o.print(frequency); 
    o.setCursor(5,1);
    o.print("Hz");   

    o.setCursor(0,3);
    o.print(V_Print);
    if(V_Print>99){o.setCursor(7,3);} 
    else{o.setCursor(5,3);}
    o.print("V");   
    if(timer_0<20){
    u8g2.setFont(u8g2_font_open_iconic_all_4x_t);
    u8g2.drawUTF8( (16 * 6), 43, "\u00AA");} //x y top left
    u8g2.setFont(u8g2_font_open_iconic_all_2x_t);
    break;}
    case idleEnd:{u8g2.setFont(fontName);opennedMenu = true;break;}
  }

  return proceed;
}