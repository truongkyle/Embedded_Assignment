#include <ESP8266WiFi.h>
#include <SocketIOClient.h>
#include <ArduinoJson.h>
#include<Servo.h>
#include "DHT.h"            
 
const int DHTTYPE = DHT11;  //Khai báo loại cảm biến, có 2 loại là DHT11 và DHT22
 
DHT dht(D3, DHTTYPE);

// define servo
Servo MyServo1, MyServo2;

SocketIOClient client;
const char* ssid     = "Vuon Dau 1";
const char* password = "CDVuonDau20183110";

char host[] = "bkhappyfish.herokuapp.com";
int port = 80;
char namespace_nodeMCU[] = "NodeMCU";
extern String RID;
extern String Rfull;

unsigned long previousMillis = 0;
long interval = 5000;
unsigned long lastreply = 0;
unsigned long lastsend = 0;

boolean check = false;
int a =1;
StaticJsonBuffer<200> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();
JsonObject& rootprocess = jsonBuffer.createObject();
JsonObject& rootGetInter = jsonBuffer.createObject();




void setup() {
  
  Serial.begin(115200);
  delay(10);
  // define pinMode Servo
  MyServo1.attach(D1);
  MyServo1.write(0);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(155);
  delay(1000);
  MyServo2.attach(D2);
  MyServo2.write(180);
  delay(1000);
  //start sensor dht11
  dht.begin();
// connect Wifi 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  if (!client.connect(host, port, namespace_nodeMCU)) {
    Serial.println("connection failed");
    return;
  } else {
      Serial.println("connect Server ");
    }
}
void listening(){
  if(client.monitor()){
      Serial.println(RID);
        if(RID == "stop"){
          check = true;
          Serial.println("STOP.....................");
        } 
      }
  
  }

void Delay(float delayTime, void (func)()){
    unsigned long endTime = millis() + delayTime;
    while(millis() < endTime)
    {
      func();
    }
}

void ProcessFeeding(int Numbers){
  Serial.println(Numbers);
  for(int i = 0; i<Numbers; i++){
      listening();
      if(check) {
        check = false;
        return;
      }
        String JSON;
//        int Temp = random(20,40);
        int Temp = dht.readTemperature();
        rootprocess["Numbers"] = i+1;
        rootprocess["Temperature"] = Temp ;
        rootprocess.printTo(JSON);  
        Serial.println(JSON); 
        client.sendJSON("Feeding", JSON);
        



      MyServo1.attach(D1);
      MyServo1.write(22);
      Delay(1000,listening);
      MyServo1.write(0);
      Delay(1000,listening);
      MyServo2.attach(D2);
      MyServo2.write(155);
      Delay(1000,listening);
      MyServo2.write(180);
      Delay(1000,listening);
       if(check) {
        check = false;
        return;
      }

    }
  
}

void loop() {
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis > interval)
  { 
    String JSON;
//    int Temp = random(20,40);
    int Temp = dht.readTemperature();

    root["Temperature"] = Temp;
    root.printTo(JSON);
   
    Serial.println(JSON);
    previousMillis = currentMillis;
    client.sendJSON("Data", JSON);
    lastsend = millis();
  }
  if (client.monitor())
  {
    if(RID == "start"){
      Serial.println(Rfull);
      JsonObject& rootstart =jsonBuffer.parseObject(Rfull);
      if(!rootstart.success()){
        Serial.println("parseObject() failed");
        return;
        }
       int Numbers = rootstart["Numbers"];
       ProcessFeeding(Numbers);
     } 


   if(RID == "IntervalTime"){
      Serial.println(Rfull);
      JsonObject& rootInter =jsonBuffer.parseObject(Rfull);
      if(!rootInter.success()){
        Serial.println("parseObject() rootInter failed");
        return;
        }
       interval = rootInter["IntervalTime"];
     } 

     if(RID == "GetInterval"){

      String JSONget;
      rootGetInter["IntervalTime"] = interval;
      rootGetInter.printTo(JSONget);  
      Serial.println(JSONget); 
      client.sendJSON("intervaltime", JSONget);
      
     } 
   
  }

  if (!client.connected()) {
      Serial.println("disconnect sserver");
      client.reconnect(host, port, namespace_nodeMCU);
    }
    
}
