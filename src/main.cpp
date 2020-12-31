#include <Arduino.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <LittleFS.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoJson.h>
#include "Manager.h"

#define Log(x) Serial.println(x)
#define R1 D5
#define R2 D6
#define R3 D7
#define R4 D8

#define S1 D0
#define S2 D2
#define S3 D3
#define S4 D4

#define trigger_pin D1

int mode=2;

int flag1=0;
int flag2=0;
int flag3=0;
int flag4=0;

int st1=0;
int st2=0;
int st3=0;
int st4=0;


char auth[]="nlJHegSIKdIJWqf66_0KqnvuFVIz_8qM";

void writeBlynk(){
  File file = LittleFS.open("./config.json", "r");
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();
  StaticJsonDocument<200> doc;
  deserializeJson(doc, buf.get());
  Blynk.virtualWrite(V1, doc["R1"].as<int>());
  Blynk.virtualWrite(V2, doc["R2"].as<int>());
  Blynk.virtualWrite(V3, doc["R3"].as<int>());
  Blynk.virtualWrite(V4, doc["R4"].as<int>());
  
  
}

void writeFS(){
  File file = LittleFS.open("./config.json", "w");
  StaticJsonDocument<200> doc;
  doc["R1"]=st1;
  doc["R2"]=st2;
  doc["R3"]=st3;
  doc["R4"]=st4;
  serializeJson(doc, file);
  file.close();
}

BLYNK_CONNECTED(){
  Serial.println("Connected to the server");
  writeBlynk();
  mode = 0;
  
}

BLYNK_DISCONNECTED(){
  Log("Disconnected");
  mode = 1;
}

BLYNK_WRITE(V1){
  st1=param.asInt();
  digitalWrite(R1, st1);
  writeFS();
}

BLYNK_WRITE(V2){
  st2=param.asInt();
  digitalWrite(R2, st2);
  writeFS();
}

BLYNK_WRITE(V3){
  st3=param.asInt();
  digitalWrite(R3, st3);
  writeFS();
}
BLYNK_WRITE(V4){
  st4=param.asInt();
  digitalWrite(R4, st4);
  writeFS();
}

void readFS(){
  File file = LittleFS.open("./config.json", "r");
  if(!file){
    file.close();
    File file = LittleFS.open("./config.json", "w");
    file.write("{\"R1\":0,\"R2\":0,\"R3\":0,\"R4\":0}");
    
  }
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();
  StaticJsonDocument<200> doc;
  deserializeJson(doc, buf.get());
  st1=doc["R1"];
  st2=doc["R2"];
  st3=doc["R3"];
  st4=doc["R4"];
  
}

void withInternet(){
  
  if (digitalRead(S1) == LOW)
  {
    if (flag1 == 0)
    {
      digitalWrite(R1, LOW);
      Blynk.virtualWrite(V1, 0);
      flag1 = 1;
      writeFS();
    }
  }
  if (digitalRead(S1) == HIGH)
  {
    if (flag1 == 1)
    {
      digitalWrite(R1, HIGH);
      Blynk.virtualWrite(V2, 1);
      flag1 = 0;
      writeFS();
    }
  }
  if (digitalRead(S2) == LOW)
  {
    if (flag2 == 0)
    {
      digitalWrite(R2, LOW);
      Blynk.virtualWrite(V2, 0);
      flag2 = 1;
      writeFS();
    }
  }
  if (digitalRead(S2) == HIGH)
  {
    if (flag2 == 1)
    {
      digitalWrite(R2, HIGH);
      Blynk.virtualWrite(V2, 1);
      flag2 = 0;
      writeFS();
    }
  }
  if (digitalRead(S3) == LOW)
  {
    if (flag3 == 0)
    {
      digitalWrite(R3, LOW);
      Blynk.virtualWrite(V3, 0);
      flag3 = 1;
      writeFS();
    }
  }
  if (digitalRead(S3) == HIGH)
  {
    if (flag3 == 1)
    {
      digitalWrite(R3, HIGH);
      Blynk.virtualWrite(V3, 1);
      flag3 = 0;
      writeFS();
    }
  }
  if (digitalRead(S4) == LOW)
  {
    if (flag4 == 0)
    {
      digitalWrite(R4, LOW);
      Blynk.virtualWrite(V4, 0);
      flag4 = 1;
      writeFS();
    }
  }
  if (digitalRead(S4) == HIGH)
  {
    if (flag4 == 1)
    {
      digitalWrite(R4, HIGH);
      Blynk.virtualWrite(V4, 1);
      flag4 = 0;
      writeFS();
    }
  }
}
void withoutInternet(){
  
  //Relay1
  if(digitalRead(S1)==LOW){
    if(flag1==0){
      digitalWrite(R1, LOW);
      st1=0;
      flag1=1;
      writeFS();
    }
  }
  if(digitalRead(S1)==HIGH){
    if(flag1==1){
      digitalWrite(R1, HIGH);
      st1=1;
      flag1=0;
      writeFS();
    }
  }
  //Relay2
  if(digitalRead(S2)==LOW){
    if(flag2==0){
      digitalWrite(R2, LOW);
      st2=0;
      flag2=1;
      writeFS();
    }
  }
  if(digitalRead(S2)==HIGH){
    if(flag2==1){
      digitalWrite(R2, HIGH);
      st2=1;
      flag2=0;
      writeFS();
    }
  }
  //Relay3
  if(digitalRead(S3)==LOW){
    if(flag3==0){
      digitalWrite(R3, LOW);
      st3=0;
      flag3=1;
      writeFS();
    }
  }
  if(digitalRead(S3)==HIGH){
    if(flag3==1){
      digitalWrite(R3, HIGH);
      st3=1;
      flag3=0;
      writeFS();
    }
  }
  //Relay4
  if(digitalRead(S4)==LOW){
    if(flag4==0){
      digitalWrite(R4, LOW);
      st4=0;
      flag4=1;
      writeFS();
    }
  }
  if(digitalRead(S4)==HIGH){
    if(flag4==1){
      digitalWrite(R4, HIGH);
      st4=1;
      flag4=0;
      writeFS();
    }
  }

}

void checkBtn(){
  if(digitalRead(trigger_pin)==LOW){
    manager.openPortal("animeshdhakall", "animeshdhakal");
  }
}
void setup(){
  Serial.begin(115200);
  LittleFS.begin();
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  pinMode(trigger_pin, INPUT_PULLUP);

  readFS();
  digitalWrite(R1, st1);
  digitalWrite(R2, st2);
  digitalWrite(R3, st3);
  digitalWrite(R4, st4);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin();
  while (WiFi.status()!=WL_CONNECTED)
  {
    Serial.print("*");
    checkBtn();
    withoutInternet();
    delay(100);
  }
  
  Blynk.config(auth);
  
}

void loop(){
  Blynk.run();
  if(mode==0){withInternet();}else{withoutInternet();}
  checkBtn();
}