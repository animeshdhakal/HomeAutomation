import os, sys


def help():
    c = "Lists of commands : \n"
    c += "`python3 build/get.py install` for installation, \n"
    c += "`python3 build/get.py reinstall` for reinstallation, \n"
    c += "`python3 build/get.py update` for updating to desired version \n\n"
    c += "This Script is Made only for HomeAutomation Project \n"
    c += "Made by: Mr. Animesh Dhakal"
    print(c)


def cmd_install(ver, download):
    if (download):
        commands = """
            echo "Installing Requirements for HomeAutomation" \n
            git submodule update --init \n
        """
        os.system(commands)

    if ver == "master":
        print("Installing from master")
        commands = f"""
            cd ./build/esp8266/ \n
            git submodule update --init -f \n
            cd tools \n 
            python3 get.py \n
        """
        os.system(commands)
    else:
        commands = f"""
            cd ./build/esp8266/ \n
            git checkout tags/{ver} -f \n
            git submodule update --init -f \n
            cd tools \n 
            python3 get.py \n
        """
        os.system(commands)


def install():
    if not os.path.exists("./build/esp8266/cores"):
        ver = input("Enter the version you want: ")
        cmd_install(ver, True)
    else:
        print("Already Installed")
    


def reinstall():

    if input("Are you sure (y/n): ") == 'y':
        print("Starting Reinstallation")
        os.system(
            "rm -rf ./build/esp8266 \n rm -rf ./build/makeEspArduino \n rm -rf ./.git/modules \n"
        )
        ver = input("Enter the version you want: ")
        cmd_install(ver, True)

    else:
        pass


def main():
    if len(sys.argv) >= 2:
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


if __name__ == '__main__':
    main()