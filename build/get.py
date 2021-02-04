import os, sys

def help():
    print("""
        Lists of commands :
        
        `python3 build/get.py install` for installation,

        `python3 build/get.py reinstallation` for reinstallation,

        `python3 build/get.py update` for updating to desired version

        This Script is Made only for HomeAutomation Project
        Made by: Mr. Animesh Dhakal
    """)

def cmd_install():
    ci = f"""
        echo "Installing Requirements for HomeAutomation" \n
        git submodule update --init \n
        cd ./build/esp8266/ \n
        git checkout tags/2.7.4 \n
        echo "Installing Requirements for Esp8266"
        git submodule update --init \n
        cd tools \n
        python{sys.version_info[0]} get.py \n
    """
    os.system(ci)

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
        cmd_install();

    else:
        print("Already Installed")

def reinstall():
    if input("Are you sure you (y/n): ") == 'y':
        print("Starting Reinstallation")
        os.system("rm -rf ./build/esp8266 \n rm -rf ./build/makeEspArduino \n rm -rf ./.git/modules \n")
        cmd_install()

def main():
    if(sys.argv[1]):
        if sys.argv[1] == 'install': install()
        elif sys.argv[1] == 'update': update()
        elif sys.argv[1] == 'reinstall': reinstall()
        else: help()
    else:
        help()


if __name__ == '__main__':
    main()