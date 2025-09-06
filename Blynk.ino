#define BLYNK_TEMPLATE_ID "TMPL2izjeYm8H"
#define BLYNK_TEMPLATE_NAME "PROJ"
#define BLYNK_AUTH_TOKEN "7VrrYa44KUuSv5juddMFzM7ShX3d5_PF"
//---------------------------------------------------------
#define DHTTYPE DHT11
//---------------------------------------------------------
#include<WiFiNINA.h>
#include<BlynkSimpleWiFiNINA.h>
#include <utility/wifi_drv.h>
#include<DHT.h>
//---------------------------------------------------------

int redled=25;
int greenled=26;
int blueled=27;

int bswitch = 0;
int redstatus=0;
int greenstatus=0;
int bluestatus=0;

int dhtpin=3;
int buttonpin=2;

float temptoadd=0.0;
float humtoadd=0.0;


DHT dht(dhtpin,DHTTYPE);
BlynkTimer timer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
Blynk.begin(BLYNK_AUTH_TOKEN,"Giza-Creativa","Creativa-Cu@Giza");
Serial.println("Blynk is Ready!");


WiFiDrv::pinMode(greenled,OUTPUT);
WiFiDrv::pinMode(redled,OUTPUT);
WiFiDrv::pinMode(blueled,OUTPUT);

pinMode(bswitch,INPUT_PULLUP);

dht.begin();
timer.setInterval(5000L,sendSensorData);

}

void loop() {
  // put your main code here, to run repeatedly:
Blynk.run();
timer.run();
}
BLYNK_WRITE(V0){
bswitch=param.asInt();
}

BLYNK_WRITE(V2){//red
if(bswitch){

 redstatus = param.asInt();
WiFiDrv::analogWrite(redled,redstatus);

}
}
BLYNK_WRITE(V1){//green
if(bswitch){

  greenstatus = param.asInt();
WiFiDrv::analogWrite(greenled,greenstatus);

}
}
BLYNK_WRITE(V3){//blue
if(bswitch){

 bluestatus = param.asInt();
WiFiDrv::analogWrite(blueled,bluestatus);

}
}


 BLYNK_WRITE(V12){
 //command{
 if(bswitch){
 String cmdread = param.asString();
 Serial.println("Master did : " + cmdread);
 char type = cmdread.charAt(0);
 String numStr = cmdread.substring(1);
double value=numStr.toFloat();
if(type=='t'){
  temptoadd=value;
}
if(type=='h'){
  humtoadd=value;
}
 }
 }
/*
 BLYNK_WRITE(V13){
//color
if(bswitch){
 String colorstat = param.asString();
 Serial.println("color swithced to : " +colorstat);
 }
 }
*/
/*
BLYNK_WRITE(V14){
//ledbutton
if(bswitch){
int ledbutton = param.asInt();
if(ledbutton==0){

WiFiDrv::analogWrite(redled,0);
WiFiDrv::analogWrite(greenled,0);
WiFiDrv::analogWrite(blueled,0);

}
else{
WiFiDrv::analogWrite(redled,redstatus);
WiFiDrv::analogWrite(greenled,greenstatus);
WiFiDrv::analogWrite(blueled,bluestatus);

}

}
}
*/
void sendSensorData(){
if(bswitch){

Blynk.virtualWrite(V4, redstatus);
Blynk.virtualWrite(V5, greenstatus);
Blynk.virtualWrite(V6, bluestatus);
Blynk.virtualWrite(V7, !digitalRead(buttonpin));
float temperature = dht.readTemperature();
float humidity =dht.readHumidity();
// Check if any reading failed

if (isnan(temperature) || isnan (humidity)){
Serial.println("Failed to read from DHT sensor!");

temperature=0;
humidity = 0;
}

temperature += temptoadd;
humidity += humtoadd;


Blynk.virtualWrite(V8,temperature>70.0 );

Blynk.virtualWrite(V9,humidity>70.0 );

Blynk.virtualWrite(V10,temperature);

Blynk.virtualWrite(V11,humidity);

}
}
