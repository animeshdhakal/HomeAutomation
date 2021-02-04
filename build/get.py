import os, sys

if os.listdir("./build/esp8266"):
    c = f"""
        echo "Installing Requirements for HomeAutomation"
        git submodule update --init \n
        cd ./build/esp8266/ \n
        git checkout tags/2.7.4 \n
        echo "Installing Requirements for Esp8266"
        git submodule update --init \n
        cd tools \n
        python{sys.version_info[0]} get.py \n
    """
    os.system(c)

else:
    print("Already Installed")
