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

void Manager::connect(String &ssid, String &pass)
{
  Print("Connecting with " + ssid);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  for (int i = 0; i < 100; i++)
  {
    if (WiFi.status() == WL_CONNECTED)
      break;
    Log(".");
    delay(100);
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Print("Connected ! Stopping Portal");
    w = 1;
  }
  else
  {
    Print("Connection Unsuccessfull ! Restarting Portal");
    openPortal(APNAME, APPASS);
  }
}

void Manager::openPortal(char *_APNAME, char *_APPASS)
{
  Print("Starting Config Portal");

  APNAME = _APNAME;
  APPASS = _APPASS;
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_station_disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAP(_APNAME, _APPASS);
  }
  else
  {
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP(APNAME, APPASS);
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
  server->on("/exit", std::bind(&Manager::handleExit, this));
  server->on("/info", std::bind(&Manager::handleInfo, this));
  server->on("/u", std::bind(&Manager::handleUpdateRoot, this));
  server->onNotFound(std::bind(&Manager::handleNotFound, this));

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
  if (captivePortal())
    return;
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
  server->send(200, "text/html", "<center style='margin-top: 20vh;'><h1>Bye Bye. See You Soon</h1></center>");
  delay(100);
  w = 1;
  WiFi.mode(WIFI_STA);
  WiFi.begin();
}
void Manager::handleWiFi()
{
  if (captivePortal())
    return;
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
    item += "%</b>";
    item += FPSTR(ICON);
    item += "</div>";
  }
  page.replace("{v}", item);
  page += FPSTR(SCRIPT);
  page += FPSTR(END);

  server->send(200, "text/html", page);
}

void Manager::handleNotFound()
{
  if (captivePortal())
  {
    return;
  }
  String message = F("File Not Found\n\n");
  message += F("URI: ");
  message += server->uri();
  server->sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server->sendHeader("Pragma", "no-cache");
  server->sendHeader("Expires", "-1");
  server->send(404, "text/plain", message);
}

void Manager::handleUpdateRoot()
{
  if (captivePortal())
    return;
  if (server->arg("update") == "check")
  {
    if (WiFi.status() == WL_CONNECTED)
    {

      HTTPClient http;
      http.begin(*client, OTA_HOST, OTA_PORT, "/");
      http.setUserAgent(F("Animesh"));
      http.addHeader(F("TYPE"), F("ESP"));
      int status = http.GET();
      String payload = http.getString();
      if (status != 200)
      {
        server->send(200, "text/plain", "Server Not Responding Properly");
        return;
      }

      http.end();
      if (ESP.getSketchMD5() != payload)
      {
        String page = FPSTR(HEAD);
        page.replace("{t}", "Updater");
        page += FPSTR(STYLE);
        page += FPSTR(HEAD_END);
        page += FPSTR(UPDATE_FOUND);
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
    WiFiClient a;
    http.begin(*client, OTA_HOST, OTA_PORT, "/u");
    http.setUserAgent(F("Animesh"));
    http.addHeader(F("TYPE"), F("ESP"));
    int httpcode = http.GET();
    uint32_t size = http.getSize();
    WiFiClient *tcp = http.getStreamPtr();

    Print("Update Starting");

    if (!Update.begin(size))
    {
      server->send(200, "text/plain", "Error " + (String)Update.getError());
      Print("Error1: " + (String)Update.getError());
      return;
    }
    int written = 0;
    int prevProgress = 0;
    uint32_t timeout;
    uint8_t buff[256];
    while (tcp->connected() && written < size)
    {
      delay(10);
      timeout = millis();
      while (tcp->connected() && !tcp->available())
      {
        delay(1);
        if (millis() - timeout > 10000L)
        {
          Print("Timeout");
        }
      }

      int len = tcp->read(buff, sizeof(buff));
      if (len <= 0)
        continue;

      Update.write(buff, len);
      written += len;

      const int progress = (written * 100) / size;
      if (progress - prevProgress >= 10 || progress == 100)
      {
        Print(String("\r ") + progress + "%");
        prevProgress = progress;
      }
    }

    if (!Update.end(true))
    {
      server->send(200, "text/plain", "Error " + (String)Update.getError());
      Print("Error3: " + (String)Update.getError());
      return;
    }

    if (!Update.isFinished())
    {
      server->send(200, "text/plain", F("Update not finished"));
      return;
    }

    http.end();
    server->send(200, "text/html", "Update Success. Rebooting Esp");
    delay(1000);
    ESP.restart();
    delay(1000);
    ESP.reset();
    while (1)
    {
    };
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
  delay(100);
  connect(ssid, pass);
}

void Manager::handleInfo()
{
  String page;
  page += "<style>body{ font-family:Avenir,Helvetica,Arial,sans-serif; -webkit-font-smoothing:antialiased; -moz-osx-font-smoothing:grayscale;text-align:center;color: #2c3e50;}</style>";
  page += "<h3>Firmware Version: " + Version + "</h3>";
  page += "<h3>Free Heap: " + (String)ESP.getFreeHeap() + " Bytes</h3>";
  page += "<h3>Free SketchSize: " + (String)ESP.getFreeSketchSpace() + " Bytes</h3>";
  page += "<h3>Sdk Version: " + (String)system_get_sdk_version() + "</h3>";
  page += "<h3>Chip ID: " + String(ESP.getChipId(), HEX) + "</h3>";
  page += "<h3>Flash Chip ID: " + (String)ESP.getFlashChipId() + "</h3>";
  page += "<h3>Flash Chip Size: " + (String)ESP.getFlashChipRealSize() + "</h3>";
  page += "<h3>CPU Frequency: " + (String)ESP.getCpuFreqMHz() + " MHz</h3>";
  page += "<h3>Boot Version: " + (String)system_get_boot_version() + "</h3>";
  page += "<h3>Last Reset Reason: " + (String)ESP.getResetInfo() + "</h3>";
  page += "<h3>Flash Chip Speed: " + (String)ESP.getFlashChipSpeed() + "</h3>";
  page += "<h3>Currently Stored SSID: " + (String)WiFi.SSID() + "</h3>";
  page += "<h3>Connection Status: " + String(WiFi.status() == WL_CONNECTED ? "Connected" : "Not Connected") + "</h3>";
  page += "<h3>WiFi IP: " + toStringIp(WiFi.localIP()) + "</h3>";
  page += "<h3>AP IP: " + toStringIp(WiFi.softAPIP()) + "</h3>";

  server->send(200, "text/html", page);
}

boolean Manager::captivePortal()
{
  if (!isIp(server->hostHeader()))
  {
    Print("Request redirected to captive portal");
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

Manager manager;