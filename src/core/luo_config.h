#ifndef _LUO_CONFIG_H_INCLUDED_
#define _LUO_CONFIG_H_INCLUDED_

#include "./../os/unix/luo_posix_config.h"

typedef intptr_t luo_int_t;
typedef uintptr_t luo_uint_t;
typedef intptr_t luo_flag_t;

#define luo_align(d, a) (((d) + (a - 1)) & ~(a - 1))

#endif /* _LUO_CONFIG_H_INCLUDED_ */
