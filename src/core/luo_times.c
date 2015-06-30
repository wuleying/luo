#include "luo_config.h"
#include "luo_core.h"

#define LUO_TIME_SLOTS 64

static luo_uint_t slot;
static luo_atomic_t luo_time_lock;
