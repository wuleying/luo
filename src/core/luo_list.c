#include "luo_config.h"
#include "luo_core.h"

// 创建链表
luo_list_t *
luo_list_create(luo_pool_t *pool, luo_uint_t n, size_t size)
{
	luo_list_t *list;

	list = luo_palloc(pool, sizeof(luo_list_t));

	if (list == NULL)
	{
		return NULL;
	}

	if(luo_list_init(list, pool, n, size) != LUO_OK)
	{
		return NULL;
	}

	return list;
}

// 添加链表元素
void *
luo_list_push(luo_list_t *list)
{
	void *elt;
	luo_list_part_t *last;

	last = list->last;

	if (last->nelts == list->nalloc)
	{
		last = luo_palloc(list->pool, sizeof(luo_list_part_t));

		if (last == NULL)
		{
			return NULL;
		}

		last->elts = luo_palloc(list->pool, list->nalloc * list->size);

		if (last->elts == NULL)
		{
			return NULL;
		}

		last->nelts = 0;
		last->next = NULL;

		list->last->next = last;
		list->last = last;
	}

	elt = (char *) last->elts + list->size * last->nelts;

	last->nelts++;

	return elt;
}
