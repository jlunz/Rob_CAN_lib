###############################################################################
# Makefile for the project rCAN_to_USB
###############################################################################

## General Flags
PROJECT = rCAN_to_USB
MCU = at90can128
TARGET = rCAN_to_USB.elf
CC = avr-gcc

CPP = avr-g++

## Options common to compile, link and assembly rules
COMMON = -mmcu=$(MCU)

## Compile options common for all C compilation units.
CFLAGS = $(COMMON)
CFLAGS += -Wall -gdwarf-2 -std=gnu99 -DF_CPU=16000000UL -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums
CFLAGS += -MD -MP -MT $(*F).o -MF dep/$(@F).d 

## Assembly specific flags
ASMFLAGS = $(COMMON)
ASMFLAGS += $(CFLAGS)
ASMFLAGS += -x assembler-with-cpp -Wa,-gdwarf2

## Linker flags
LDFLAGS = $(COMMON)
LDFLAGS +=  -Wl,-Map=rCAN_to_USB.map


## Intel Hex file production flags
HEX_FLASH_FLAGS = -R .eeprom -R .fuse -R .lock -R .signature

HEX_EEPROM_FLAGS = -j .eeprom
HEX_EEPROM_FLAGS += --set-section-flags=.eeprom="alloc,load"
HEX_EEPROM_FLAGS += --change-section-lma .eeprom=0 --no-change-warnings


## Include Directories
INCLUDES = -I"C:\Documents and Settings\jl\Desktop\rCAN_to_USB\rob_can_lib" -I"C:\Documents and Settings\jl\Desktop\rCAN_to_USB\uart_lib" 

## Objects that must be built in order to link
OBJECTS = main_rCAN_to_USB.o crc_can.o rob_can_config.o rob_can_interrupt.o rob_can_private.o rob_can_public.o uart.o 

## Objects explicitly added by the user
LINKONLYOBJECTS = 

## Build
all: $(TARGET) rCAN_to_USB.hex rCAN_to_USB.eep rCAN_to_USB.lss size

## Compile
main_rCAN_to_USB.o: ../main_rCAN_to_USB.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

crc_can.o: ../crc_can.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rob_can_config.o: ../rob_can_lib/rob_can_config.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rob_can_interrupt.o: ../rob_can_lib/rob_can_interrupt.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rob_can_private.o: ../rob_can_lib/rob_can_private.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

rob_can_public.o: ../rob_can_lib/rob_can_public.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

uart.o: ../uart_lib/uart.c
	$(CC) $(INCLUDES) $(CFLAGS) -c  $<

##Link
$(TARGET): $(OBJECTS)
	 $(CC) $(LDFLAGS) $(OBJECTS) $(LINKONLYOBJECTS) $(LIBDIRS) $(LIBS) -o $(TARGET)

%.hex: $(TARGET)
	avr-objcopy -O ihex $(HEX_FLASH_FLAGS)  $< $@

%.eep: $(TARGET)
	-avr-objcopy $(HEX_EEPROM_FLAGS) -O ihex $< $@ || exit 0

%.lss: $(TARGET)
	avr-objdump -h -S $< > $@

size: ${TARGET}
	@echo
	@avr-size -C --mcu=${MCU} ${TARGET}

## Clean target
.PHONY: clean
clean:
	-rm -rf $(OBJECTS) rCAN_to_USB.elf dep/* rCAN_to_USB.hex rCAN_to_USB.eep rCAN_to_USB.lss rCAN_to_USB.map


## Other dependencies
-include $(shell mkdir dep 2>/dev/null) $(wildcard dep/*)

