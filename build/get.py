import os, sys

def help():
    m = """
    Lists of commands :
    `python3 build/get.py install` for installation,
    `python3 build/get.py reinstall` for reinstallation,
    `python3 build/get.py update` for updating to desired version

    This Script is Made only for HomeAutomation Project
    Made by: Mr. Animesh Dhakal
    """
    print(m)

def cmd_install(ver, download):
    if(download):
        ci = """
            echo "Installing Requirements for HomeAutomation" \n
            git submodule update --init \n
        """
        os.system(ci)

    if ver == "master":
        print("Installing from master")
        ci = f"""
            cd ./build/esp8266/ \n
            git submodule update --init -f \n
            cd tools \n 
            python{sys.version_info[0]} get.py \n
        """
        os.system(ci)
    else: 
        ci = f"""
            cd ./build/esp8266/ \n
            git checkout tags/{ver} -f \n
            git submodule update --init -f \n
            cd tools \n 
            python{sys.version_info[0]} get.py \n
        """
        os.system(ci)     
        

def install():
    print("hdsgdj")
    try:
        if not os.listdir("./build/esp8266"):
            ver = input("Enter the version you want: ")
            cmd_install(ver, True)
        else:
            print("Already Installed")
    except:
        ver = input("Enter the version you want: ")
        cmd_install(ver, True)

def reinstall():

    if input("Are you sure (y/n): ") == 'y':
        print("Starting Reinstallation")
        os.system("rm -rf ./build/esp8266 \n rm -rf ./build/makeEspArduino \n rm -rf ./.git/modules \n")
        ver = input("Enter the version you want: ")
        cmd_install(ver, True)

    else:
        pass

def main():
    try:
        if(sys.argv[1]):
            if sys.argv[1] == 'install': 
                install()
            elif sys.argv[1] == 'update': 
                ver = input("Enter the version you want: ")
                cmd_install(ver, False)
            elif sys.argv[1] == 'reinstall': 
                reinstall()
            else: 
                help()
        else:
            help()
    except:
        help()
    


if __name__ == '__main__':
    main()