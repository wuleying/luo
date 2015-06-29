#include "luo_config.h"
#include "luo_core.h"

// 创建数组
luo_array_t *
luo_array_create(luo_pool_t *pool, luo_uint_t n, size_t size)
{
	luo_array_t *array;

	array = luo_palloc(pool, sizeof(luo_array_t));

	if (array == NULL)
	{
		return NULL;
	}

	if (luo_array_init(array, pool, n, size) != LUO_OK)
	{
		return NULL;
	}

	return array;
}

// 销毁数组
void luo_array_destroy(luo_array_t *array)
{
	luo_pool_t *pool;

	pool = array->pool;

	if ((u_char *) array->elts + array->size * array->nalloc == pool->d.last)
	{
		pool->d.last -= array->size * array->nalloc;
	}

	if ((u_char *) array + sizeof(luo_array_t) == pool->d.last)
	{
		pool->d.last = (u_char *) array;
	}
}

// 向数组中添加一个元素
void *
luo_array_push(luo_array_t *array)
{
	void *elt, *new;
	size_t size;
	luo_pool_t *pool;

	if (array->nelts == array->nalloc)
	{
		size = array->size * array->nalloc;

		pool = array->pool;

		if ((u_char *) array->elts + size == pool->d.last
				&& pool->d.last + array->size <= pool->d.end)
		{
			pool->d.last += array->size;
			array->nalloc++;
		}
		else
		{
			new = luo_palloc(pool, 2 * size);

			if (new == NULL)
			{
				return NULL;
			}

			luo_memcpy(new, array->elts, size);
			array->elts = new;
			array->nalloc *= 2;
		}
	}

	elt = (u_char *) array->elts + array->size * array->nelts;
	array->nelts++;

	return elt;
}

// 向数组中添加N个元素
void *
luo_array_push_n(luo_array_t *array, luo_uint_t n)
{
	void *elt, *new;
	size_t size;
	luo_uint_t nalloc;
	luo_pool_t *pool;

	// 计算N个元素的大小
	size = n * array->size;

	if (array->nelts + n > array->nalloc)
	{
		pool = array->pool;

		if ((u_char *) array->elts + array->size * array->nalloc == pool->d.last
				&& pool->d.last + size <= pool->d.end)
		{
			pool->d.last += size;
			array->nalloc += n;
		}
		else
		{
			nalloc = 2 * ((n >= array->nalloc) ? n : array->nalloc);

			new = luo_palloc(pool, nalloc * array->size);

			if (new == NULL)
			{
				return NULL;
			}

			luo_memcpy(new, array->elts, array->nelts * array->size);
			array->elts = new;
			array->nalloc = nalloc;
		}
	}

	elt = (u_char *) array->elts + array->size * array->nelts;
	array->nelts += n;

	return elt;
}

