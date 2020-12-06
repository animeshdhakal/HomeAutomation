/******Code Written By Mr. Animesh Dhakal******/
#include "Manager.h"

int Manager::getRSSIasQuality(int RSSI)
{
  int quality = 0;
  if (RSSI <= -100)
  {
    quality = 0;
  }
  else if (RSSI >= -50)
  {
    quality = 100;
  }
  else
  {
    quality = 2 * (RSSI + 100);
  }
  return quality;
}

void Manager::pageOpened(const char *ssid, const char *pass)
{

  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin(ssid, pass);
  for (int i = 0; i < 100; i++)
  {
    Serial.print("*");
    if (WiFi.status() == WL_CONNECTED)
      break;
    delay(100);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Debug("Connected");
   
    delay(100);
    i = 1;
  }
  else
  {
    Debug("Connection time out starting the AP");
    openPortal(WiFi.softAPSSID().c_str(), WiFi.softAPPSK().c_str());
  }
}

void Manager::openPortal(const char *_APNAME, const char *_APPASS)
{

  Debug("Starting Config Portal");
  if (WiFi.status() != WL_CONNECTED)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_APNAME, _APPASS);
  }
  else
  {
    WiFi.mode(WIFI_AP_STA);
  }

  startServer();
}

void Manager::startServer()
{
  server.reset(new ESP8266WebServer(80));
  dnsServer.reset(new DNSServer);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());

  server->on("/", std::bind(&Manager::handleRoot, this));
  server->on("/wifi", std::bind(&Manager::handleWiFi, this));
  server->on("/add", std::bind(&Manager::handleSave, this));
  server->on("/r", std::bind(&Manager::handleReset, this));
  server->onNotFound(std::bind(&Manager::handleNotFound, this));
  
  server->begin();

  while (i == 0)
  {
    dnsServer->processNextRequest();
      
    server->handleClient();
    
  }
  Debug("We are in the reset sectuion");
  server.reset();
  dnsServer.reset();
}

void Manager::handleRoot(){
  if (captivePortal())
    {
      return;
    }
  
  server->send_P(200, "text/html", portal);
}

void Manager::handleReset(){
  ESP.reset();
  delay(2000);
}

void Manager::handleWiFi()
{
  
  String page = FPSTR(WEB);
  String item;
  int n = WiFi.scanNetworks();
  int indices[n];
  for (int i = 0; i < n; i++)
  {
    indices[i] = i;
  }
  for (int i = 0; i < n; i++)
  {
    int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));
    item+="<div>";
    item += "<a href=\"#\" onclick=\"clicked(this)\">";
    item += WiFi.SSID(i);
    item += "</a>";
    item += " ";
    item += quality;
    item += "%";
    item += "<div/>";
  }
  page.replace("{v}", item);
  
  server->send(200, "text/html", page);
}

void Manager::handleSave()
{

  String ssid = server->arg("ssid");
  String pass = server->arg("pass");
  server->send(200, "text/plain", "<style>p{color: red;}</style><center><h2>ESP GOT SSID</h2></center>");
  delay(5000);
  pageOpened(ssid.c_str(), pass.c_str());
}

void Manager::handleNotFound()
{
  if (captivePortal())
  {
    return;
  }
  
  server->send(404, "text/plain", "Not Found");
}

boolean Manager::captivePortal()
{
  if (!isIp(server->hostHeader()))
  {
    Debug("Request redirected to captive portal");
    server->sendHeader("Location", String("http://") + toStringIp(server->client().localIP()), true);
    server->send(302, "text/plain", "");
    server->client().stop();
    return true;
  }
  return false;
}

boolean Manager::isIp(String str)
{
  for (size_t i = 0; i < str.length(); i++)
  {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9'))
    {
      return false;
    }
  }
  return true;
}

String Manager::toStringIp(IPAddress ip)
{
  String res = "";
  for (int i = 0; i < 3; i++)
  {
    res += String((ip >> (8 * i)) & 0xFF) + ".";
  }
  res += String(((ip >> 8 * 3)) & 0xFF);
  return res;
}
template <typename Generic>
void Manager::Debug(Generic text)
{
  if (_debug)
  {
    Serial.print("**M: ");
    Serial.println(text);
  }
}
Manager manager;