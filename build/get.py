import os, sys
import requests, json

def help():
    p = f'Use "python{sys.version_info[0]} ./build/get.py install" for installing all requirements'
    print(p)


def update():
    ver = input("Enter the version you want:  ")
    if ver == "master":
        ci = f"""
            cd ./build/esp8266/ \n
            git checkout master -f \n
            git submodule update --init -f \n
            cd tools \n 
            python{sys.version_info[0]} get.py \n
        """
        os.system(ci)
    else: 
        ci = f"""
            cd ./build/esp8266/ \n
            git checkout tags/{ver} -f \n
            cd tools \n 
            python{sys.version_info[0]} get.py \n
        """
        os.system(ci)     
        

def install():
    if not os.listdir("./build/esp8266"):
        c = f"""
            echo "Installing Requirements for HomeAutomation"
            git submodule update --init \n
            cd ./build/esp8266/ \n{sys.version_info[0]}
            git checkout tags/2.7.4 \n
            echo "Installing Requirements for Esp8266"
            git submodule update --init \n
            cd tools \n
            python{sys.version_info[0]} get.py \n
        """
        os.system(c)

    else:
        print("Already Installed")

def main():
    if(sys.argv[1]):
        if sys.argv[1] == 'install': install()
        elif sys.argv[1] == 'update': update()
        else: help()
    else:
        help()


if __name__ == '__main__':
    main()