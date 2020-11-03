#include <BlynkSimpleEsp8266.h>
#include <WiFiManager.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>


#define BLYNK_PRINT DebugSerial


BlynkTimer timer;
WiFiManager wifimanager;





char auth[] = "nlJHegSIKdIJWqf66_0KqnvuFVIz_8qM";
char ssid[] = "unique nepal";
char pass[] = "UNIQUENEPAL";
int mode = 0;

#define R1 D5
#define R2 D6
#define R3 D7
#define R4 D8

#define S1 D0
#define S2 D2
#define S3 D3
#define S4 D4



#define trigger D1

int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;

int s1 = 0;
int s2 = 0;
int s3 = 0;
int s4 = 0;

void BlynkCon(){
  Blynk.virtualWrite(V1, 1);
  Blynk.virtualWrite(V2, 1);
  Blynk.virtualWrite(V3, 1);
  Blynk.virtualWrite(V4, 1);
}
void ReadFS(){
  File file = LittleFS.open("./config.json", "r");
  if(!file){
    file.close();
    File file = LittleFS.open("./config.jsom", "w");
    file.write("{\"R1\":0,\"R2\":0,\"R3\":0,\"R4\":0}");
  }
  
  while(file.available()){
    Serial.write(file.read());
  }
  
  size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  StaticJsonDocument<200> data;
  deserializeJson(data, buf.get());
  digitalWrite(R1, data["R1"].as<int>());
  digitalWrite(R2, data["R2"].as<int>());
  digitalWrite(R3, data["R3"].as<int>());
  digitalWrite(R4, data["R4"].as<int>());
  

}


BLYNK_CONNECTED(){
  BlynkCon();
  Blynk.syncVirtual(V1, V2, V3, V4);
  Serial.println("Connected to Server");
}

BLYNK_WRITE(V1)
{
  s1 = param.asInt();
  digitalWrite(R1, s1);
}

BLYNK_WRITE(V2)
{
  s2  = param.asInt();
  digitalWrite(R2, s2);
}
BLYNK_WRITE(V3)
{
  s3 = param.asInt();
  digitalWrite(R3, s3);
}
BLYNK_WRITE(V4)
{
  s4 = param.asInt();
  digitalWrite(R4, s4);
}



void WriteFS(){
  File file = LittleFS.open("./config.json", "w");
  StaticJsonDocument<200> doc;
  doc["R1"]=s1;
  doc["R2"]=s2;
  doc["R3"]=s3;
  doc["R4"]=s4;
  serializeJson(doc, file);
  file.close();
}

void checkPush(){
  if (digitalRead(trigger) == LOW)
  {
    wifimanager.startConfigPortal("Animesh");
  }
  
  

}

void checkBlynk()
{
  bool connected = Blynk.connected();
  if (connected)
  {
    mode = 0;
  }
  else
  {
    mode = 1;
  }
}

void with_internet()
{
  if (digitalRead(S1) == LOW)
  {
    if (flag1 == 0)
    {
      digitalWrite(R1, LOW);
      Blynk.virtualWrite(V1, 0);
      flag1 = 1;
    }
  }
  if (digitalRead(S1) == HIGH)
  {
    if (flag1 == 1)
    {
      digitalWrite(R1, HIGH);
      Blynk.virtualWrite(V1, 1);
      flag1 = 0;
    }
  }

  if (digitalRead(S2) == LOW)
  {
    if (flag2 == 0)
    {
      digitalWrite(R2, LOW);

      Blynk.virtualWrite(V2, 0);
      flag2 = 1;
    }
  }
  if (digitalRead(S2) == HIGH)
  {
    if (flag2 == 1)
    {
      digitalWrite(R2, HIGH);

      Blynk.virtualWrite(V2, 1);
      flag2 = 0;
    }
  }

  if (digitalRead(S3) == LOW)
  {
    if (flag3 == 0)
    {
      digitalWrite(R3, LOW);

      Blynk.virtualWrite(V3, 0);
      flag3 = 1;
    }
  }
  if (digitalRead(S3) == HIGH)
  {
    if (flag3 == 1)
    {
      digitalWrite(R3, HIGH);

      Blynk.virtualWrite(V3, 1);
      flag3 = 0;
    }
  }

  if (digitalRead(S4) == LOW)
  {
    if (flag4 == 0)
    {
      digitalWrite(R4, LOW);

      Blynk.virtualWrite(V4, 0);
      flag4 = 1;
    }
  }
  if (digitalRead(S4) == HIGH)
  {
    if (flag4 == 1)
    {
      digitalWrite(R4, HIGH);

      Blynk.virtualWrite(V4, 1);
      flag4 = 0;
    }
  }
}

void without_internet()
{
  digitalWrite(R1, digitalRead(S1));
  digitalWrite(R2, digitalRead(S2));
  digitalWrite(R3, digitalRead(S3));
  digitalWrite(R4, digitalRead(S4));
}

void setup()
{
  Serial.begin(115200);
  if(LittleFS.begin()){
    Serial.println("Mounted the File System");
  }else{
    Serial.println("Error!!");
  }

  
  pinMode(R1, OUTPUT);
  pinMode(R2, OUTPUT);
  pinMode(R3, OUTPUT);
  pinMode(R4, OUTPUT);

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
  pinMode(S3, INPUT_PULLUP);
  pinMode(S4, INPUT_PULLUP);
  pinMode(trigger, INPUT_PULLUP);

  
  
  ReadFS();
  
  // wifimanager.autoConnect("Animesh");
  WiFi.begin(ssid, pass);
  Serial.println(WiFi.psk());
  Serial.println(WiFi.SSID());
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print("*");
    checkPush();
    // without_internet();
    delay(100);
  }
  Serial.println("Youtube");
  Blynk.config(auth);
  timer.setInterval(3000L, checkBlynk);
}

void loop()
{
  Blynk.run();
  timer.run();
  if(mode == 0){with_internet();}else{without_internet();}
  WriteFS();
  checkPush();
  Serial.println(s3);
}