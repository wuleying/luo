#include "./../../core/luo_config.h"
#include "./../../core/luo_core.h"

luo_uint_t luo_pagesize;
luo_uint_t luo_pagesize_shift;
luo_uint_t luo_cacheline_size;

// 分配长度为size字节的内存块
void *
luo_alloc(size_t size, luo_log_t *log)
{
	void *p;

	p = malloc(size);

	if(NULL == p)
	{
		// todo add log
	}

	// todo add log

	return p;
}

// 分配长度为size字节的内存块,并将值填充为0
void *
luo_calloc(size_t size, luo_log_t *log)
{
	void *p;

	p = luo_alloc(size, log);

	if(p)
	{
		luo_memzero(p, size);
	}

	return p;
}
