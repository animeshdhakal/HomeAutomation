import os
import requests
import sys

def help():
    c = "List of Commands: \n"
    c += "`python3 build/get.py install` for Installation\n"
    c += "`python3 build/get.py vscode` for vscode"
    c += "`python3 build/get.py makefile` for generating Makefile vscode"
    c += "Made by Mr Animesh Dhakal for HomeAutomation Project"
    print(c)

def generateMakefile(board):
    file = open("Makefile", "w")
    content = "ESP_ROOT = {0}\n".format(os.path.abspath("build/"+board))
    content += "SKETCH = ./src/main.cpp\n"
    content += "MONITOR_SPEED = 115200\n"
    content += "LIBS = ./lib\n"
    content += "BOARD = {0}\n\n".format(board)
    content += "include ./build/makeEspArduino/makeEspArduino.mk"
    file.write(content)
    file.close()

def generateVSCode(board):
    if board == "8266":
        res = requests.get("https://gist.githubusercontent.com/Animeshdhakal/94f02a4dd25787e754497536a7d558ca/raw/0b8e67ad92fd1e6b11c9e5d00d756482575926b5/c_cpp_properties_esp8266.json")
    elif board == "32":
        res = requests.get("https://gist.githubusercontent.com/Animeshdhakal/94f02a4dd25787e754497536a7d558ca/raw/0b8e67ad92fd1e6b11c9e5d00d756482575926b5/c_cpp_properties_esp8266.json")
    else:
        return

    if not os.path.isdir(".vscode"):
        os.mkdir(".vscode")

    file = open(".vscode/c_cpp_properties.json", "w")
    file.write(res.text)
    file.close()
    

def install():
    board = input("Enter the Board Type (8266 for ESP8266 and 32 for ESP32): ")
    if board in ["8266", "32"]:
        cmd = "cd build \n"
        cmd += "git clone https://github.com/plerup/makeEspArduino/ \n"

        if board == "8266":
            cmd += "git clone https://github.com/esp8266/arduino esp8266 \n"

        elif board == "32":
            cmd += "git clone https://github.com/espressif/arduino-esp32 esp32 \n"

        cmd += f"cd esp{board} \n"

        version = input("Enter the Core Version (master for master version): ")

        generatevscode = input("Do you want to generate vscode settings (y/n): ")

        if version != "master":
            cmd += f"git checkout tags/{version} -f \n"
        
        cmd += "git submodule update --init -f \n"
        cmd += "cd tools \n"
        cmd += "python3 get.py \n"

        os.system(cmd)
        generateMakefile("esp"+board)

        if generatevscode == "y":
            generateVSCode(board)

    else:
        print("This board is not available or Invalid")

def main():
    if len(sys.argv) >= 2:
        if sys.argv[1] == 'install':
            install()
        elif sys.argv[1] == 'vscode':
            board = input("Enter the Board Type (8266 for ESP8266 and 32 for ESP32): ")
            if board in ["8266", "32"]:
                generateVSCode(board)
            else:
                print("Invalid Board")
        else:
            help()
    else:
        help()

if __name__ == "__main__":
    main()