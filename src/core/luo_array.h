#ifndef _LUO_ARRAY_H_INCLUDED_
#define _LUO_ARRAY_H_INCLUDED_

#include "luo_config.h"
#include "luo_core.h"

// 数组结构体
typedef struct
{
	// 数组元素数据区起始位置
	void *elts;
	// 数组元素实际个数
	luo_uint_t nelts;
	// 数组单个元素大小
	size_t size;
	// 数组所含空间个数 实际分配的空间个数
	luo_uint_t nalloc;
	// 数组在此内存池内分配
	luo_pool_t *pool;
} luo_array_t;

// 创建数组
luo_array_t *luo_array_create(luo_pool_t *pool, luo_uint_t n, size_t size);
// 销毁数组
void luo_array_destroy(luo_array_t *array);
// 向数组中添加一个元素
void *luo_array_push(luo_array_t *array);
// 向数组中添加N个元素
void *luo_array_push_n(luo_array_t *array, luo_uint_t n);

// 初始化数组
static luo_inline luo_int_t luo_array_init(luo_array_t *array, luo_pool_t *pool,
		luo_uint_t n, size_t size)
{
	array->nelts = 0;
	array->size = size;
	array->nalloc = n;
	array->pool = pool;

	array->elts = luo_palloc(pool, n * size);

	if (array->elts == NULL)
	{
		return LUO_ERROR;
	}

	return LUO_OK;
}

#endif /* _LUO_ARRAY_H_INCLUDED_ */
