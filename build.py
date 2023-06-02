# ########################################################################
#     ____               _           __  ____        _ __    __         
#    / __ \_________    (_)__  _____/ /_/ __ )__  __(_) /___/ /__  _____
#   / /_/ / ___/ __ \  / / _ \/ ___/ __/ __  / / / / / / __  / _ \/ ___/
#  / ____/ /  / /_/ / / /  __/ /__/ /_/ /_/ / /_/ / / / /_/ /  __/ /    
# /_/   /_/   \____/_/ /\___/\___/\__/_____/\__,_/_/_/\__,_/\___/_/     
#                 /___/                                                 
# ########################################################################
#
#  Description: Simple build script for usage with platformio cli
#  Version: 1.0.0
#  Author: Volodymyr Noha
#
# ########################################################################

# import python modules
import os
import sys

# download and install necessary modules
try:
    from pyfiglet import Figlet
except ImportError:
    os.system("pip3 install pyfiglet")

try:
    from termcolor import colored
except ImportError:
    os.system("pip3 install termcolor")

# init figlet logo prinnter
f = Figlet(font='slant')

# path to platformio core
pio_tool_path = "/home/pi/.platformio/penv/bin/platformio"

print(f.renderText('Project Builder'))
print("Version: 1.0.0\n")

# get list of all available devices
print("Available devices: ")
os.system(pio_tool_path + " device list")
print("")

print("\n======Start project build======\n")

# run project build
print("\n-----Firmware build")
os.system(pio_tool_path + " run")

print("Firmware bin file: .pio/build/esp32dev/",colored("firmware.bin", "green"))
print("Firmware elf file: .pio/build/esp32dev/",colored("firmware.elf", "green"), "\n")

# upload binary to the target device (on demand)
try:
   upload_firmware = input("Do you want to upload the firmware? (y/n): ")

   if upload_firmware == 'y':
       print("\n-----Firmware Upload Started")
       os.system(pio_tool_path + " run --target upload")

   if upload_firmware == 'n':
       print("-----Firmware upload skipped")
       pass

except Exception as e:
   print ("Error occured")
   print (e)
   sys.exit(1)

print("\n==================Job Finished==================")
