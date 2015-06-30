#ifndef _LUO_CONFIG_H_INCLUDED_
#define _LUO_CONFIG_H_INCLUDED_

#include "../os/unix/luo_posix_config.h"

typedef intptr_t luo_int_t;
typedef uintptr_t luo_uint_t;
typedef intptr_t luo_flag_t;

#ifndef LUO_ALIGNMENT
#define LUO_ALIGNMENT sizeof(unsigned long)
#endif

#define luo_align(d, a) (((d) + (a - 1)) & ~(a - 1))
#define luo_align_ptr(p, a)														\
		(u_char *) (((uintptr_t) (p) + ((uintptr_t) a - 1)) & ~((uintptr_t) a - 1))

#ifndef luo_inline
#define luo_inline inline
#endif

#endif /* _LUO_CONFIG_H_INCLUDED_ */
