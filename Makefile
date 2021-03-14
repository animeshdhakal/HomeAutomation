ESP_ROOT = ./build/esp8266/
SKETCH = ./src/main.cpp
LIBS = ./lib
BOARD = nodemcu
MONITOR_SPEED = 115200
OTA_ADDR = 192.168.100.200


include ./build/makeEspArduino/makeEspArduino.mk