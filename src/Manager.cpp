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
    w = 1;
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
    WiFi.softAP(_APNAME, _APPASS);
  }

  startServer();
}

void Manager::startServer()
{
  delay(500);
  Debug(F("AP IP : "));
  Debug(WiFi.softAPIP());
  server.reset(new ESP8266WebServer(80));
  dnsServer.reset(new DNSServer);
  dnsServer->start(DNS_PORT, "*", WiFi.softAPIP());
  server->on("/", std::bind(&Manager::handleRoot, this));
  server->on("/wifi", std::bind(&Manager::handleWiFi, this));
  server->on("/add", std::bind(&Manager::handleSave, this));
  server->on("/exit", std::bind(&Manager::handleExit, this));
  server->on("/info", std::bind(&Manager::handleInfo, this));
  server->on("/u", std::bind(&Manager::handleUpdateRoot, this));  
  server->onNotFound(std::bind(&Manager::handleRoot, this));
  server->begin();
  while (w == 0)
  {
    dnsServer->processNextRequest();
    server->handleClient();
    yield();
  }
  server->stop();
  server.reset();
  dnsServer->stop();
  dnsServer.reset();
  w = 0;
}

void Manager::handleRoot()
{
  if (captivePortal()) return;
  String page = FPSTR(HEAD);
  page.replace("{t}", "Manager Portal");
  page += FPSTR(STYLE);
  page += FPSTR(HEAD_END);
  page += FPSTR(PORTAL);
  page += FPSTR(END);
  server->send(200, "text/html", page);
}

void Manager::handleExit()
{
  w = 1;
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}

void Manager::handleWiFi()
{
  if (captivePortal()) return;
  String page = FPSTR(HEAD);
  page.replace("{t}", "Manager");
  page += FPSTR(STYLE);
  page += FPSTR(HEAD_END);
  page += FPSTR(WIFI);
  String item;
  int n = WiFi.scanNetworks();
  int indices[n];
  for (int i = 0; i < n; i++)
  {
    indices[i] = i;
  }

  for (int i = 0; i < n; i++)
  {
    for (int j = i + 1; j < n; j++)
    {
      if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i]))
      {
        std::swap(indices[i], indices[j]);
      }
    }
  }

  for (int i = 0; i < n; i++)
  {
    int quality = getRSSIasQuality(WiFi.RSSI(indices[i]));
    item += "<div class='a'>";
    item += "<b><a href=\"#\" onclick=\"clicked(this)\">";
    item += WiFi.SSID(indices[i]);
    item += "</a></b><b> ";
    item += quality;
    item += "%";
    item += "</b></div>";
  }
  page.replace("{v}", item);
  page += FPSTR(SCRIPT);
  page += FPSTR(END);

  server->send(200, "text/html", page);
}

void Manager::handleUpdateRoot()
{
  if (captivePortal()) return;
  if (server->arg("update") == "check")
  {
    if (WiFi.status() == WL_CONNECTED)
    {

      HTTPClient http;
      http.begin(OTA_HOST, OTA_PORT, "/");
      http.setUserAgent(F("Animesh"));
      http.addHeader(F("TYPE"), F("ESP"));
      int status = http.GET();
      String payload = http.getString();
      if (status != 200)
      {
        Debug("Server Not Responding Properly");
        Debug(payload);
        server->send(200, "text/plain", "Server Not Responding Properly");
        return;
      }
      Debug(Version);
      Debug(payload);
      http.end();
      if (Version != payload)
      {
        String page = FPSTR(HEAD);
        page.replace("{t}", "Updater");
        page += FPSTR(STYLE);
        page += FPSTR(HEAD_END);
        page += FPSTR(UPDATE_FOUND);
        page.replace("{CV}", Version);
        page.replace("{UV}", payload);
        page += FPSTR(SCRIPT);
        page += FPSTR(END);
        server->send(200, "text/html", page);
      }
      else
      {
        server->send(200, "text/html", "You are up to date");
      }
    }
    else
    {
      server->send(200, "text/plain", "Please Connect to the WiFi");
    }
  }
  else if (server->arg("update") == "update")
  {
    HTTPClient http;
    Debug("Update Starting");
    http.begin(OTA_HOST, OTA_PORT, "/u");
    http.setUserAgent(F("Animesh"));
    http.addHeader(F("TYPE"), F("ESP"));
    int httpcode = http.GET();
    int size = http.getSize();
    Debug(httpcode);
    Debug(size);
    WiFiClient *tcp = http.getStreamPtr();
    Update.runAsync(true);
  
    if (!Update.begin(size))
    {
      Debug("Update begin Failed");
      server->send(200, "text/plain", "Error " + (String)Update.getError());
      Update.end();
      return;
    }
    if (Update.writeStream(*tcp) != size)
    {
      Debug("Update writing Failed");
      server->send(200, "text/html", "Error" + (String)Update.getError());
      Update.end();
      return;
    }
    
    if (!Update.end(true))
    {
      Debug("Update Failed");
      server->send(200, "text/plain", "Error " + (String)Update.getError());
      Update.end();
      return;
    }
    Debug("Update Successs");
    http.end();
    server->send(200, "text/html", "Update Success. Rebooting Esp");
    delay(1000);
    ESP.reset();
  }
  else
  {
    String page = FPSTR(HEAD);
    page.replace("{t}", "Updater");
    page += FPSTR(STYLE);
    page += FPSTR(HEAD_END);
    page += FPSTR(UPDATE);
    page += FPSTR(SCRIPT);
    page += FPSTR(END);
    server->send(200, "text/html", page);
  }
}

void Manager::handleSave()
{
  String ssid = server->arg("ssid");
  String pass = server->arg("pass");
  server->send(200, "text/plain", "<style>p{color: red;}</style><center><h2 style='margin-top: 20vh'>Credentials Received By ESP</h2></center>");
  delay(5000);
  pageOpened(ssid.c_str(), pass.c_str());
}

void Manager::handleInfo()
{
  String page;
  page += "<style>body{ font-family:Avenir,Helvetica,Arial,sans-serif; -webkit-font-smoothing:antialiased; -moz-osx-font-smoothing:grayscale;text-align:center;color: #2c3e50;}</style>";
  page += "<h3>Firmware Version: " + Version + "</h3>";
  page += "<h3>Free Heap: " + (String)ESP.getFreeHeap() + "</h3>";
  page += "<h3>Free SketchSize: " + (String)ESP.getFreeSketchSpace() + "</h3>";
  page += "<h3>Sdk Version: " + (String)system_get_sdk_version() + "</h3>";
  page += "<h3>Chip ID: " + String(ESP.getChipId(), HEX) + "</h3>";
  page += "<h3>Flash Chip ID: " + (String)ESP.getFlashChipId() + "</h3>";
  page += "<h3>Flash Chip Size: " + (String)ESP.getFlashChipRealSize() + "</h3>";
  page += "<h3>CPU Frequency: " + (String)ESP.getCpuFreqMHz() + "</h3>";
  page += "<h3>Boot Version: " + (String)system_get_boot_version() + "</h3>";
  page += "<h3>Core Version: " + ESP.getCoreVersion() + "</h3>";
  page += "<h3>Last Reset Reason: " + (String)ESP.getResetInfo() + "</h3>";
  page += "<h3>Vcc: " + (String)ESP.getVcc() + "</h3>";
  page += "<h3>Md5 Hash of Sketch: " + (String)ESP.getSketchMD5() + "</h3>";
  page += "<h3>Flash Chip Speed: " + (String)ESP.getFlashChipSpeed() + "</h3>";
  
  server->send(200, "text/html", page);
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

  Serial.print("**M: ");
  Serial.println(text);
}
Manager manager;