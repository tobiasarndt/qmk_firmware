// Copyright 2021 Joric (@joric)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "spacebarracecar_tobi.h"
#include "jorne.h"
#include "hid_display.h"

enum layers {
    _QWERTY = 0,
    _COLEMAK = 1,
    _BLAU,
    _ROT,
    _ADJUST
};


enum custom_keycodes {
    RGBRST = SAFE_RANGE
};

#define F12_RGU MT(MOD_RGUI, KC_F12)
#define PLS_LCT MT(MOD_LCTL, KC_PPLS)
#define EQL_LCT MT(MOD_LCTL, KC_PEQL)
//#define APP_LCT MT(MOD_LCTL, KC_APP)
//#define EQL_RCT MT(MOD_RCTL, KC_PEQL)
//#define QUO_RCT MT(MOD_RCTL, KC_QUOT)
#define APP_RCT MT(MOD_RCTL, KC_APP)
//#define MIN_RCT MT(MOD_RCTL, KC_MINS)
#define EQL_LAL MT(MOD_LALT, KC_EQL)
//#define BSL_RAL MT(MOD_RALT, KC_BSLS)
#define BSP_LSH MT(MOD_LSFT, KC_BSPC)
#define SPC_RSH MT(MOD_RSFT, KC_SPC)
#define DEL_NUM LT(_ROT, KC_DEL)
#define TAB_NUM LT(_ROT, KC_TAB)
#define ENT_LWR LT(_LOWER, KC_ENT)
#define ESC_UML LT(_BLAU, KC_ESC)

#define BSP_UML LT(_BLAU, KC_BSPC)
#define COLEMAK TG(_COLEMAK)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_QWERTY] = LAYOUT(
  KC_LGUI, DE_CIRC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    CU_SLSH, RGUI_T(KC_RBRC),
           KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    CU_LBR,  CU_RBR,
           KC_LALT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    DE_COMM, DE_DOT,  DE_MINS, CU_EQL,
                                      TAB_NUM, BSP_UML, CU_LSFT,      DEL_NUM, CU_RSFT, ESC_UML
),

[_COLEMAK] = LAYOUT(
  KC_LGUI, DE_CIRC, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,         KC_J,    KC_L,    KC_U,    KC_Y,    CU_LBR,  CU_RBR,  RGUI_T(KC_RBRC),
           KC_LCTL, KC_A,    KC_S,    KC_R,    KC_T,    KC_G,         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,  CU_SLSH,
           KC_LALT, KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,         KC_K,    KC_H,    DE_COMM, DE_DOT,  DE_MINS, CU_EQL,
                                      TAB_NUM, BSP_UML, CU_LSFT,      DEL_NUM, CU_RSFT, ESC_UML
),

[_BLAU] = LAYOUT(
  _______, KC_ESC,  DE_AT,   CU_QUOT, DE_ACUT, _______, _______,      _______, DE_UDIA, _______, DE_ODIA, _______, DE_BSLS,  F12_RGU,
           PLS_LCT, DE_ADIA, DE_SS,   _______, KC_LEFT, KC_UP,        KC_DOWN, KC_RGHT, _______, _______, DE_LCBR, DE_RCBR,
           EQL_LAL, _______, _______, KC_MPLY, KC_MPRV, KC_VOLD,      KC_VOLU, KC_MNXT, DE_LABK, DE_RABK, KC_PSLS, KC_PAST,
                                      _______, _______, _______,      _______, _______, _______
),

[_ROT] = LAYOUT(
  _______, KC_NLCK, KC_1,    KC_2,    CU_3,    KC_4,    KC_5,         KC_6,    CU_7,    CU_8,    CU_9,    CU_0,    _______, KC_CAPS, 
           EQL_LCT, _______, _______, _______, KC_HOME, KC_PGUP,      KC_PGDN, KC_END,  _______, _______, KC_INS,  APP_RCT,
           _______, _______, _______, KC_COPY, KC_INS,  DEC_SCR,      INC_SCR, _______, _______, _______, _______, CU_HASH, 
                                      _______, _______, _______,      _______, _______, _______
),

[_ADJUST] = LAYOUT(
  QK_BOOT, RGBRST,  KC_ASUP, KC_ASTG, KC_ASDN, _______, COLEMAK,      _______, _______, KC_ASDN, KC_ASTG, KC_ASUP, RGBRST,  QK_BOOT,
           RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, _______, _______,      _______, _______, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG,
           RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, _______, _______,      _______, _______, RGB_VAD, RGB_SAD, RGB_HUD, RGB_MOD,
                                      _______, SH_TG,   _______,      _______, SH_TG,   _______
)
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _BLAU, _ROT, _ADJUST);
    return state;
}

 #ifdef OLED_ENABLE
oled_rotation_t oled_init_user(oled_rotation_t rotation) {
    if (is_keyboard_left()) {
        return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
    }
    return rotation;
}
static void render_status(void) {
    // Host Keyboard Layer StatusF
    oled_write_P(PSTR("Layer: "), false);
    switch (get_highest_layer(layer_state)) {
        case _QWERTY:
            oled_write_P(PSTR("Default\n"), false);
            break;
        case _BLAU:
            oled_write_P(PSTR("Umlaute und\nNavigation"), false);
            break;
        case _ROT:
            oled_write_P(PSTR("Zahlen\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        case _COLEMAK:
            oled_write_P(PSTR("Colemak Mod-DH"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined\n"), false);
    }

    // Host Keyboard LED Status
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.num_lock ? PSTR("NUMLCK ") : PSTR("       "), false);
    oled_write_P(led_state.caps_lock ? PSTR("CAPLCK ") : PSTR("       "), false);
    oled_write_P(led_state.scroll_lock ? PSTR("SCRLCK ") : PSTR("       "), false);
}

bool oled_task_user(void) {
  if (is_keyboard_left()) {
      write_info_screen();
  } else {
      render_status(); // Renders the current keyboard state (layer, lock, caps, scroll, etc)
      //render_logo();
  }
  return false;
}
#endif
