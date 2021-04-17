/******Code Written By Mr. Animesh Dhakal******/
#ifndef Manager_h
#define Manager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>
#include <memory>


const char HEAD[] PROGMEM                 = "<!DOCTYPE html><html lang=\"en\"><head><meta name=\"viewport\"content=\"width=device-width,initial-scale=1,user-scalable=no\" />";
const char STYLE[] PROGMEM                = "<style>body{ font-family:Avenir,Helvetica,Arial,sans-serif; -webkit-font-smoothing:antialiased; -moz-osx-font-smoothing:grayscale;text-align:center;color: #2c3e50; }a {color:black;text-decoration:none; } .container{ text-align:center;justify-content:center;align-items:center;margin-top:20vh; }button{ width:17em;height:2.3em;background:linear-gradient(135deg, #6e8efb, #a777e3);border:none;border-radius:0.3rem;margin-top:1.5em;color:white;font-weight:500;font-size:1em;box-shadow:0px 15px 20px rgba(25,26,25,0.4); }button:hover{box-shadow:0px 15px 20px rgba(0,16,247,0.4);background:linear-gradient(135deg,#a777e3, #6e8efb); }input{ width:19em;height:2em;margin-top:10px;font-size:0.90rem;border:none;border-bottom:0.5px solid rgb(11,175,216) }input:focus{ outline:none; }div{ margin-top:0.3rem; } .loader{ display:none;border:10px solid#f3f3f3;border-top:10px solid rgb(209,6,40);border-radius:50%;width:50px;height:50px;animation:spin 0.5s linear infinite; } .a::after{ background-image:url('data:image/jpeg;base64,/9j/4AAQSkZJRgABAQAAAQABAAD/2wBDAAMCAgMCAgMDAwMEAwMEBQgFBQQEBQoHBwYIDAoMDAsKCwsNDhIQDQ4RDgsLEBYQERMUFRUVDA8XGBYUGBIUFRT/2wBDAQMEBAUEBQkFBQkUDQsNFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBQUFBT/wAARCAAQABADASIAAhEBAxEB/8QAFgABAQEAAAAAAAAAAAAAAAAABQcI/8QAJBAAAQQCAgIBBQAAAAAAAAAAAQIDBAUGBxESCCEAFCIxMmH/xAAUAQEAAAAAAAAAAAAAAAAAAAAA/8QAFBEBAAAAAAAAAAAAAAAAAAAAAP/aAAwDAQACEQMRAD8A1r5G+WV/rG/tKPGceqoTFOhlVrmedzHK2jiuPI7tMMlKFOzHSn2Usg9fwTzyAF4+eZmT7EyKrrrulxzLqSzlCC3lus5j82JAklJUhudFeQmRGSoJVw6odOf5yRd9z6dqtr0bbrkCtdyunZkvY3ZWsb6lmtnuMlDcgsn7HOququFA/qCPfwnQmlF65pYl1lSKa12nMgNw7/JqiCmILHotSkcoSlIPUKCe3UFXUEgegA//2Q==');background-repeat:no-repeat;background-position:0px 3.1px;background-size:19px 19px;display:inline-block;width:19px;height:19px;content: \"\"; } @keyframes spin{ 0% {transform:rotate(0deg); }100% {transform:rotate(360deg); } } </style>";
const char HEAD_END[] PROGMEM             = "</head><body>";
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
        bool _debug = true;
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
        template <typename Generic>
        void   Debug(Generic text);
};

extern Manager manager;

#endif

