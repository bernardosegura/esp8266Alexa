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
#include "SinricProTV.h"

#define WIFI_SSID         "Nombre de la red"    
#define WIFI_PASS         "pass de la red"
#define APP_KEY           "clave que proviene de sinric pro"      
#define APP_SECRET        "dato que proviene de sinric pro"  
#define TV_ID             "ID que proviene de sinric pro"  
#define BAUD_RATE         9600

bool tvPowerState = false;
unsigned int tvVolume;
unsigned int tvChannel;
bool tvMuted = false;
bool startedOn = true;
bool startedMute = true;

IRsend irsend(D1);
int idDisp = SAMSUNG;


///////////////////////////////////////////////////////////////////////////

// TV device callbacks

bool onAdjustVolume(const String &deviceId, int &volumeDelta, bool volumeDefault) {
  tvVolume += volumeDelta;  // calcualte new absolute volume
  if(volumeDelta < 0){
    
    for(int v=0; v < (volumeDelta * -1); v++){
      volumen('-');
      delay(250);
    }
     //volumen('-');
     //Serial.printf("Volume Down\r\n");
  }else{
    for(int v=0; v < volumeDelta; v++){
      volumen('+');
      delay(250);
    }
  }
  
  Serial.printf("Volumen cambiado la cantidad de %i\r\n", volumeDelta);
  volumeDelta = tvVolume; // return new absolute volume
  return true;
}

/*bool onChangeChannel(const String &deviceId, String &channel) {
  //tvChannel = channelNumbers[channel]; // save new channelNumber in tvChannel variable
  Serial.printf("Change channel to \"%s\" (channel number %d)\r\n", channel.c_str(), tvChannel);
  return true;
}*/

bool onChangeChannelNumber(const String& deviceId, int channelNumber, String& channelName) {
  char canal[4]=" ";
  tvChannel = channelNumber; // update tvChannel to new channel number
  //if (tvChannel < 0) tvChannel = 0;
  //if (tvChannel > MAX_CHANNELS-1) tvChannel = MAX_CHANNELS-1;

  itoa(tvChannel,canal,10);
  //sprintf(canal,"%d",tvChannel);
  channelName = canal;//channelNames[tvChannel]; // return the channelName

  for(int c=0; c < channelName.length(); c++){
    char num[]={channelName[c]};
    Serial.printf("%s \r\n", num);
    digit(atoi(num));
    delay(250);
  }
  Serial.printf("Cambiando a canal %d \r\n", tvChannel);
  //Serial.printf("Change to channel to %d (channel name \"%s\" %c %i)\r\n", tvChannel,canal,channelName[0], channelName.length());// channelName.c_str());
  return true;
}

/*bool onMediaControl(const String &deviceId, String &control) {
  Serial.printf("MediaControl: %s\r\n", control.c_str());
  if (control == "Play") {}           // do whatever you want to do here
  if (control == "Pause") {}          // do whatever you want to do here
  if (control == "Stop") {}           // do whatever you want to do here
  if (control == "StartOver") {}      // do whatever you want to do here
  if (control == "Previous") {}       // do whatever you want to do here
  if (control == "Next") {}           // do whatever you want to do here
  if (control == "Rewind") {}         // do whatever you want to do here
  if (control == "FastForward") {}    // do whatever you want to do here
  return true;
}*/

bool onMute(const String &deviceId, bool &mute) {
  
  if(tvMuted != mute || startedMute)
    toggleMute();
  
  tvMuted = mute; // set tvMuted state

  startedMute = false;
  
  Serial.printf("TV volumen es %s\r\n", mute?"sin sonido":"con sonido");
  return true;
}

bool onPowerState(const String &deviceId, bool &state) {
  
  if(tvPowerState != state || startedOn){
    togglePower();

    if(state){
      tvMuted = false;
    }
    
  }
    
  tvPowerState = state; // set powerState

  startedOn  = false;
  
  Serial.printf("TV %s\r\n", state?"encendido":"apagado");
  return true; 
}

bool onSelectInput(const String &deviceId, String &inputName){
  if(strstr(inputName.c_str(), "ARRIBA")){
    input();
    delay(250);
    cursorTop();
    delay(250);
    intro();
  }else
    if(strstr(inputName.c_str(), "ABAJO")){
      input();
      delay(250);
      cursorBottom();
      delay(250);
      intro();
    }else{
      input();
      delay(250);
      input();
      delay(250);
      intro();
    }
  
  Serial.printf("Cambiando entrada %s\r\n", inputName.c_str());
  return true;
}

bool onSetVolume(const String &deviceId, int &volume) {
  int vol = volume; // - tvVolume;
  /*if(vol < 0){
    vol = (vol * -1);
    for(int v=0; v < vol; v++){
      volumen('-');
      delay(500);
    }
  }else{
    for(int v=0; v < vol; v++){
      volumen('+');
      delay(500);
    }
  }*/
  if(volume < 50){
    
    for(int v=0; v < vol; v++){
      volumen('-');
      delay(250);
    }
     //volumen('-');
     //Serial.printf("Volume Down\r\n");
  }else{
    vol -= 50;
    for(int v=0; v < vol; v++){
      volumen('+');
      delay(250);
    }
     //volumen('+');
     //Serial.printf("Volume Up\r\n");
  }
  
  tvVolume = volume; // update tvVolume
  Serial.printf("Volume puesto a:  %i veces %i\r\n", volume,vol);
  return true;
}

bool onSkipChannels(const String &deviceId, const int channelCount, String &channelName) {
  /*tvChannel += channelCount; // calculate new channel number
  if (tvChannel < 0) tvChannel = 0;
  //if (tvChannel > MAX_CHANNELS-1) tvChannel = MAX_CHANNELS-1;
  channelName = "Siguiente";// String(channelNames[tvChannel]); // return channel name*/
  if(channelCount < 0){
    channelSkip('-');
     Serial.printf("Cambiar hacia abajo canal\r\n");
  }else{
    channelSkip('+');
    Serial.printf("Cambiar hacia arriba canal\r\n");
  }

  //Serial.printf("Skip channel: %i (number: %i / name: \"%s\")\r\n", channelCount, tvChannel, channelName.c_str());
  
  return true;
}

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
  SinricProTV& myTV = SinricPro[TV_ID];

  // set callback functions to device
  myTV.onAdjustVolume(onAdjustVolume);//sube-baja volumen/ n veces/ una unidad 
//  myTV.onChangeChannel(onChangeChannel);
  myTV.onChangeChannelNumber(onChangeChannelNumber);// cambia canal numero 
//  myTV.onMediaControl(onMediaControl);
  myTV.onMute(onMute);// silencia calla
  myTV.onPowerState(onPowerState);//reactiva sonido
  myTV.onSelectInput(onSelectInput); //cambia entrada tv/televisin arriba-abajo
  myTV.onSetVolume(onSetVolume); // cambia 10 % volumen
  myTV.onSkipChannels(onSkipChannels);// cambia canal/ sube-baja canal

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
  Serial.print("Creando mando a TV: ");
  Serial.println(idDisp);//nomDisp[idDisp]);

  setupWiFi();
  setupSinricPro();
  //setupChannelNumbers();
}

void loop() {
  //ejecutaCmd();
  SinricPro.handle();
}

void ejecutaCmd(){

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
  
}

void togglePower(){

  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E040BF, 32); break;
  }
}

void volumen(char vol){

  switch(idDisp){
    case SAMSUNG:  if(vol == '+'){
                        irsend.sendSAMSUNG(0xE0E0E01F, 32); 
                    }
                    if(vol == '-'){
                        irsend.sendSAMSUNG(0xE0E0D02F, 32); 
                    }break;
  }
 
}

void toggleMute(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0F00F, 32); break;
  }
}

void channelSkip(char chn){

  switch(idDisp){
    case SAMSUNG:  if(chn == '+'){
                        irsend.sendSAMSUNG(0xE0E048B7, 32); 
                    }
                    if(chn == '-'){
                        irsend.sendSAMSUNG(0xE0E008F7, 32); 
                    }break;
  }
 
}

void input(){//tv,hdmi1..etc
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0807F, 32); break;
  }
}

void toggleInfo(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0F807, 32); break;
  }
}

void toggleMenu(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E058A7, 32); break;
  }
}

void cursorTop(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E006F9, 32); break;
  }
}

void cursorBottom(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E08679, 32); break;
  }
}

void cursorRight(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xEE0E046B9, 32); break;
  }
}

void cursorLeft(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0A659, 32); break;
  }
}

void acceptOK(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E016E9, 32); break;
  }
}

void tools(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0D22D, 32); break;
  }
}

void listChannel(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0D629, 32); break;
  }
}

void retuurn(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E01AE5, 32); break;
  }
}

void backChannel(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0C837, 32); break;
  }
}

void digit(int num){
  switch(idDisp){
    case SAMSUNG: int digits[] = {0xE0E08877,0xE0E020DF,0xE0E0A05F,0xE0E0609F,0xE0E010EF,0xE0E0906F,0xE0E050AF,0xE0E030CF,0xE0E0B04F,0xE0E0708F}; irsend.sendSAMSUNG(digits[num], 32); break;
  }
}

void digitInput(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E0C43B, 32); break;
  }
}

void intro(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E016E9, 32); break;
  }
}

void content(){
  switch(idDisp){
    case SAMSUNG: irsend.sendSAMSUNG(0xE0E09E61, 32); break;
  }
}
