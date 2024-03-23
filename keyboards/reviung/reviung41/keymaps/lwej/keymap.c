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


const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,      KC_T,               KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
    KC_LCTL,  KC_A,     KC_S,     KC_D,     KC_F,      KC_G,               KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,
    KC_LSFT,  KC_Z,     KC_X,     KC_C,     KC_V,      KC_B,               KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,  KC_ENT,
                                            TD(_MT_L), MO(_ONE), KC_SPC,   MO(_TWO), KC_LALT
  ),

  [_ONE] = LAYOUT(
    TD(T_ESC), SE_EXLM,  SE_DQUO,  SE_HASH,  SE_DLR,    SE_PERC,            SE_AMPR,  SE_SLSH,  SE_LPRN,  SE_RPRN,  SE_EQL,   KC_DEL,
    _______,   SE_LPRN,  SE_RPRN,  SE_LCBR,  SE_RCBR,   SE_ARNG,            KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_GRV,   SE_ASTR,
    _______,   SE_LBRC,  SE_RBRC,  SE_SLSH,  SE_BSLS,   SE_GRV,             KC_C_L,   KC_C_R,   SE_LABK,  SE_RABK,  SE_PLUS,  RSFT_T(KC_SPC),
                                            _______,   _______,  KC_ENT,    MO(_ADJUST),  KC_RALT
  ),

  [_TWO] = LAYOUT(
    SE_TILD,  KC_1,     KC_2,     KC_3,     KC_4,      KC_5,               KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_DEL,
    _______,  SE_ACUT,  SE_AT,    SE_GRV,   KC_RBRC,   KC_BSLS,            KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,
    SE_PIPE,  KC_ESC,   KC_RGUI,  KC_RALT,  KC_CAPS,   KC_QUOT,            KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,
                                            _______,   MO(_ADJUST),   KC_BSPC,  _______,  _______
  ),

  [_ADJUST] = LAYOUT(
    RGB_VAI,   RGB_SAI, RGB_HUI,  RGB_MOD,  XXXXXXX,   RGB_TOG,            XXXXXXX,  KC_BTN1,  KC_BTN2,  XXXXXXX,  XXXXXXX,  XXXXXXX,
    RGB_VAD,   RGB_SAD, RGB_HUD,  RGB_RMOD, XXXXXXX,   XXXXXXX,            KC_MS_L,  KC_MS_D,  KC_MS_U,  KC_MS_R,  XXXXXXX,  XXXXXXX,
    XXXXXXX,   XXXXXXX, XXXXXXX,  XXXXXXX,  XXXXXXX,   XXXXXXX,            QK_BOOT,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,  XXXXXXX,
                                            _______,   _______,  XXXXXXX,  _______,  _______
  ),
};


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

#ifdef RGBLIGHT_ENABLE

// Use top LED to visualize layer
const rgblight_segment_t PROGMEM base_layer[] = RGBLIGHT_LAYER_SEGMENTS({11, 11, HSV_GREEN});

const rgblight_segment_t PROGMEM one_layer[] = RGBLIGHT_LAYER_SEGMENTS({11, 11, HSV_MAGENTA});

const rgblight_segment_t PROGMEM two_layer[] = RGBLIGHT_LAYER_SEGMENTS({11, 11, HSV_CYAN});

const rgblight_segment_t *const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(base_layer, one_layer, two_layer);

void keyboard_post_init_user(void) {
    // Enable the LED layers
    rgblight_layers = my_rgb_layers;
}

layer_state_t layer_state_set_user(layer_state_t state) {
    rgblight_set_layer_state(1, layer_state_cmp(state, _ONE));
    rgblight_set_layer_state(2, layer_state_cmp(state, _TWO));
    return state;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(5, led_state.caps_lock);
    return true;
}

#else

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _ONE, _TWO, _ADJUST);
}

#endif // ENDIF RGBLIGHT_ENABLE