/******Code Written By Mr. Animesh Dhakal******/
#ifndef Manager_h
#define Manager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <memory>


const char HEAD[] PROGMEM                 = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\" />";
const char STYLE[] PROGMEM                = "<style>body{ font-family:Arial,Helvetica,sans-serif; } .container{ text-align:center;justify-content:center;align-items:center;margin-top:20vh; }button{ width:17em;height:2.3em;background-color:rgb(9,137,223);border:none;border-radius:0.2rem;margin-top:1.5em;color:white;font-weight:500;font-size:1em; }input{ width:19.3em;height:2em;margin-top:10px;font-size:0.85rem; }div{ margin-top:0.3rem; } .loader{ display:none;border:10px solid#f3f3f3;border-top:10px solid rgb(209,6,40);border-radius:50%;width:50px;height:50px;animation:spin 0.5s linear infinite; } @keyframes spin{ 0% {transform:rotate(0deg); }100% {transform:rotate(360deg); } } </style>";
const char HEAD_END[] PROGMEM             = "</head><body>";
const char WIFI[] PROGMEM                 = "<div class=\"container\"> {v} <input type=\"text\"name=\"ssid\"placeholder=\"SSID\"id=\"ssid\"><br><input type=\"text\"name=\"pass\"placeholder=\"Password\"id=\"pass\"><br><button onclick=\"btn_click()\">Submit</button></div>";
const char PORTAL[] PROGMEM               = "<div class=\"container\"><button onclick=\"window.location='/wifi'\">WiFi Setup</button><br><button onclick=\"window.location='/r'\">Reset</button><br><button onclick=\"window.location='/checkUpdate'\">Updates</button><br><button onclick=\"window.location='/exit'\">Exit</button></div>";
const char UPDATE[] PROGMEM               = "<div class=\"container\"><center><div class=\"loader\"id=\"Cloader\"></div></center><button onclick=\"check()\">Check For Updates</button></div>";
const char UPDATE_FOUND[] PROGMEM         = "<div class=\"container\"><center><div class=\"loader\"id=\"Uloader\"></div></center><div id=\"info\"><b>Update Found</b><br><b>Current Version: {CV}</b><br><b>Update Version: {UV}</b></div><button onclick=\"update()\">Update</button></div>";
const char SCRIPT[] PROGMEM               = "<script>function clicked(t) {document.getElementById(\"ssid\").value= t.innerText; }function btn_click() {var url= \"/add?ssid=\" +document.getElementById(\"ssid\").value+ \"&pass=\" +document.getElementById(\"pass\").value;call(url,document.body); }function call(url,where) {var xhr= new XMLHttpRequest();xhr.onreadystatechange= function() {if(this.readyState==4 &&this.status==200) {where.innerHTML= this.responseText; } };xhr.open(\"GET\",url);xhr.send(); }function check(){document.getElementById(\"Cloader\").style.display= \"block\";call(\"/u?update=check\",document.body); }function update(){document.getElementById(\"Uloader\").style.display= \"block\";document.getElementById(\"info\").style.display= \"none\";call('/u?update=update',document.body); } </script>";
const char END[] PROGMEM                  = "</body></html>";

const String Version = "T.1";

class Manager{
    public:
        void openPortal(const char* _APNAME, const char* _APPASS);
    private:
        String OTA_HOST = "update-server-ad.herokuapp.com";
        int OTA_PORT = 80;
        const byte DNS_PORT = 53;
        bool _debug = true;
        std::unique_ptr<ESP8266WebServer> server;
        std::unique_ptr<DNSServer> dnsServer;
        std::unique_ptr<WiFiClient> client;
        void startServer();
        int getRSSIasQuality(int RSSI);
        void pageOpened(const char* ssid,const char* pass);
        void handleRoot();
        void handleExit();
        void handleWiFi();
        void handleCheckUpdate();
        void handleUpdateRoot();
        void handleInfo();
        void handleSave();
        boolean captivePortal();
        String toStringIp(IPAddress ip);
        boolean isIp(String str);
        int w = 0;
        template <typename Generic>
        void   Debug(Generic text);
};

extern Manager manager;

#endif

