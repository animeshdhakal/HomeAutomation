/******Code Written By Mr. Animesh Dhakal******/
#ifndef Manager_h
#define Manager_h

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>
#include <memory>

const char WEB[] PROGMEM = R"=====(
    <html>
        <head>
            <style>
                body {
            
                font-family: Arial, Helvetica, sans-serif;
            }

        .container {
            margin-top: 10vw;
            text-align: center;
            justify-content: center;
            align-items: center;


        }

        button {
            width: 17em;
            height: 2em;
            background-color: rgb(9, 137, 223);
            border: none;
            border-radius: 0.2rem;
            margin-top: 1.5em;
            color: black;
            font-size: 1em;

        }

        button,
        input {
            margin-top: 10px;
        }

        input {
            width: 20em;
            height: 2em;
            
            font-size: 0.85rem;
            
        }
        div{
            margin-top: 0.3rem;
        }
        
               
            </style>
        </head>
        <body>
            <div class="container">
                {v}
                
                <input type="text" name="ssid" placeholder="SSID" id="ssid">
                <br>
                <input type="text" name="pass" placeholder="Pass" id="pass">
                <br>
                <button onclick="btn_click()">Submit</button>
               
            </div>
            <script>
                function clicked(t) {
                    document.getElementById("ssid").value = t.innerText;
                }
                function btn_click(){
                    var xhr = new XMLHttpRequest();
                    var url = "/add?ssid="+document.getElementById("ssid").value+"&pass="+document.getElementById("pass").value;
                    xhr.onreadystatechange = function(){
                        if(this.readyState == 4 && this.status == 200){
                        
                            document.body.innerHTML = this.responseText;
                        }
                    }
                    xhr.open("GET", url);
                    xhr.send();
                }
            </script>
        </body>
    </html>
)=====";


const char portal[] PROGMEM = R"=====(
    <!DOCTYPE html>
<html lang="en">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        .container {
            margin-top: 10vw;
            justify-content: center;
            align-items: center;
            text-align: center;


        }

        button {
            width: 17em;
            height: 2em;
            background-color: rgb(9, 137, 223);
            border: none;
            border-radius: 0.2rem;
            margin-top: 1.5em;
            font-size: 1em;
            font-family: Arial, Helvetica, sans-serif;
            color: black;

        }
    </style>
</head>

<body>
    <div class="container">

        <button onclick="window.location='/wifi'">WiFi Setup</button>
        <br>
        <button onclick="window.location='/r'">Reset</button>
        <br>
        <button>Setup Blynk</button>
        <br>
        <button>Exit</button>

    </div>
</body>

</html>
)====="; 


class Manager{
    public:
        void openPortal(const char* _APNAME, const char* _APPASS);
    private:
        const byte DNS_PORT = 53;
        bool _debug = true;
        std::unique_ptr<ESP8266WebServer> server;
        std::unique_ptr<DNSServer> dnsServer;
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
        int i =0;
       
        template <typename Generic>
        void   Debug(Generic text);
};

extern Manager manager;

#endif

