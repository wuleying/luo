#ifndef _LUO_PALLOC_H_INCLUDE_
#define _LUO_PALLOC_H_INCLUDE_

#include "luo_config.h"
#include "luo_core.h"

#define LUO_MAX_ALLOC_FROM_POOL (luo_pagesize - 1)

#define LUO_DEFAULT_POOL_SIZE (16 * 1024)

#define LUO_POOL_ALIGNMENT 16

#define LUO_MIN_POOL_SIZE 												\
	luo_align((sizeof(luo_pool_t) + 2 * sizeof(luo_pool_large_t)), 		\
			LUO_POOL_ALIGNMENT)


typedef void(*luo_pool_cleanup_pt)(void *data);

typedef struct luo_pool_large_s luo_pool_large_t;

typedef struct luo_pool_cleanup_s luo_pool_cleanup_t;

typedef struct {
	u_char *last;
	u_char *end;
	luo_pool_t *next;
	luo_uint_t failed;
} luo_pool_data_t;

struct luo_pool_large_s {
	luo_pool_large_t *next;
	void *alloc;
};

struct luo_pool_cleanup_s {
	luo_pool_cleanup_pt handler;
	void *data;
	luo_pool_cleanup_t *next;
};

// 内存池结构体
struct luo_pool_s {
	luo_pool_data_t d;
	size_t max;
	luo_pool_t *current;
	luo_chain_t *chain;
	luo_pool_large_t *large;
	luo_pool_cleanup_t *cleanup;
	luo_log_t *log;
};


#endif /* _LUO_LOG_H_INCLUDED_ */
