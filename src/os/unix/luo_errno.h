#ifndef _LUO_ERRNO_H_INCLUDED_
#define _LUO_ERRNO_H_INCLUDED_

#include "./../../core/luo_config.h"
#include "./../../core/luo_core.h"

typedef int luo_err_t;

#define LUO_ENOENT		ENOENT

#define luo_errno		errno

#endif /* _LUO_ERRNO_H_INCLUDED_ */
