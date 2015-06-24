#include "luo_config.h"
#include "luo_core.h"

static void *luo_palloc_block(luo_pool_t *pool, size_t size);
static void *luo_palloc_large(luo_pool_t *pool, size_t size);

// 创建内存池
luo_pool_t *
luo_create_pool(size_t size, luo_log_t *log)
{
	luo_pool_t *p;

	p = luo_memalign(LUO_POOL_ALIGNMENT, size, log);

	if(p == NULL)
	{
		return NULL;
	}

	p->d.last = (u_char *) p + sizeof(luo_pool_t);
	p->d.end = (u_char *) p + size;
	p->d.next = NULL;
	p->d.failed = 0;

	size = size - sizeof(luo_pool_t);
	p->max = (size < LUO_MAX_ALLOC_FROM_POOL) ? size : LUO_MAX_ALLOC_FROM_POOL;

	p->current = p;
	p->chain = NULL;
	p->large = NULL;
	p->cleanup = NULL;
	p->log = log;

	return p;
}

// 销毁内存池
void
luo_destroy_pool(luo_pool_t *pool)
{
	luo_pool_t *p, *n;
	luo_pool_large_t *l;
	luo_pool_cleanup_t *c;

	for(c = pool->cleanup; c; c = c->next)
	{
		if(c->handler)
		{
			c->handler(c->data);
		}
	}

	for(l = pool->large; l; l = l->next)
	{
		if(l->alloc)
		{
			luo_free(l->alloc);
		}
	}

	for(p = pool, n = pool->d.next; ; p = n, p = n->d.next)
	{
		luo_free(p);

		if(n == NULL)
		{
			break;
		}
	}
}

// 重置内存池
void
luo_reset_pool(luo_pool_t *pool)
{
	luo_pool_t *p;
	luo_pool_large_t *l;

	for(l = pool->large; l; l = l->next)
	{
		if(l->alloc)
		{
			luo_free(l->alloc);
		}
	}

	for(p = pool; p; p = p->d.next)
	{
		p->d.last = (u_char *) p + sizeof(luo_pool_t);
		p->d.failed = 0;
	}

	p->current = pool;
	p->chain = NULL;
	p->large = NULL;
}

//
void *
luo_palloc(luo_pool_t *pool, size_t size)
{
	u_char *m;
	luo_pool_t *p;

	if(size <= pool->max)
	{
		p = pool->current;

		do {
			m = luo_align_ptr(p->d.last, LUO_ALIGNMENT);

			if((size_t) (p->d.end - m) >= size)
			{
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while(p);

		return luo_palloc_block(pool, size);
	}

	return luo_palloc_large(pool, size);
}

void *
luo_palloc_block(luo_pool_t *pool, size_t size)
{
	u_char *m;
	size_t psize;
	luo_pool_t *p, *new;

	psize = (size_t) (pool->d.end - (u_char *) pool);

	m = luo_memalign(LUO_POOL_ALIGNMENT, psize, pool->log);

	if(m == NULL)
	{
		return NULL;
	}

	new = (luo_pool_t *) m;

	new->d.end = m + psize;
	new->d.next = NULL;
	new->d.failed = 0;

	m += sizeof(luo_pool_data_t);
	m = luo_align_ptr(m, LUO_ALIGNMENT);

	new->d.last = m + size;

	for(p = pool->current; p->d.next; p = p->d.next)
	{
		if(p->d.failed++ > 4)
		{
			pool->current = p->d.next;
		}
	}

	p->d.next = new;

	return m;
}

void *
luo_palloc_large(luo_pool_t *pool, size_t size)
{
	void *p;
	luo_uint_t n;
	luo_pool_large_t *large;

	p = luo_alloc(size, pool->log);

	if(p == NULL)
	{
		return NULL;
	}

	n = 0;

	for(large = pool->large; large; large = large->next)
	{
		if(large->alloc == NULL)
		{
			large->alloc = p;
			return p;
		}

		if(n++ > 3)
		{
			break;
		}
	}

	large = luo_palloc(pool, sizeof(luo_pool_large_t));

	if(large == NULL)
	{
		luo_free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}
