# SWE-450 Milestone 2 (Doodle Jump Game)
# Dima Bondar
# 02/09/2025

# Target executable name
TARGET = game

# Device and toolchain configuration
ALT_DEVICE_FAMILY ?= soc_cv_av
SOCEDS_ROOT ?= $(SOCEDS_DEST_ROOT)
HWLIBS_ROOT = $(SOCEDS_ROOT)/ip/altera/hps/altera_hps/hwlib
CROSS_COMPILE = arm-linux-gnueabihf-
CC = $(CROSS_COMPILE)gcc
ARCH = arm

# Directories
LCD_DIR = LCD
INCLUDES = -I$(HWLIBS_ROOT)/include/$(ALT_DEVICE_FAMILY) \
          -I$(HWLIBS_ROOT)/include/ \
          -I$(LCD_DIR)

# Compilation flags
CFLAGS = -static -g -Wall -std=c99 \
         -D$(ALT_DEVICE_FAMILY) \
         -D_DEFAULT_SOURCE \
         $(INCLUDES)

LDFLAGS = -g -Wall

# Object files
LCD_OBJS = $(LCD_DIR)/terasic_lib.o \
		   $(LCD_DIR)/LCD_Lib.o \
		   $(LCD_DIR)/LCD_Driver.o \
           $(LCD_DIR)/LCD_Hw.o \
           $(LCD_DIR)/lcd_graphic.o \
           $(LCD_DIR)/font.o \
           

GAME_OBJS = app.o \
            game.o \
            hardware.o \
            seven_seg.o \
            push_buttons.o \
            lcd.o \
            switches.o

OBJS = $(LCD_OBJS) $(GAME_OBJS)

# Default build rule
build: $(TARGET)

# Link everything into final executable
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ -lrt -lm

# Compile source files to object files
$(LCD_DIR)/%.o: $(LCD_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean rule
.PHONY: clean
clean:
	rm -f $(TARGET) *.o $(LCD_DIR)/*.o *~ *.bmp

# Build all rule
.PHONY: all
all: $(TARGET)