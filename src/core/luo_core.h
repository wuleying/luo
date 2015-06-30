#ifndef _LUO_CORE_H_INCLUDED_
#define _LUO_CORE_H_INCLUDED_

#include "luo_config.h"

typedef struct luo_log_s luo_log_t;
typedef struct luo_pool_s luo_pool_t;
typedef struct luo_chain_s luo_chain_t;

#define LUO_OK 		 	 0
#define LUO_ERROR 		-1
#define LUO_AGAIN		-2
#define LUO_BUSY		-3
#define LUO_DONE		-4
#define LUO_DECLINED	-5
#define LUO_ABORT		-6

#include "../os/unix/luo_alloc.h"
#include "../os/unix/luo_files.h"
#include "../os/unix/luo_errno.h"

#include "luo_string.h"
#include "luo_log.h"
#include "luo_palloc.h"
#include "luo_queue.h"
#include "luo_list.h"
#include "luo_array.h"

#endif
