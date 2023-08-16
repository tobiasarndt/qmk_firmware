// Copyright 2021 Joric (@joric)
// SPDX-License-Identifier: GPL-2.0-or-later
#include QMK_KEYBOARD_H
#include "spacebarracecar_tobi.h"
#ifdef OLED_ENABLE
    #include "hid_display.h"
#endif

enum layers {
    _COLEMAK = 0,
    _QWERTY = 1,
    _GAMING = 2,
    _SYM,
    _NUM,
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
#define DEL_NUM LT(_NUM, KC_DEL)
#define TAB_NUM LT(_NUM, KC_TAB)
#define ESC_UML LT(_SYM, KC_ESC)
#define BSP_UML LT(_SYM, KC_BSPC)
#define QWERTY TG(_QWERTY)
#define GAMING TG(_GAMING)

// Left-hand home row mods
#define HOME_A MT(MOD_LGUI, KC_A)
#define HOME_S MT(MOD_LALT, KC_S)
#define HOME_R MT(MOD_LCTL, KC_R)
#define HOME_T MT(MOD_RSFT, KC_T)

// Right-hand home row mods
#define HOME_N MT(MOD_RSFT, KC_N)
#define HOME_E MT(MOD_RCTL, KC_E)
#define HOME_I MT(MOD_LALT, KC_I)
#define HOME_O MT(MOD_RGUI, KC_O)

// Meta key dependent on mac
#ifdef MAC
    #define META KC_LGUI
    #define ELSE KC_LALT
#endif
#ifndef MAC
    #define META KC_LALT
    #define ELSE KC_LGUI
#endif

enum combos {
    FP_LBRC,
    RT_LPRN,
    CD_LCBR,
    LU_RBRC,
    NE_RPRN,
    HCOMM_RCBR,
    DOTMINS_PLS,
    WP_PRV,
    ST_PLY,
    XD_NXT
};

const uint16_t PROGMEM fp_combo[] = {KC_F, KC_P, COMBO_END};
const uint16_t PROGMEM rt_combo[] = {HOME_R, HOME_T, COMBO_END};
const uint16_t PROGMEM cd_combo[] = {KC_C, KC_D, COMBO_END};
const uint16_t PROGMEM lu_combo[] = {KC_L, KC_U, COMBO_END};
const uint16_t PROGMEM ne_combo[] = {HOME_N, HOME_E, COMBO_END};
const uint16_t PROGMEM hcomm_combo[] = {KC_H, DE_COMM, COMBO_END};
const uint16_t PROGMEM dotmins_combo[] = {DE_DOT, DE_MINS, COMBO_END};

// media keys
const uint16_t PROGMEM wp_combo[] = {KC_W, KC_P, COMBO_END};
const uint16_t PROGMEM st_combo[] = {HOME_A, HOME_R, COMBO_END};
const uint16_t PROGMEM xd_combo[] = {KC_X, KC_D, COMBO_END};

combo_t key_combos[] = {
    [FP_LBRC] = COMBO(fp_combo, DE_LBRC),
    [RT_LPRN] = COMBO(rt_combo, DE_LPRN),
    [CD_LCBR] = COMBO(cd_combo, DE_LCBR),
    [LU_RBRC] = COMBO(lu_combo, DE_RBRC),
    [NE_RPRN] = COMBO(ne_combo, DE_RPRN),
    [HCOMM_RCBR] = COMBO(hcomm_combo, DE_RCBR),
    [DOTMINS_PLS] = COMBO(dotmins_combo, DE_PLUS),
    [WP_PRV] = COMBO(wp_combo, KC_MPRV),
    [ST_PLY] = COMBO(st_combo, KC_MPLY),
    [XD_NXT] = COMBO(xd_combo, KC_MNXT)
};


// bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
//     /* Disable combo `SOME_COMBO` on layer `_LAYER_A` */
//     if (layer_state_is(_COLEMAK)) {
//         return true;
//     }
//     return false;
// }

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

[_COLEMAK] = LAYOUT(
  KC_LGUI, DE_CIRC, KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,         KC_J,    KC_L,    KC_U,    KC_Y,    CU_SLSH,  CU_RBR,  RGUI_T(KC_RBRC),
           KC_LCTL, HOME_A,  HOME_S,  HOME_R,  HOME_T,  KC_G,         KC_M,    HOME_N,  HOME_E,  HOME_I,  HOME_O,  CU_SLSH,
           META,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,         KC_K,    KC_H,    DE_COMM, DE_DOT,  DE_MINS, CU_EQL,
                                      TAB_NUM, BSP_UML, CU_LSFT,      DEL_NUM, CU_RSFT, ESC_UML
),

[_QWERTY] = LAYOUT(
  KC_LGUI, DE_CIRC, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    CU_SLSH, RGUI_T(KC_RBRC),
           KC_LCTL, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,         KC_H,    KC_J,    KC_K,    KC_L,    CU_LBR,  CU_RBR,
           KC_LALT, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    DE_COMM, DE_DOT,  DE_MINS, CU_EQL,
                                      TAB_NUM, BSP_UML, CU_LSFT,      DEL_NUM, CU_RSFT, ESC_UML
),

[_GAMING] = LAYOUT(
  KC_ESC,  CU_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,         KC_J,    KC_L,    KC_U,    KC_Y,    CU_LBR,  CU_RBR,  RGUI_T(KC_RBRC),
           KC_4,    KC_A,    KC_S,    KC_R,    KC_T,    KC_G,         KC_M,    KC_N,    KC_E,    KC_I,    KC_O,    CU_SLSH,
           KC_H,    KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,         KC_K,    KC_H,    DE_COMM, DE_DOT,  KC_ENT,  GAMING,
                                      KC_LALT, KC_BSPC, KC_1,         BSP_UML, CU_RSFT, ESC_UML
),

[_SYM] = LAYOUT(
  _______, KC_ESC,  DE_AT,   CU_QUOT, DE_ACUT, DE_DLR,  DE_PERC,      DE_AMPR, _______, DE_UDIA, _______, DE_BSLS, DE_RCBR,  F12_RGU,
           PLS_LCT, DE_ADIA, DE_SS,   _______, KC_LEFT, KC_UP,        KC_DOWN, KC_RGHT, _______, DE_HASH, DE_ODIA, DE_BSLS,
           EQL_LAL, CU_PIPE, DE_TILD, DE_CIRC, _______, _______,      _______, _______, DE_LABK, DE_RABK, KC_PSLS, KC_PAST,
                                      _______, _______, _______,      _______, _______, _______
),

[_NUM] = LAYOUT(
  _______, ELSE,    KC_1,    KC_2,    CU_3,    _______, _______,      DE_EQL,  DE_PLUS, DE_MINS, DE_ASTR, _______, _______, KC_CAPS,
           EQL_LCT, KC_4,    KC_5,    KC_6,    KC_HOME, KC_PGUP,      KC_PGDN, KC_END,  _______, _______, _______, APP_RCT,
           _______, KC_7,    KC_8,    KC_9,    KC_0,    DEC_SCR,      INC_SCR, _______, _______, _______, CU_HASH, _______,
                                      _______, _______, _______,      _______, _______, _______
),

[_ADJUST] = LAYOUT(
  QK_BOOT, KC_LGUI, _______, _______, _______, _______, QWERTY,       _______, _______, _______, _______, _______, RGBRST,  QK_BOOT,
           RGB_TOG, RGB_HUI, RGB_SAI, RGB_VAI, _______, GAMING,       _______, _______, RGB_VAI, RGB_SAI, RGB_HUI, RGB_TOG,
           RGB_MOD, RGB_HUD, RGB_SAD, RGB_VAD, _______, _______,      _______, _______, RGB_VAD, RGB_SAD, RGB_HUD, RGB_MOD,
                                      _______, SH_TG,   _______,      _______, SH_TG,   _______
)
};

layer_state_t layer_state_set_user(layer_state_t state) {
    state = update_tri_layer_state(state, _NUM, _SYM, _ADJUST);
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
            oled_write_P(PSTR("QUERTY\n"), false);
            break;
        case _SYM:
            oled_write_P(PSTR("Umlaute und\nNavigation"), false);
            break;
        case _NUM:
            oled_write_P(PSTR("Zahlen\n"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("Adjust\n"), false);
            break;
        case _COLEMAK:
            oled_write_P(PSTR("Colemak Mod-DH"), false);
            break;
        case _GAMING:
            oled_write_P(PSTR("Gaming"), false);
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
