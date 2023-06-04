# Profiler application

This folder contains the Node-RED flow for the profiler application. The Node-RED (https://nodered.org/docs/getting-started/local) need to be installed on the host machine in order to run this application flow.

To import project into Node-RED use this guide: https://nodered.org/docs/user-guide/editor/workspace/import-export

## Profiler application demo:

![Profiler app demo](Profiler_app_demo.png)

## Profiler description

Simple profiler for Arduino applications, based on Node-RED and ArduinoProfiler + ArduinoJson libs.
Profiler receives JSON data from target device from UART interface. The data contains:

- RAM usage in bytes;
- Free RAM in bytes;
- RAM block usage in bytes;
- Free RAM block in bytes.

# How to use profiler

To be able to use profiler you need to perform next steps:

- Enable the **ARDUINO_PROFILER** and **DEBUG_PRINTER** options in the **main.h** file;
- Compile and upload firmware to the target device;
- Install the Node-RED on your host machine, using the guide provided in first section, and import the profiler.json to the Node-RED environmetn (use the guide provided in first section);
- Check the COM port to which target device is connected and change it (if needed) in profiler.json UART configuration;
- Connect the target device to PC and  open the web application with http://localhost:1880/ui.