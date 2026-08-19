#include QMK_KEYBOARD_H
#include "transactions.h"

// Caps Word is tracked only on the master, so mirror it across the link to
// keep is_caps_word_on() truthful on both halves.
static void recv_caps_word_state(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    if (*(const bool *)in_data != is_caps_word_on()) {
        caps_word_toggle();
    }
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_ID_SYNC_CAPS_WORD, recv_caps_word_state);
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) {
        return;
    }
    // Leaving last_sent alone on a failed send retries on the next tick.
    static bool last_sent = false;
    bool        state     = is_caps_word_on();
    if (state != last_sent && transaction_rpc_send(RPC_ID_SYNC_CAPS_WORD, sizeof(state), &state)) {
        last_sent = state;
    }
}
