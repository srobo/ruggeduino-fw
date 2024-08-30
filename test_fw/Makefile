FW_VER=1
DEV=/dev/ttyACM0

CC=avr-gcc
CXX=avr-g++
AR=avr-ar
OBJCOPY=avr-objcopy
AVRDUDE=avrdude
ARDUINO=/usr/share/arduino

SRCDIR := src

CFLAGS = -Os -Wl,--gc-sections -mmcu=atmega328p -DF_CPU=16000000L -MMD -DUSB_VID=null -DUSB_PID=null -DARDUINO=101 \
	 -DFW_VER=$(FW_VER) \
         -I$(ARDUINO)/hardware/arduino/cores/arduino \
         -I$(ARDUINO)/hardware/arduino/variants/standard \
         -I$(SRCDIR)
CXXFLAGS = $(CFLAGS)

BUILDDIR := build

CORE_SRCDIR := $(ARDUINO)/hardware/arduino/cores/arduino
CORE_SRC := $(wildcard $(CORE_SRCDIR)/*.cpp) \
            $(wildcard $(CORE_SRCDIR)/*.c)
CORE_OBJ := $(patsubst $(CORE_SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter %.cpp,$(CORE_SRC))) \
            $(patsubst $(CORE_SRCDIR)/%.c,$(BUILDDIR)/%.o,$(filter %.c,$(CORE_SRC)))

SRC := $(wildcard $(SRCDIR)/*.cpp)
#OBJ := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter %.cpp,$(SRC)))
OBJ := $(SRC:.cpp=.o)

ruggeduino_test.hex: ruggeduino_test.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

ruggeduino_test.elf: $(OBJ) $(BUILDDIR)/core.a
	$(CC) $(CFLAGS) -o $@ $^ -L$(BUILDDIR)

$(BUILDDIR)/%o: $(SRC)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/core.a: $(CORE_OBJ)
	$(AR) rcs $@ $^

$(BUILDDIR)/%.o: $(CORE_SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILDDIR)/%.o: $(CORE_SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean flash

flash: ruggeduino_test.hex
	$(AVRDUDE) -v -p atmega328p -c arduino -P $(DEV) -D -U flash:w:$<:i

clean:
	rm -rf $(BUILDDIR)/*
