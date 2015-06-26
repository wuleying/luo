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

typedef void (*luo_pool_cleanup_pt)(void *data);

typedef struct luo_pool_large_s luo_pool_large_t;
typedef struct luo_pool_cleanup_s luo_pool_cleanup_t;

typedef struct
{
	u_char *last;
	u_char *end;
	luo_pool_t *next;
	luo_uint_t failed;
} luo_pool_data_t;

typedef struct
{
	luo_fd_t fd;
	u_char *name;
	luo_log_t *log;
} luo_pool_cleanup_file_t;

struct luo_pool_large_s
{
	luo_pool_large_t *next;
	void *alloc;
};

struct luo_pool_cleanup_s
{
	luo_pool_cleanup_pt handler;
	void *data;
	luo_pool_cleanup_t *next;
};

// 内存池结构体
struct luo_pool_s
{
	luo_pool_data_t d;
	size_t max;
	luo_pool_t *current;
	luo_chain_t *chain;
	luo_pool_large_t *large;
	luo_pool_cleanup_t *cleanup;
	luo_log_t *log;
};

void *luo_alloc(size_t size, luo_log_t *log);
void *luo_calloc(size_t size, luo_log_t *log);

// 创建内存池
luo_pool_t *luo_create_pool(size_t size, luo_log_t *log);
// 销毁内存池
void luo_destroy_pool(luo_pool_t *pool);
// 重置内存池
void luo_reset_pool(luo_pool_t *pool);

void *luo_palloc(luo_pool_t *pool, size_t size);
void *luo_pnalloc(luo_pool_t *pool, size_t size);
void *luo_pcalloc(luo_pool_t *pool, size_t size);
void *luo_pmemalign(luo_pool_t *pool, size_t size, size_t alignment);
luo_int_t luo_pfree(luo_pool_t *pool, void *p);

luo_pool_cleanup_t *luo_cleanup_pool_add(luo_pool_t *pool, size_t size);
void luo_pool_run_cleanup_file(luo_pool_t *pool, luo_fd_t fd);
void luo_pool_cleanup_file(void *data);
void luo_pool_delete_file(void *data);

#endif /* _LUO_LOG_H_INCLUDED_ */
