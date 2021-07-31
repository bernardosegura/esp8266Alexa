#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#include <IRsend.h>
//#include <ESP8266WiFi.h>
//#include <ESP8266WebServer.h>
//#include <ArduinoJson.h>
//#include <EEPROM.h>

#include <Arduino.h>
#ifdef ESP8266 
       #include <ESP8266WiFi.h>
#endif 
#ifdef ESP32   
       #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProWindowAC.h"

#define WIFI_SSID         "Nombre de la red"    
#define WIFI_PASS         "pass de la red"
#define APP_KEY           "clave que proviene de sinric pro"      
#define APP_SECRET        "dato que proviene de sinric pro"  
#define TV_ID             "ID que proviene de sinric pro"
#define BAUD_RATE         9600

bool PowerState = false;
int temperatura = 26;
//int globalRangeValue;
//float globalTargetTemp;
 

IRsend irsend(D1);
int idDisp = COOLIX;

///////////////////////////////////////////////////////////////////////////

// AC device callbacks
bool onPowerState(const String &deviceId, bool &state) { //para apagar tendria que ser este
  
  PowerState = state; 
  Serial.printf("AC %s", state?"encendido":"apagado",temperatura);
  if(!state){
    powerOff();
  }else{
    activaTemperatura(temperatura);
    Serial.printf(" a %i grados",temperatura);
  }
  Serial.printf("\r\n");
  return true; 
}

/*bool onRangeValue(const String &deviceId, int &rangeValue) {
  Serial.printf("Device %s range value has been set to %d\r\n", rangeValue);
  return true; // request handled properly
}*/


/*bool onAdjustRangeValue(const String &deviceId, int &rangeValueDelta) {
  globalRangeValue += rangeValueDelta; // calculate absolute rangeValue
  Serial.printf("Device %s range value has been changed about %i to %d\r\n", rangeValueDelta, globalRangeValue);
  rangeValueDelta = globalRangeValue; // return absolute rangeValue
  return true; // request handled properly
}*/

bool onTargetTemperature(const String &deviceId, float &targetTemp) { //este es para poner el aire directo

temperatura = targetTemp;
temperatura = (temperatura < 17)? 17:((temperatura > 30)? 30: temperatura);

if(PowerState){
   activaTemperatura(temperatura);
}
  Serial.printf("Temperatura a %i grados\r\n", temperatura);
  return true; // request handled properly
}


/*bool onAdjustTargetTemperature(const String &deviceId, float &tempDelta) {
  globalTargetTemp += tempDelta; // change global target temperature about tempDelta
  Serial.printf("Device %s target temperature changed about %f to %f\r\n", deviceId.c_str(), tempDelta, globalTargetTemp);
  tempDelta = globalTargetTemp; // return absolute target temperature
  return true; // request handled properly
}*/

/*bool onThermostatMode(const String& deviceId, String& mode) { // modo calefaccion HEAT y Enfriamiento COOL
  Serial.printf("Deivce %s thermostat mode set to %s\r\n", deviceId.c_str(), mode.c_str());
  return true; // request handled properly
}*/


// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Conectando");
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }
  IPAddress localIP = WiFi.localIP();
  Serial.printf("conectado!\r\n[WiFi]: IP-Direccion es %d.%d.%d.%d\r\n", localIP[0], localIP[1], localIP[2], localIP[3]);
}

// setup function for SinricPro
void setupSinricPro() {
  // add device to SinricPro
  SinricProWindowAC& myAC = SinricPro[AC_ID];

  myAC.onPowerState(onPowerState);
  //myAC.onRangeValue(onRangeValue);
  //myAC.onAdjustRangeValue(onAdjustRangeValue);
  myAC.onTargetTemperature(onTargetTemperature);
  //myAC.onAdjustTargetTemperature(onAdjustTargetTemperature);
  //myAC.onThermostatMode(onThermostatMode);

  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
  SinricPro.begin(APP_KEY, APP_SECRET);
}
//////////////////////////////////////////////////////////////////////////

void setup() {
  
  Serial.begin(BAUD_RATE);
  irsend.begin();
  Serial.println("Sistema de Mando por Voz v1.0");
  Serial.print("Creando mando a Aire AC: ");
  Serial.println(idDisp);//nomDisp[idDisp]);

  setupWiFi();
  setupSinricPro();
  //setupChannelNumbers();
}

void loop() {
  //ejecutaCmd();
  SinricPro.handle();
}

void powerOff(){

  switch(idDisp){
    case COOLIX: irsend.sendCOOLIX(0xB27BE0, 24); break;
  }
}

void activaTemperatura(int temp){
    
  switch(idDisp){
    case COOLIX: int grados[] = {0xB21F08,0xB21F18,0xB21F38,0xB21F28,0xB21F68,0xB21F78,0xB21F58,0xB21F48,0xB21FC8,0xB21FD8,0xB21F98,0xB21F88,0xB21FA8,0xB21FB8}; irsend.sendCOOLIX(grados[temp-17], 24); break;
  }
}

/*void ejecutaCmd(){

  if (Serial.available() > 0) {
    
     char cmd = Serial.read(); 
    
     if(cmd == 't')
        togglePower(); 
    
    if(cmd == '+' || cmd == '-')
        volumen(cmd);
    
    if(cmd == 'm'){
        toggleMute(); 
    }
    
     if(cmd == 'c'){
        channelSkip('+'); 
    }
  
    if(cmd == 'v'){
        channelSkip('-'); 
    }

    if(cmd == 'o'){
        input(); 
    }

    if(cmd == 'i'){
        toggleInfo(); 
    }

    if(cmd == 'u'){
        toggleMenu(); 
    }

    if(cmd == 'w'){
        cursorTop(); 
    }

     if(cmd == 's'){
        cursorBottom(); 
    }

    if(cmd == 'a'){
        cursorLeft(); 
    }

     if(cmd == 'd'){
        cursorRight(); 
    }
    
    if(cmd == 'p'){
        acceptOK(); 
    }

    if(cmd == 'z'){
        tools(); 
    }

    if(cmd == 'l'){
        listChannel(); 
    }

    if(cmd == 'r'){
        retuurn(); 
    }

    if(cmd == 'b'){
        backChannel(); 
    }

    if(cmd == '0' || cmd == '1' || cmd == '2' || cmd == '3' || cmd == '4' || cmd == '5' || cmd == '6' || cmd == '7' || cmd == '8' || cmd == '9'){
      char num[] = {cmd};
      digit(atoi(num)); 
    }

    if(cmd == 'q'){
      digitInput(); 
    }

    if(cmd == 'e'){
      intro(); 
    }

    if(cmd == 'y'){
      content(); 
    }

    Serial.print(cmd);
  }
  
}*/
