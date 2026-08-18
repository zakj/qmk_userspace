#include QMK_KEYBOARD_H

#define CTL_ESC LCTL_T(KC_ESC)
#define CMD_LCB G(KC_LCBR)
#define CMD_RCB G(KC_RCBR)
#define CMD_LBR G(KC_LBRC)
#define CMD_RBR G(KC_RBRC)

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT_ortho_4x12(
        KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,          KC_Y,  KC_U,   KC_I,    KC_O,    KC_P,    KC_BSPC,
        CTL_ESC, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,          KC_H,  KC_J,   KC_K,    KC_L,    KC_SCLN, KC_QUOT,
        KC_GRV,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,          KC_N,  KC_M,   KC_COMM, KC_DOT,  KC_SLSH, SC_SENT,
        MO(2),   KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, MO(1),         MO(1), KC_SPC, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT
    ),
    [1] = LAYOUT_ortho_4x12(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,            KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    _______,
        _______, KC_EXLM, KC_AT,   KC_HASH, KC_DLR,  KC_PERC,         KC_UNDS, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN, KC_DQUO,
        _______, KC_LT,   KC_GT,   KC_ASTR, KC_AMPR, KC_CIRC,         KC_BSLS, KC_LBRC, KC_RBRC, KC_PLUS, KC_EQL,  _______,
        _______, _______, _______, _______, _______, _______,         _______, _______, _______, _______, _______, _______
    ),
    [2] = LAYOUT_ortho_4x12(
        KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,           KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,
        KC_LCTL, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX, CMD_LCB, CMD_RCB, XXXXXXX, XXXXXXX, XXXXXXX,
        _______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,         XXXXXXX, CMD_LBR, CMD_RBR, XXXXXXX, XXXXXXX, _______,
        _______, _______, _______, _______, KC_SPC,  _______,         _______, _______, _______, _______, _______, _______
    )
};

const key_override_t bspc_del = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, KC_DEL);
const key_override_t *key_overrides[] = { &bspc_del, NULL };
