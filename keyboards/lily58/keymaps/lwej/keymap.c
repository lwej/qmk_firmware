#include QMK_KEYBOARD_H
#include "keymap_swedish.h"
#include "process_tap_dance.h"

#define KC_C_L RCTL(KC_LEFT)
#define KC_C_R RCTL(KC_RGHT)

enum layer_number {
  _QWERTY = 0,
  _LOWER,
  _RAISE,
  _ADJUST,
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
/* QWERTY
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * | ESC  |   1  |   2  |   3  |   4  |   5  |                    |   6  |   7  |   8  |   9  |   0  | BSPC |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * | Tab  |   Q  |   W  |   E  |   R  |   T  |                    |   Y  |   U  |   I  |   O  |   P  |  Å   |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |LCTRL |   A  |   S  |   D  |   F  |   G  |-------.    ,-------|   H  |   J  |   K  |   L  |   ;  |  '   |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |LShift|   Z  |   X  |   C  |   V  |   B  |-------|    |-------|   N  |   M  |   ,  |   .  |   /  |RShift|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |      |      |      |
 *                   `----------------------------'           '------''--------------------'
 */

 [_QWERTY] = LAYOUT(
  KC_ESC,   KC_1,   KC_2,    KC_3,    KC_4,    KC_5,                   KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_BSPC,
  KC_TAB,   KC_Q,   KC_W,    KC_E,    KC_R,    KC_T,                   KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC,
  KC_LCTL,  KC_A,   KC_S,    KC_D,    KC_F,    KC_G,                   KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
  KC_LSFT,  KC_Z,   KC_X,    KC_C,    KC_V,    KC_B, XXXXXXX, XXXXXXX, KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_ESC,
                      KC_LALT, TD(_MT_L), MO(_LOWER), KC_SPC, KC_ENT, MO(_RAISE), KC_BSPC, KC_RGUI
),
/* LOWER
 * ,-----------------------------------------.                    ,-----------------------------------------.
 * |  F1  |  F2  |  F3  |  F4  |  F5  |  F6  |                    |  F7  |  F8  |  F9  | F10  | F11  | F12  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   !  |  "   |  #   |  $   |  %   |                    |  &   |  /   |   (  |   )  |  =   | DEL  |
 * |------+------+------+------+------+------|                    |------+------+------+------+------+------|
 * |      |   (  |   )  |   {  |   }  |   å  |-------.    ,-------| LEFT | DOWN |  UP  | RIGHT|   §  |   *  |
 * |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
 * |   |  |   [  |   ]  |   /  |   \  |   `  |-------|    |-------|CLEFT |CRIGHT|   <  |   >  |   +  |S(SPC)|
 * `-----------------------------------------/       /     \      \-----------------------------------------'
 *                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |ADJUST |BackSP| RGUI |
 *                   |      |      |      |/       /         \      \ |       |      |      |
 *                   `----------------------------'           '------''--------------------'
 */
 [_LOWER] = LAYOUT(
  KC_F1,     KC_F2,    KC_F3,    KC_F4,    KC_F5,   KC_F6,                     KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,
  TD(T_ESC), SE_EXLM,  SE_DQUO,  SE_HASH,  SE_DLR,  SE_PERC,                   SE_AMPR,  SE_SLSH,  SE_LPRN,  SE_RPRN,  SE_EQL,   KC_DEL,
  _______,   SE_LPRN,  SE_RPRN,  SE_LCBR,  SE_RCBR, SE_ARNG,                   KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_GRV,   SE_ASTR,
  _______,   SE_LBRC,  SE_RBRC,  SE_SLSH,  SE_BSLS, SE_ASTR, _______, _______, KC_C_L,   KC_C_R,   SE_LABK,  SE_RABK,  SE_PLUS,  RSFT_T(KC_SPC),
                                  _______, _______, _______, _______, _______,   MO(3),   _______, _______
),
/* RAISE
* ,-----------------------------------------.                    ,-----------------------------------------.
* |ESC_~ |   ´  |   @  |  `   |      |      |                    |   ^  |      |      |      |      |   §  |
* |------+------+------+------+------+------|                    |------+------+------+------+------+------|
* |      |      |      |      |      |      |                    |      |M_BTN1|M_BTN1|      |      |  ~   |
* |------+------+------+------+------+------|                    |------+------+------+------+------+------|
* |      |   ´  |   @  |  `   |   ¨  |   '  |-------.    ,-------|MS_LEF|MS_UP |MS_DOW|MS_RI |      |      |
* |------+------+------+------+------+------|   [   |    |    ]  |------+------+------+------+------+------|
* |   |  |      |      |      |      |   *  |-------|    |-------|MWH_LE|MWH_DO|MWH_UP|MWH_RI|      |      |
* `-----------------------------------------/       /     \      \-----------------------------------------'
*                   | LAlt | LGUI |ADJUST| /Space  /       \Enter \  |RAISE |BackSP| RGUI |
*                   |      |      |      |/       /         \      \ |      |      |      |
*                   `----------------------------'           '------''--------------------'
*/

 [_RAISE] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______,
  SE_TILD,  KC_1,   KC_2,    KC_3,   KC_4,    KC_5,                          KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_DEL,
  _______,  SE_ACUT,  SE_AT,  SE_GRV,  KC_RBRC, SE_ARNG,                     KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,
  SE_PIPE,  KC_ESC,  KC_RGUI, KC_RALT, SE_CIRC, SE_QUOT, _______, _______,   KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,
                              _______, _______,   MO(3), _______, _______,  _______, _______, _______
  ),
/* ADJUST
* ,-----------------------------------------.                    ,-----------------------------------------.
* | BOOT |      |      |      |      |      |                    |      |      |      |      |      |      |
* |------+------+------+------+------+------|                    |------+------+------+------+------+------|
* |      |      |      |      |      |      |                    |      |      |      |      |      |      |
* |------+------+------+------+------+------|                    |------+------+------+------+------+------|
* |      |      |      |      |      |      |-------.    ,-------|      |      |      |      |      |      |
* |------+------+------+------+------+------|       |    |       |------+------+------+------+------+------|
* |      |      |      |      |      |      |-------|    |-------|      |      |      |      |      |      |
* `-----------------------------------------/       /     \      \-----------------------------------------'
*                   | LAlt | LGUI |LOWER | /Space  /       \Enter \  |RAISE |BackSP| RGUI |
*                   |      |      |      |/       /         \      \ |      |      |      |
*                   `----------------------------'           '------''--------------------'
*/
 [_ADJUST] = LAYOUT(
  _______, _______, _______, _______, _______, _______,                    _______, _______, _______, _______, _______, _______,
  QK_BOOT, _______, _______, _______, _______, _______,                    _______,  KC_BTN1, KC_BTN2, _______, _______, _______,
  _______, _______, _______, _______, _______, _______,                    KC_MS_L,  KC_MS_D,  KC_MS_U, KC_MS_R, _______,_______,
  _______,  SE_ACUT,  SE_AT,  SE_GRV,  KC_RBRC, SE_QUOT, _______, _______, KC_WH_L, KC_WH_D, KC_WH_U, KC_WH_R, _______, _______,
                              _______, _______, _______, _______, _______,  _______, _______, _______
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
                set_oneshot_layer(_LOWER, ONESHOT_START);
                clear_oneshot_layer_state(ONESHOT_PRESSED);
                break;
            case TD_SINGLE_HOLD:
                register_code(KC_LGUI);
                break;
            case TD_DOUBLE_TAP:
                set_oneshot_layer(_LOWER, ONESHOT_START);
                set_oneshot_layer(_LOWER, ONESHOT_PRESSED);
                break;
            case TD_DOUBLE_HOLD:
                register_code(KC_LGUI);
                layer_on(_LOWER);
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
                layer_off(_LOWER);
            unregister_code(KC_LGUI);
            break;
        default:
            break;
    }
    mtl_tap_state.state = TD_NONE;
}

tap_dance_action_t tap_dance_actions[] = {
    [_MT_L] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, mod_tap_layer_finished, mod_tap_layer_reset),
    [T_ESC] = ACTION_TAP_DANCE_DOUBLE(KC_ESC, SE_TILD),
};


layer_state_t layer_state_set_user(layer_state_t state) {
  return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


