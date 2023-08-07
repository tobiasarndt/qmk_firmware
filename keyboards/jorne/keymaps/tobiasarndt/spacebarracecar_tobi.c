#include "spacebarracecar_tobi.h"
#include "hid_display.h"

#ifdef GERMAN_ENABLE
// These indicate if left and right shift are physically pressed
bool lshift = false;
bool rshift = false;

// Interrupt and times for space cadet shift
bool lshiftp = false;
bool rshiftp = false;
uint16_t lshift_timer = 0;
uint16_t rshift_timer = 0;

// Number of items that are saved in prev_kcs
uint8_t prev_indx = 0;
// Used to save the last 6 actual keycodes activated by frankenkeycodes
uint16_t prev_kcs[6] = {0, 0, 0, 0, 0, 0};

// If true the deadkey characters grave and circonflexe are not automatically escaped
bool esct = false;

/*
Used to add a keycode to a prev_kcs to remember it.
When full the last code gets discarded and replaced by
the new one.
*/
void add_to_prev(uint16_t kc){
  for (int i=0; i<prev_indx; i++){
    if (kc == prev_kcs[i])
      return;
  }
  if (prev_indx == 6){
    for (int i=5; i>0; i--){
      prev_kcs[i] = prev_kcs[i-1];
    }
    prev_kcs[0] = kc;
  } else {
    prev_kcs[prev_indx] = kc;
    prev_indx++;
  }
}

/*
Unregisters all codes saved in prev_kcs and resets prev_indx.
gets called on multiple occasions mainly when shift is released
and when frankenkeycodes are pressed. Prevents output of
wrong characters when really specific key combinations
that would never occur during normal usage are pressed.
*/
void unreg_prev(void){
  if (prev_indx == 0)
    return;
  for (int i=0; i<prev_indx; i++){
    unregister_code(prev_kcs[i]);
  }
  prev_indx = 0;
}
#endif

// Interrupt and times for Nav/Esc
bool navesc = false;
uint16_t navesc_timer = 0;

// If true Gui keys and Space Cadet Shift get disabled
bool game = false;

// Interrupts all timers
void timer_timeout(void){
  #ifdef GERMAN_ENABLE
  lshiftp = false;
  rshiftp = false;
  #endif
  navesc = false;
  timer_timeout_keymap();
}

__attribute__((weak))
void timer_timeout_keymap(void){
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
  case CU_GAME:
    if(record->event.pressed) {
      timer_timeout();
      game = !game;
    }
    // allows keymap to execute further commands when CU_GAME is pressed, for example enabling a macro layer
    return process_record_keymap(keycode, record) && false;
  case KC_LGUI:
  case KC_RGUI:
    if (record->event.pressed)
      timer_timeout();
    if (game)
      return false;
    else
      return true;
  case CU_NAV:
    if(record->event.pressed) {
      navesc = true;
      navesc_timer = timer_read();
      layer_on(_NAV);
    } else {
      if (timer_elapsed(navesc_timer) < TAPPING_TERM && navesc) {
        register_code(KC_ESC);
        unregister_code(KC_ESC);
      }
      layer_off(_NAV);
    }
    return false;
  case KC_P00:
    if(record->event.pressed) {
      timer_timeout();
      register_code(KC_P0);
      unregister_code(KC_P0);
      register_code(KC_P0);
      unregister_code(KC_P0);
    }
    return false;

  #ifdef RGBLIGHT_ENABLE
  case CU_RGBV:
    if(record->event.pressed) {
      timer_timeout();
      if (rgblight_get_val()+32>255)
        rgblight_sethsv(rgblight_get_hue(), rgblight_get_sat(), 31);
      else
        rgblight_sethsv(rgblight_get_hue(), rgblight_get_sat(), rgblight_get_val()+32);
    }
    return false;
  #endif

  #ifdef GERMAN_ENABLE
  case CU_LSFT:
    if(record->event.pressed) {
      lshiftp = true;
      lshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      lshift = true;
    } else {
      if (timer_elapsed(lshift_timer) < TAPPING_TERM && lshiftp && !game) {
        unregister_code(KC_LSFT);
        register_code(KC_ENT);
        unregister_code(KC_ENT);
      }
      unreg_prev();
      if (!rshift)
        unregister_code(KC_LSFT);
      lshift = false;
    }
    return false;
  case CU_RSFT:
    if(record->event.pressed) {
      rshiftp = true;
      rshift_timer = timer_read();
      unregister_code(KC_LSFT);
      register_code(KC_LSFT);
      rshift = true;
    } else {
      if (timer_elapsed(rshift_timer) < TAPPING_TERM && rshiftp && !game) {
        unregister_code(KC_LSFT);
        register_code(KC_SPC);
        unregister_code(KC_SPC);
      }
      unreg_prev();
      if (!lshift)
        unregister_code(KC_LSFT);
      rshift = false;
    }
    return false;
  case CU_6:
    if(record->event.pressed){
      timer_timeout();
      unregister_code(KC_LSFT);
      if (lshift || rshift){
        unregister_code(DE_CIRC);
        register_code(DE_CIRC);
        unregister_code(DE_CIRC);
        if (!esct) {
          register_code(KC_SPC);
          unregister_code(KC_SPC);
        }
        register_code(KC_LSFT);
      } else {
        register_code(DE_6);
      }
    } else {
      unregister_code(DE_6);
    }
    return false;
  // case CU_COMM:
  //   SHIFT_NO(DE_COMM, DE_LABK)
  case CU_SLSH:
    SHIFT_NO(DE_SLSH, DE_QUES)
  case CU_3:
    SHIFT_NO(DE_3, DE_HASH)
  case CU_7:
    SHIFT_NO(DE_7, DE_AMPR)
  case CU_8:
    SHIFT_NO(DE_8, DE_ASTR)
  case CU_9:
    SHIFT_NO(DE_9, DE_LPRN)
  case CU_0:
    SHIFT_NO(DE_0, DE_RPRN)
  case CU_EQL:
    SHIFT_NO(DE_EQL, DE_PLUS)
  case CU_LBR:
    SHIFT_NO(DE_LPRN, DE_LBRC)
  case CU_RBR:
    SHIFT_NO(DE_RPRN, DE_RBRC)
  case CU_QUOT:
    SHIFT_NO(DE_QUOT, DE_DQUO)
  case CU_HASH:
    SHIFT_NO(DE_HASH, DE_TILD)
  case CU_TAB:
    SHIFT_NO(KC_TAB, KC_ESC)
  case CU_PIPE:
    SHIFT_NO(DE_PIPE, DE_TILD)
  case INC_SCR: 
    if(record->event.pressed) {
      uprintf("INC_SCR: ");
      increase_screen_num();
    }
    return false;
  case DEC_SCR: 
    if(record->event.pressed) {
      uprintf("DEC_SCR");
      decrease_screen_num();
    }
    return false;
  case KC_LCTL:
  case KC_RCTL:
    if(!record->event.pressed) {
      timer_timeout();
      unregister_code(KC_Z);
      unregister_code(KC_Y);
    }
    return true;
  #endif

  default:
    if(record->event.pressed) {
      timer_timeout();

      #ifdef GERMAN_ENABLE
      if (lshift || rshift)
        register_code(KC_LSFT);
      else
        unregister_code(KC_LSFT);
      #endif

    }
    return process_record_keymap(keycode, record);
  }
}

__attribute__((weak))
bool process_record_keymap(uint16_t keycode, keyrecord_t *record) {
  return true;
}
