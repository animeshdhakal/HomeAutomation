/******Code Written By Mr. Animesh Dhakal******/
#ifndef Manager_h
#define Manager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <memory>


const char HEAD[] PROGMEM                 = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\" />";
const char STYLE[] PROGMEM                = "<style>body{ font-family:Avenir,Helvetica,Arial,sans-serif; -webkit-font-smoothing:antialiased; -moz-osx-font-smoothing:grayscale;text-align:center;color: #2c3e50; }a {color:black;text-decoration:none; } .container{ text-align:center;justify-content:center;align-items:center;margin-top:20vh; }button{ width:17em;height:2.3em;background:linear-gradient(135deg, #6e8efb, #a777e3);border:none;border-radius:0.3rem;margin-top:1.5em;color:white;font-weight:500;font-size:1em;box-shadow:0px 15px 20px rgba(25,26,25,0.4); }button:hover{box-shadow:0px 15px 20px rgba(0,16,247,0.4);background:linear-gradient(135deg,#a777e3, #6e8efb); }input{ width:19em;height:2em;margin-top:10px;font-size:0.90rem;border:none;border-bottom:0.5px solid rgb(11,175,216) }input:focus{ outline:none; }div{ margin-top:0.3rem; } .loader{ display:none;border:10px solid#f3f3f3;border-top:10px solid rgb(209,6,40);border-radius:50%;width:50px;height:50px;animation:spin 0.5s linear infinite; } @keyframes spin{ 0% {transform:rotate(0deg); }100% {transform:rotate(360deg); } } </style>";
const char HEAD_END[] PROGMEM             = "</head><body>";
const char ICON[] PROGMEM                 = "<svg style=\"margin-left:0.3em;\"width=\"18\"height=\"18\"viewBox=\"0 0 24 17\"><path d=\"M8.213 16.984c.97-1.028 2.308-1.664 3.787-1.664s2.817.636 3.787 1.664l-3.787 4.016-3.787-4.016zm-1.747-1.854c1.417-1.502 3.373-2.431 5.534-2.431s4.118.929 5.534 2.431l2.33-2.472c-2.012-2.134-4.793-3.454-7.864-3.454s-5.852 1.32-7.864 3.455l2.33 2.471zm-4.078-4.325c2.46-2.609 5.859-4.222 9.612-4.222s7.152 1.613 9.612 4.222l2.388-2.533c-3.071-3.257-7.313-5.272-12-5.272s-8.929 2.015-12 5.272l2.388 2.533z\"></path></svg>";
const char WIFI[] PROGMEM                 = "<div class=\"container\"> {v} <div style=\"margin-top:1em;\"><input type=\"text\"name=\"ssid\"placeholder=\"SSID\"id=\"ssid\"><br><input type=\"text\"name=\"pass\"placeholder=\"Password\"id=\"pass\"><br><button onclick=\"btn_click()\">Submit</button></div></div>";
const char PORTAL[] PROGMEM               = "<div class=\"container\"><button onclick=\"window.location='/wifi'\">WiFi Setup</button><br><button onclick=\"window.location='/r'\">Reset</button><br><button onclick=\"window.location='/u'\">Updates</button><br><button onclick=\"window.location='/exit'\">Exit</button></div>";
const char UPDATE[] PROGMEM               = "<div class=\"container\"><center><div class=\"loader\"id=\"Cloader\"></div></center><button onclick=\"check()\">Check For Updates</button></div>";
const char UPDATE_FOUND[] PROGMEM         = "<div class=\"container\"><center><div class=\"loader\"id=\"Uloader\"></div></center><div id=\"info\"><b>Update Found</b><br><b>Current Version: {CV}</b><br><b>Update Version: {UV}</b></div><button onclick=\"update()\">Update</button></div>";
const char SCRIPT[] PROGMEM               = "<script>function clicked(t) {document.getElementById(\"ssid\").value= t.innerText; }function btn_click() {var url= \"/add?ssid=\" +document.getElementById(\"ssid\").value+ \"&pass=\" +document.getElementById(\"pass\").value;call(url,function(d) {document.body.innerHTML= d; }); }function call(url,callback) {var xhr= new XMLHttpRequest();xhr.onreadystatechange= function() {if(this.readyState==4 &&this.status==200) {callback(this.responseText); } };xhr.open(\"GET\",url);xhr.send(); }function check() {document.getElementById(\"Cloader\").style.display= \"block\";call(\"/u?update=check\",function(d) {document.body.innerHTML= d; }); }function update() {document.getElementById(\"Uloader\").style.display= \"block\";document.getElementById(\"info\").style.display= \"none\";call('/u?update=update',function(d) {document.body.innerHTML= d; }); } </script>";
const char END[] PROGMEM                  = "</body></html>";

const String Version = "T.1";

class Manager{
    public:
        void openPortal(const char* _APNAME, const char* _APPASS);
    private:
        String OTA_HOST = "update-server-ad.herokuapp.com";
        int OTA_PORT = 80;
        const byte DNS_PORT = 53;
        char *APNAME;
        char *APPASS;
        std::unique_ptr<ESP8266WebServer> server;
        std::unique_ptr<DNSServer> dnsServer;
        std::unique_ptr<WiFiClient> client;
        void startServer();
        int getRSSIasQuality(int RSSI);
        void connect(const char* ssid,const char* pass);
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
};

extern Manager manager;

#endif

