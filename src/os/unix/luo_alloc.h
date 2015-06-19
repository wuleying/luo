#ifndef _LUO_ALLOC_H_INCLUDED_
#define _LUO_ALLOC_H_INCLUDED_

#include "./../../core/luo_config.h"
#include "./../../core/luo_core.h"

// 分配长度为size字节的内存块
void *luo_alloc(size_t size, luo_log_t *log);
// 分配长度为size字节的内存块,并将值填充为0
void *luo_calloc(size_t size, luo_log_t *log);

// 用来释放申请的空间
#define luo_free free

// todo
#define luo_memalign(alignment, size, log) luo_alloc(size, log)

// todo
extern luo_uint_t luo_pagesize;
extern luo_uint_t luo_pagesize_shift;
extern luo_uint_t luo_cacheline_size;

#endif /* _LUO_ALLOC_H_INCLUDED_ */
