# MCU name
MCU = atmega32u4

# Bootloader selection
BOOTLOADER = caterina

# Build Options
#   change yes to no to disable
#
BOOTMAGIC_ENABLE = no      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = yes      # Mouse keys
EXTRAKEY_ENABLE = yes      # Audio control and System control
CONSOLE_ENABLE = no        # Console for debug
COMMAND_ENABLE = no        # Commands for debug and configuration
NKRO_ENABLE = no           # Enable N-Key Rollover
BACKLIGHT_ENABLE = no      # Enable keyboard backlight functionality
AUDIO_ENABLE = no          # Audio output
SPLIT_KEYBOARD = yes       # Split common
LTO_ENABLE     = yes
OLED_ENABLE = yes
OLED_DRIVER = SSD1306
RAW_ENABLE          = yes   # Enable RAW HID communication to PC
LINK_TIME_OPTIMIZATION_ENABLE = yes


DEFAULT_FOLDER = jorne/rev1

GERMAN_ENABLE = yes

OPT_DEFS += -DGERMAN_ENABLE             \



SRC +=  spacebarracecar.c		\
        hid_display.c 	\
#        ./lib/oled_driver_custom.c  \