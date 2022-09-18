#include <stdio.h>
#include <string.h>
#include "hid_display.h"
#include "jorne.h"

// Add headers for raw hid communication
// #include <split_scomm.h>
#include "raw_hid.h"

// HID input
bool hid_connected = false; // Flag indicating if we have a PC connection yet
uint8_t screen_max_count = 0;  // Number of info screens we can scroll through (set by connecting node script)
uint8_t screen_show_index = 0; // Current index of the info screen we are displaying
bool font_16 = true; // Flag indicating if 16 bit font is used

uint16_t screen_data_buffer[84] =  {0}; // Buffer used to store the screen data sent by connected node script
uint16_t screen_data[85] =  {0}; // Buffer used to store the screen data sent by connected node script
/*} else {
  uint8_t screen_data_buffer[84] =  {0}; // Buffer used to store the screen data sent by connected node script
  uint8_t screen_data[85] =  {0}; // Buffer used to store the screen data sent by connected node script
})*/
int screen_data_index = 0; // Current index into the screen_data_buffer that we should write to
char hid_status_str[21] = {};


static void render_logo(void) {
    static const char PROGMEM raw_logo[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 128, 128, 0, 0, 0, 192, 224, 240, 240, 240, 240, 240, 240, 240, 224,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 112, 112, 112, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 192, 224, 240, 0, 0, 240, 252, 254, 255, 255, 255, 1, 240, 252, 255,
        255, 255, 255, 255, 255, 255, 255, 127, 31, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 254, 254, 254, 0, 0, 240,
        248, 252, 30, 14, 14, 14, 14, 14, 14, 30, 252, 248, 240, 0, 0, 254, 254, 252, 28, 14, 14, 14, 14, 0, 0, 254, 254, 252, 28, 14, 14,
        14, 14, 30, 252, 248, 240, 0, 0, 240, 248, 252, 222, 206, 206, 206, 206, 206, 206, 222, 252, 248, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 63, 255, 255, 255, 254, 240, 255, 255, 255, 255, 255, 255, 248, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 193, 128, 128, 128, 192, 240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 128, 255, 255, 255, 0, 0,
        15, 31, 63, 120, 112, 112, 112, 112, 112, 112, 120, 63, 31, 15, 0, 0, 127, 127, 127, 0, 0, 0, 0, 0, 0, 0, 127, 127, 127, 0, 0, 0,
        0, 0, 0, 127, 127, 127, 0, 0, 15, 31, 63, 121, 113, 113, 113, 113, 113, 113, 121, 61, 29, 13, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 3, 1, 1, 3, 3, 7, 7, 7, 3, 3, 1, 3, 7, 7, 7, 7, 7, 7, 7, 3, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 7, 3, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0
    };
    oled_write_raw_P(raw_logo, sizeof(raw_logo));
}

bool is_hid_connected(void) {
  return hid_connected;
}

const char *read_hid_status(void) {
  if (hid_connected) {
    snprintf(hid_status_str, sizeof(hid_status_str), "HID Screen: %d", screen_show_index + 1);
  } else {
    snprintf(hid_status_str, sizeof(hid_status_str), "HID Disconnected");
  }
  return hid_status_str;
}

void raw_hid_send_screen_index(void) {
  if (hid_connected) {
    // Send the current info screen index to the connected node script so that it can pass back the new data
    uint8_t send_data[32] = {0};
    send_data[0] = screen_show_index + 1; // Add one so that we can distinguish it from a null byte
    raw_hid_send(send_data, sizeof(send_data));
  }
}

void decrease_screen_num (void) {
  screen_show_index = (screen_show_index - 1) % screen_max_count;
  raw_hid_send_screen_index();
}

void increase_screen_num (void) {
  screen_show_index = (screen_show_index + 1) % screen_max_count;
  raw_hid_send_screen_index();
}

// Slave screen writer
void write_info_screen(void) {
  if (font_16) {
    //    uprintf("write_info_screen byte 0: %d \n", screen_data[0]);
    if (screen_data[0] > 0) {
      // If the first byte of the buffer is non-zero we should have a full set of data to show,
      // So we copy it into the display
      oled_index(screen_data + 1, 84, 0);
    } else {
      // Otherwise we just draw the logo
      render_logo();
    }
  } else {
    if (screen_data[0] > 0) {
      // If the first byte of the buffer is non-zero we should have a full set of data to show,
      // So we copy it into the display
      oled_write((char*)screen_data + 1, 0);
    } else {
      // Otherwise we just draw the logo
      render_logo();
    }
  }
}

void raw_hid_receive8(uint8_t *data, uint8_t length) {
  // PC connected, so set the flag to show a message on the master display
  hid_connected = true;

  // Initial connections use '1' in the first byte to indicate this
  if (length > 1 && data[0] == 1) {
    // New connection so restart screen_data_buffer
    screen_data_index = 0;

    // The second byte is the number of info screens the connected node script allows us to scroll through
    screen_max_count = data[1];
    if (screen_show_index >= screen_max_count) {
      screen_show_index = 0;
    }

    // Tell the connection which info screen we want to look at initially
    raw_hid_send_screen_index();
    return;
  }

  // Otherwise the data we receive is one line of the screen to show on the display
  if (length >= 21) {
    // Copy the data into our buffer and increment the number of lines we have got so far
    memcpy((char*)&screen_data_buffer[screen_data_index * 21], data, 21);
    screen_data_index++;

    // Once we reach 4 lines, we have a full screen
    if (screen_data_index == 4) {
      // Reset the buffer back to receive the next full screen data
      screen_data_index = 0;

      // Now get ready to transfer the whole 4 lines to the slave side of the keyboard.
      // First clear the transfer buffer with spaces just in case.
      memset((char*)&screen_data[0], ' ', sizeof(screen_data));

      // Copy in the 4 lines of screen data, but start at index 1, we use index 0 to indicate a connection in the slave code
      memcpy((char*)&screen_data[1], screen_data_buffer, sizeof(screen_data_buffer));

      // Set index 0 to indicate a connection has been established
      screen_data[0] = 1;
    }
  }
}

void raw_hid_receive(uint8_t *data, uint8_t length) {
  // PC connected, so set the flag to show a message on the master display
  hid_connected = true;
  uprintf("\nScreen Index Start, length, bit 0 : %d, %d, %d\n", screen_data_index, length, data[0]);


  // Initial connections use '1' in the first byte to indicate this
  if (length > 1 && data[0] == 1) {
    // New connection so restart screen_data_buffer
    // screen_data_index = 0;

    // The second byte is the number of info screens the connected node script allows us to scroll through
    screen_max_count = data[1];
    if (screen_show_index >= screen_max_count) {
      screen_show_index = 0;
    }

    // Tell the connection which info screen we want to look at initially
    raw_hid_send_screen_index();
    return;
  }

  // Otherwise the data we receive is one line of the screen to show on the display
  if (length >= 28) {
    // Copy the data into our buffer and increment the number of lines we have got so far
    for (int i = 0; i < 14; i++) {
      screen_data_buffer[screen_data_index * 14 + i] = 
      ((uint16_t)((data[2*i] -2 ) << 8)) + ((uint16_t)(data[2*i+1]));
      uprintf("%d, %d\t,", i, ((uint16_t)((data[2*i]) - 2) << 8) + ((uint16_t)(data[2*i+1])));
    }
    screen_data_index++;
    uprintf("\nScreen Index, size: %d, %d\n", screen_data_index, sizeof(screen_data_buffer));

    // Once we reach 4 lines, we have a full screen
    if (screen_data_index == 6) {
      // Reset the buffer back to receive the next full screen data
      screen_data_index = 0;

      // Now get ready to transfer the whole 4 lines to the slave side of the keyboard.
      // First clear the transfer buffer with spaces just in case.
      memset((uint16_t*)&screen_data[0], ' ', sizeof(screen_data));

      // Copy in the 4 lines of screen data, but start at index 1, we use index 0 to indicate a connection in the slave code
      memcpy((uint16_t*)&screen_data[1], screen_data_buffer, sizeof(screen_data_buffer));

      // Set index 0 to indicate a connection has been established
      screen_data[0] = 1;
    }
  }
}
