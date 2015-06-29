#ifndef _LUO_LIST_H_INCLUDED_
#define _LUO_LIST_H_INCLUDED_

#include "luo_config.h"
#include "luo_core.h"

// 链表节点
typedef struct luo_list_part_s luo_list_part_t;

// 链表节点结构体
struct luo_list_part_s
{
	void *elts;
	luo_uint_t nelts;
	luo_list_part_t *next;
};

// 链表接构体
typedef struct
{
	luo_list_part_t *last;
	luo_list_part_t part;
	size_t size;
	luo_uint_t nalloc;
	luo_pool_t *pool;
} luo_list_t;

// 创建链表
luo_list_t *luo_list_create(luo_pool_t *pool, luo_uint_t n, size_t size);

// 添加链表元素
void *luo_list_push(luo_list_t *list);

// 初始化链表
static luo_inline luo_int_t
luo_list_init(luo_list_t *list, luo_pool_t *pool, luo_uint_t n, size_t size)
{
	list->part.elts = luo_palloc(pool, n * size);

	if (list->part.elts == NULL)
	{
		return LUO_ERROR;
	}

	list->part.nelts = 0;
	list->part.next = NULL;
	list->last = &list->part;
	list->size = size;
	list->nalloc = n;
	list->pool = pool;

	return LUO_OK;
}

#endif /* _LUO_LIST_H_INCLUDED_ */
