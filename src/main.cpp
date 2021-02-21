#include <Arduino.h>
#include <ESP8266WiFi.h>
#define BLYNK_PRINT Serial
#include <LittleFS.h>
#include <BlynkSimpleEsp8266.h>
#include "Manager.h"

#define R1 D5
#define R2 D6
#define R3 D7
#define R4 D8

#define S1 D0
#define S2 D2
#define S3 D3
#define S4 D4

#define trigger_pin D1

int mode=0;

int flag1=0;
int flag2=0;
int flag3=0;
int flag4=0;

int st1=0;
int st2=0;
int st3=0;
int st4=0;

char auth[]="nlJHegSIKdIJWqf66_0KqnvuFVIz_8qM";

int toInt(char a){
  return a - '0';
}

void writeBlynk(){
  File file = LittleFS.open("./config.json", "r");
  char data[file.size()];
  file.readBytes(data, file.size());
  file.close();
  Blynk.virtualWrite(V1, toInt(data[0]));
  Blynk.virtualWrite(V2, toInt(data[1]));
  Blynk.virtualWrite(V3, toInt(data[2]));
  Blynk.virtualWrite(V4, toInt(data[3]));
}

void writeFS(){
  File file = LittleFS.open("./config.json", "w");
  String data;
  data += st1;
  data += st2;
  data += st3;
  data += st4;
  file.write(data.c_str());
  file.close();
}

BLYNK_CONNECTED(){
  Serial.println("Connected to the server");
  writeBlynk();
  mode = 1; 
}

BLYNK_DISCONNECTED(){
  Serial.println("Disconnected");
  mode = 0;
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
    file.write("1111");
    
  }
  char data[file.size()];
  file.readBytes(data, file.size());
  file.close();
  st1 = toInt(data[0]);
  st2 = toInt(data[1]);
  st3 = toInt(data[2]);
  st4 = toInt(data[3]);
  
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
  while(WiFi.status() != WL_CONNECTED)
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
  if(mode==1){withInternet();}else{withoutInternet();}
  checkBtn();
}