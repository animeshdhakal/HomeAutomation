/******Code Written By Mr. Animesh Dhakal******/
#ifndef Manager_h
#define Manager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <memory>


extern "C"{
    #include "user_interface.h"
}
const char HEAD[] PROGMEM                 = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\" />";
const char STYLE[] PROGMEM                = "<style>body{ font-family:Arial,Helvetica,sans-serif; } .container{ margin-top:10vw;text-align:center;justify-content:center;align-items:center; }button{ width:17em;height:2em;background-color:rgb(9,137,223);border:none;border-radius:0.2rem;margin-top:1.5em;color:black;font-size:1em; }input{ width:19.5em;height:2em;margin-top:10px;font-size:0.85rem; }div{ margin-top:0.3rem; } </style>";
const char HEAD_END[] PROGMEM             = "</head><body>";
const char WIFI[] PROGMEM                 = "<div class=\"container\"> {v} <input type=\"text\"name=\"ssid\"placeholder=\"SSID\"id=\"ssid\"><br><input type=\"text\"name=\"pass\"placeholder=\"Pass\"id=\"pass\"><br><button onclick=\"btn_click()\">Submit</button></div>";
const char PORTAL[] PROGMEM               = "<div class=\"container\"><button onclick=\"window.location='/wifi'\">WiFi Setup</button><br><button onclick=\"window.location='/r'\">Reset</button><br><button onclick=\"window.location='/u'\">Updates</button><br><button>Exit</button></div>";
const char SCRIPT[] PROGMEM               = "<script>function clicked(t) {document.getElementById(\"ssid\").value= t.innerText; }function btn_click() {var xhr= new XMLHttpRequest();var url= \"/add?ssid=\" +document.getElementById(\"ssid\").value+ \"&pass=\" +document.getElementById(\"pass\").value;xhr.onreadystatechange= function() {if(this.readyState==4 &&this.status==200) {document.body.innerHTML= this.responseText; } };xhr.open(\"GET\",url);xhr.send(); } </script>";
const char END[] PROGMEM                  = "</body></html>";


class Manager{
    public:
        void openPortal(const char* _APNAME, const char* _APPASS);
    private:
        const byte DNS_PORT = 53;
        bool _debug = true;
        std::unique_ptr<ESP8266WebServer> server;
        std::unique_ptr<DNSServer> dnsServer;
        std::unique_ptr<ESP8266HTTPUpdateServer> httpUpdate;
        std::unique_ptr<WiFiClient> client;
        void startServer();
        int getRSSIasQuality(int RSSI);
        void pageOpened(const char* ssid,const char* pass);
        void handleRoot();
        void handleReset();
        void handleWiFi();
        void handleSave();
        void handleNotFound();
        boolean captivePortal();
        String toStringIp(IPAddress ip);
        boolean isIp(String str);
        int w = 0;
       
        template <typename Generic>
        void   Debug(Generic text);
};

extern Manager manager;

#endif

