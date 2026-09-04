#pragma once

#include <stdbool.h>

// True while a one-shot layer is waiting for the next key, as opposed to being
// held down.
bool is_osl_armed(void);
