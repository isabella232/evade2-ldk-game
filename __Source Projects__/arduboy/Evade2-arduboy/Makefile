# Arduino Make file. Refer to https://github.com/sudar/Arduino-Makefile

# Setup ARDUINO_DIR and AVR_TOOLS_DIR variables using a private config.mk if found
# -include ./config.mk

# Set ARDUINO_DIR to your IDE install location
ARDUINO_DIR = $(arduino_dir)
ARDMK_DIR = ../tools/Arduino-Makefile
#AVR_TOOLS_DIR ?= EDIT_OR_CREATE_config.mk
USER_LIB_PATH = $(realpath ./src)
BOARD_TAG = leonardo
# List EEPROM even if it will be optimized away because Arduboy2 depends on
# including EEPROM.h
ARDUINO_LIBS += EEPROM Arduboy2 ATMLib2
CFLAGS += -g

## Uncomment the following block to compile without standard Arduino core files
NO_CORE = Yes
MCU = atmega32u4
F_CPU = 16000000L
VARIANT = leonardo
ARDUINO_LIBS += ArduinoCore
##

include ../tools/Arduino-Makefile/Arduino.mk
