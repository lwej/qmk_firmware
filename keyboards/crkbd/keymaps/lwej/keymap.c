#include QMK_KEYBOARD_H
#include "keymap_swedish.h"
#include "process_tap_dance.h"

#define KC_C_L RCTL(KC_LEFT)
#define KC_C_R RCTL(KC_RGHT)

enum layer_names {
    _BASE,
    _ONE,
    _TWO,
    _ADJUST
};

enum td_keycodes {
    _MT_L,
    T_ESC
};

typedef enum {
    TD_NONE,
    TD_UNKNOWN,
    TD_SINGLE_TAP,
    TD_SINGLE_HOLD,
    TD_DOUBLE_TAP,
    TD_DOUBLE_HOLD
} td_state_t;

typedef struct {
    bool is_press_action;
    td_state_t state;
} td_tap_t;

td_state_t cur_dance(tap_dance_state_t *state);
void mod_tap_layer_finished(tap_dance_state_t *state, void *user_data);
void mod_tap_layer_reset(tap_dance_state_t *state, void *user_data);
/* ·················· */
/* :     KEYMAP     : */
/* ·················· */

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
      KC_TAB,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                         KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,  KC_BSPC,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LCTL,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                         KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN, KC_QUOT,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
      KC_LSFT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                         KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH,  KC_ESC,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          TD(_MT_L),   MO(1),  KC_SPC,     KC_ENT,   MO(2), KC_RALT
                                      //`--------------------------'  `--------------------------'

  ),

    [_ONE] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     TD(T_ESC), SE_EXLM,  SE_DQUO,  SE_HASH,  SE_DLR,  SE_PERC,                  SE_AMPR,  SE_SLSH,  SE_LPRN,  SE_RPRN,  SE_EQL,   KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______,   SE_LPRN,  SE_RPRN,  SE_LCBR,  SE_RCBR, SE_ARNG,                   KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_GRV,   SE_ASTR,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______,   SE_LBRC,  SE_RBRC,  SE_SLSH,  SE_BSLS, SE_ASTR,                   KC_C_L,  KC_C_R,   SE_LABK,  SE_RABK,  SE_PLUS,  RSFT_T(KC_SPC),
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+ --------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT,   MO(3), KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [_TWO] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     SE_TILD,  KC_1,   KC_2,    KC_3,   KC_4,    KC_5,                          KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_DEL,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     _______,  SE_ACUT,  SE_AT,  SE_GRV,  KC_RBRC, SE_ARNG,                     KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     SE_PIPE,  KC_ESC,  KC_RGUI, KC_RALT, SE_CIRC, SE_QUOT,                     KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI,   MO(3),  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  ),

    [_ADJUST] = LAYOUT_split_3x6_3(
  //,-----------------------------------------------------.                    ,-----------------------------------------------------.
     QK_BOOT, _______, _______, _______, _______, _______,                     _______,  KC_BTN1, KC_BTN2, _______, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     RGB_TOG, KC_VOLU, KC_MNXT, RGB_VAI, _______, _______,                     KC_MS_L,  KC_MS_D,  KC_MS_U, KC_MS_R, _______, _______,
  //|--------+--------+--------+--------+--------+--------|                    |--------+--------+--------+--------+--------+--------|
     RGB_MOD, KC_VOLD, KC_MPRV, RGB_VAD, _______, _______,                     KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, _______, _______,
  //|--------+--------+--------+--------+--------+--------+--------|  |--------+--------+--------+--------+--------+--------+--------|
                                          KC_LGUI, _______,  KC_SPC,     KC_ENT, _______, KC_RALT
                                      //`--------------------------'  `--------------------------'
  )
};


/* ···················· */
/* :     TAPDANCE     : */
/* ···················· */

// Determine the tapdance state to return
td_state_t cur_dance(tap_dance_state_t *state) {
    if (state->count == 1) {
        if (state->pressed) {
            return TD_SINGLE_HOLD;
        } else {
            return TD_SINGLE_TAP;
        }
    } else if (state->count == 2) {
        if (state->interrupted || !state->pressed) {
            return TD_DOUBLE_HOLD;
        } else {
            return TD_DOUBLE_TAP;
        }
    } else {
        return 6;
    }
}

static td_tap_t mtl_tap_state = {.is_press_action = true, .state = TD_NONE};

void mod_tap_layer_finished(tap_dance_state_t *state, void *user_data) {
    mtl_tap_state.state = cur_dance(state);
    switch (mtl_tap_state.state) {
        case TD_SINGLE_TAP:
            set_oneshot_layer(_TWO, ONESHOT_START);
            clear_oneshot_layer_state(ONESHOT_PRESSED);
            break;
        case TD_SINGLE_HOLD:
            register_code(KC_LGUI);
            break;
        case TD_DOUBLE_TAP:
            set_oneshot_layer(_TWO, ONESHOT_START);
            set_oneshot_layer(_TWO, ONESHOT_PRESSED);
            break;
        case TD_DOUBLE_HOLD:
            register_code(KC_LGUI);
            layer_on(_TWO);
            break;
        default:
            break;
    }
}

void mod_tap_layer_reset(tap_dance_state_t *state, void *user_data) {
    switch (mtl_tap_state.state) {
        case TD_SINGLE_TAP:
            break;
        case TD_SINGLE_HOLD:
            unregister_code(KC_LGUI);
            break;
        case TD_DOUBLE_TAP:
            break;
        case TD_DOUBLE_HOLD:
            layer_off(_TWO);
            unregister_code(KC_LGUI);
            break;
        default:
            break;
    }
    mtl_tap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [_MT_L] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mod_tap_layer_finished, mod_tap_layer_reset),
    [T_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, SE_TILD)};


/* ···················· */
/* :     GRAPHICS     : */
/* ···················· */

#ifdef OLED_ENABLE
const char *read_layer_state(void);
static uint32_t oled_logo_timer = 0;

static const char PROGMEM my_logo[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 0xf0, 0x70, 0x78, 0x3c, 0x1c, 0x1c, 0x0e, 0x0e,
        0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x1c, 0x1c, 0x3c, 0x78, 0x70, 0xf0, 0xe0, 0xc0, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0xf8, 0x04, 0x02, 0xc2, 0xe2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2,
        0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xf2, 0xe2, 0xc2, 0x02, 0x02, 0x82, 0xc2, 0x82, 0x02, 0x02, 0x02,
        0x02, 0x02, 0x02, 0xc2, 0xc2, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0xf8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x30, 0x18, 0x0c,
        0x04, 0x06, 0x02, 0x02, 0x02, 0x02, 0x06, 0x0c, 0x18, 0x30, 0x60, 0xe0, 0xe0, 0xe0, 0xc0, 0x80,
        0x80, 0x00, 0x00, 0xe0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xf0, 0xfe, 0xff, 0x0f, 0x03, 0x01, 0x00, 0x80, 0x80, 0xc0, 0xc0, 0xc0, 0xe0,
        0xe0, 0xe0, 0x70, 0x70, 0x78, 0x38, 0x3b, 0x1f, 0x1f, 0x18, 0x00, 0x01, 0x03, 0x0f, 0xff, 0xfe,
        0xf0, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xf3, 0xff, 0xff,
        0xff, 0xdf, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xe1, 0xf3, 0xe1, 0x00, 0x00, 0x00,
        0x20, 0x70, 0xf8, 0x00, 0x00, 0x1c, 0x3e, 0x3e, 0x3e, 0x1c, 0x00, 0xff, 0x00, 0x00, 0x4e, 0xd1,
        0x51, 0x51, 0x51, 0x51, 0x51, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0x80, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x3f, 0x38, 0x38, 0x71, 0xa2,
        0x6d, 0x00, 0x00, 0x44, 0x44, 0x44, 0x54, 0x54, 0x54, 0x54, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x0f, 0x7f, 0xff, 0xf0, 0xc0, 0x80, 0x0f, 0x0f, 0x0f, 0x01, 0x01, 0x01, 0x03,
        0x03, 0x07, 0x0e, 0x0e, 0x1c, 0x3c, 0x38, 0x70, 0x70, 0x00, 0x00, 0x80, 0xc0, 0xf0, 0xff, 0x7f,
        0x0f, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xcf, 0xcf, 0xff, 0xff,
        0xff, 0xfb, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00, 0x00, 0x80,
        0x80, 0xe0, 0xe0, 0x80, 0x80, 0x00, 0x00, 0x00, 0x38, 0x38, 0x00, 0xff, 0x00, 0x00, 0xe4, 0x16,
        0x15, 0xe5, 0x14, 0x14, 0x14, 0xf4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfd, 0x02, 0x01,
        0x21, 0x31, 0x01, 0x01, 0x01, 0x02, 0xfc, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x03, 0x00, 0x00, 0x4e, 0x84, 0x04, 0x04, 0x04, 0x84, 0x44, 0x4e, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0f, 0x0e, 0x1e, 0x3c, 0x38, 0x38, 0x70, 0x70,
        0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x38, 0x38, 0x3c, 0x1e, 0x0e, 0x0f, 0x07, 0x03, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x1f, 0x20, 0x40, 0x43, 0x47, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f,
        0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x4f, 0x47, 0x43, 0x40, 0x40, 0x43, 0x47, 0x43, 0x40, 0x40, 0x41,
        0x41, 0x47, 0x47, 0x41, 0x41, 0x40, 0x40, 0x40, 0x47, 0x47, 0x20, 0x1f, 0x00, 0x00, 0x01, 0x01,
        0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
        0x02, 0x02, 0x02, 0x02, 0x06, 0x0f, 0x1f, 0x1e, 0x3c, 0x38, 0x70, 0x60, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x04, 0x04, 0x05, 0x06, 0x05, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00
    };

void init_timer(void){
    oled_logo_timer = timer_read32();
}

void render_logo(void) {
    oled_write_raw_P(my_logo, sizeof(my_logo));
}

bool oled_task_user(void) {
    if (is_keyboard_master()) {
        oled_set_cursor(0, 0); {
            oled_write_ln("corne by 42keebs", false);
        }
        oled_set_cursor(0, 3); {
            oled_write_ln(read_layer_state(), false);
        }
    } else {
        render_logo();
    }
    return false;
}
#endif
