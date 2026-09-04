#include QMK_KEYBOARD_H
#include <string.h>
#include "transactions.h"
#include "state_sync.h"

typedef struct {
    bool caps_word;
    bool osl_armed;
} sync_state_t;

static bool mirrored_osl_armed = false;

bool is_osl_armed(void) {
    if (!is_keyboard_master()) {
        return mirrored_osl_armed;
    }
    // A press sets ONESHOT_START, which is PRESSED|OTHER_KEY_PRESSED; only
    // OTHER_KEY_PRESSED survives the release that arms the layer.
    uint8_t state = get_oneshot_layer_state();
    return state && !(state & ONESHOT_PRESSED);
}

// Caps Word and one-shot state are tracked only on the master, so mirror them
// across the link to keep both halves' indicators truthful.
static void recv_state(uint8_t in_len, const void *in_data, uint8_t out_len, void *out_data) {
    const sync_state_t *state = in_data;
    if (state->caps_word != is_caps_word_on()) {
        caps_word_toggle();
    }
    mirrored_osl_armed = state->osl_armed;
}

void keyboard_post_init_user(void) {
    transaction_register_rpc(RPC_ID_SYNC_STATE, recv_state);
}

void housekeeping_task_user(void) {
    if (!is_keyboard_master()) {
        return;
    }
    // Leaving last_sent alone on a failed send retries on the next tick.
    static sync_state_t last_sent = {0};
    sync_state_t        state     = {is_caps_word_on(), is_osl_armed()};
    if (memcmp(&state, &last_sent, sizeof(state)) != 0 && transaction_rpc_send(RPC_ID_SYNC_STATE, sizeof(state), &state)) {
        last_sent = state;
    }
}
