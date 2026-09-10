#include QMK_KEYBOARD_H
#include "state_sync.h"

#define SYM 1
#define FN 2

#define CTL_ESC LCTL_T(KC_ESC)
#define SFT_GRV LSFT_T(KC_GRV)
#define SFT_BSL RSFT_T(KC_BSLS)
#define SYM_ENT LT(SYM, KC_ENT)
#define SYM_OSL OSL(SYM)
#define CMD_LCB G(KC_LCBR)
#define CMD_RCB G(KC_RCBR)
#define CMD_LBR G(KC_LBRC)
#define CMD_RBR G(KC_RBRC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,            KC_Y,    KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
        CTL_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,            KC_H,    KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        SFT_GRV, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,            KC_N,    KC_M,   KC_COMM, KC_DOT,  KC_SLSH, SFT_BSL,
        MO(FN),  KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, SYM_OSL,         SYM_ENT, KC_SPC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
    [SYM] = LAYOUT_ortho_4x12(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,         KC_UNDS, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN, KC_DQUO,
        _______, KC_LT,   KC_GT,   KC_ASTR, KC_AMPR, KC_CIRC,         KC_EQL,  KC_LBRC, KC_RBRC, KC_PLUS, KC_QUES, _______,
        _______, _______, _______, _______, _______, XXXXXXX,         _______, _______, _______, _______, _______, _______
    ),
    [FN] = LAYOUT_ortho_4x12(
        _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,           KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
        KC_ESC,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX, CMD_LCB, CMD_RCB, XXXXXXX, XXXXXXX, KC_ENT,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, QK_BOOT,         XXXXXXX, CMD_LBR, CMD_RBR, XXXXXXX, XXXXXXX, _______,
        _______, _______, _______, _______, KC_SPC,  _______,         _______, _______, _______, _______, _______, _______
    )
};

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    // OSL runs through the tapping state machine, so holding it would otherwise
    // delay each symbol until the symbol key's own release.
    return keycode == SYM_OSL;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    // OSL's release only clears ONESHOT_PRESSED, so a hold leaves the layer
    // armed for whatever is left of ONESHOT_TIMEOUT. Only a tap should arm it.
    if (keycode == SYM_OSL && !record->event.pressed && !record->tap.count) {
        clear_oneshot_layer_state(ONESHOT_START);
    }
    return true;
}

// Both shift keys on the left hand carry MOD_BIT_LSHIFT, and QMK tracks mods as
// a bitmask rather than a refcount, so whichever releases first drops shift out
// from under the other.
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
    static bool thumb_held, pinky_held;
    switch (keycode) {
        case KC_LSFT:
            thumb_held = record->event.pressed;
            break;
        case SFT_GRV:
            // A tap sends its own keycode and never owns the mod.
            pinky_held = record->event.pressed && !record->tap.count;
            break;
        default:
            return;
    }
    if (!record->event.pressed && (thumb_held || pinky_held)) {
        register_mods(MOD_BIT_LSHIFT);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    rgb_matrix_set_color_all(0, 0, 0);

    if (is_caps_word_on()) {
        for (uint8_t col = 0; col < MATRIX_COLS; col++) {
            rgb_matrix_set_color(g_led_config.matrix_co[0][col], 80, 80, 80);
            rgb_matrix_set_color(g_led_config.matrix_co[5][col], 80, 80, 80);
        }
    }

    if (is_osl_armed()) {
        rgb_matrix_set_color(g_led_config.matrix_co[3][5], 0, 40, 0); // thumb sym
        rgb_matrix_set_color(g_led_config.matrix_co[8][0], 0, 40, 0); // thumb sym/enter
    }

    rgb_matrix_set_color(g_led_config.matrix_co[1][4], 0, 0, 40); // F
    rgb_matrix_set_color(g_led_config.matrix_co[6][1], 0, 0, 40); // J
    rgb_matrix_set_color(g_led_config.matrix_co[3][4], 0, 0, 40); // thumb shift
    rgb_matrix_set_color(g_led_config.matrix_co[8][1], 0, 0, 40); // thumb space

    return false;
}
