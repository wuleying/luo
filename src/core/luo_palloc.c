#include "luo_config.h"
#include "luo_core.h"

static void *luo_palloc_block(luo_pool_t *pool, size_t size);
static void *luo_palloc_large(luo_pool_t *pool, size_t size);

// 创建内存池
luo_pool_t *
luo_create_pool(size_t size, luo_log_t *log) {
	luo_pool_t *p;

	p = luo_memalign(LUO_POOL_ALIGNMENT, size, log);

	if (p == NULL) {
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
void luo_destroy_pool(luo_pool_t *pool) {
	luo_pool_t *p, *n;
	luo_pool_large_t *l;
	luo_pool_cleanup_t *c;

	for (c = pool->cleanup; c; c = c->next) {
		if (c->handler) {
			c->handler(c->data);
		}
	}

	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			luo_free(l->alloc);
		}
	}

	for (p = pool, n = pool->d.next;; p = n, p = n->d.next) {
		luo_free(p);

		if (n == NULL) {
			break;
		}
	}
}

// 重置内存池
void luo_reset_pool(luo_pool_t *pool) {
	luo_pool_t *p;
	luo_pool_large_t *l;

	for (l = pool->large; l; l = l->next) {
		if (l->alloc) {
			luo_free(l->alloc);
		}
	}

	for (p = pool; p; p = p->d.next) {
		p->d.last = (u_char *) p + sizeof(luo_pool_t);
		p->d.failed = 0;
	}

	p->current = pool;
	p->chain = NULL;
	p->large = NULL;
}

void *
luo_palloc(luo_pool_t *pool, size_t size) {
	u_char *m;
	luo_pool_t *p;

	if (size <= pool->max) {
		p = pool->current;

		do {
			m = luo_align_ptr(p->d.last, LUO_ALIGNMENT);

			if ((size_t) (p->d.end - m) >= size) {
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while (p);

		return luo_palloc_block(pool, size);
	}

	return luo_palloc_large(pool, size);
}

void *
luo_pnalloc(luo_pool_t *pool, size_t size) {
	u_char *m;
	luo_pool_t *p;

	if (size <= pool->max) {

		p = pool->current;

		do {
			m = p->d.last;

			if ((size_t) (p->d.end - m) >= size) {
				p->d.last = m + size;

				return m;
			}

			p = p->d.next;

		} while (p);

		return luo_palloc_block(pool, size);
	}

	return luo_palloc_large(pool, size);
}

// 调用luo_memalign分配一块与pool大小相同的内存, 前16字节为luo_pool_data_t结构体
void *
luo_palloc_block(luo_pool_t *pool, size_t size) {
	u_char *m;
	size_t psize;
	luo_pool_t *p, *new;

	// 计算pool的大小
	psize = (size_t) (pool->d.end - (u_char *) pool);

	// 分配一块与pool大小相同的内存
	m = luo_memalign(LUO_POOL_ALIGNMENT, psize, pool->log);

	if (m == NULL) {
		return NULL;
	}

	new = (luo_pool_t *) m;

	// 设置end指针
	new->d.end = m + psize;
	new->d.next = NULL;
	new->d.failed = 0;

	// 让m指向这块内存luo_pool_data_t结构体之后的数据区起始位置
	m += sizeof(luo_pool_data_t);
	// 按4字节对齐
	m = luo_align_ptr(m, LUO_ALIGNMENT);

	// 在数据区分配size大小的内存，并设置last指针
	new->d.last = m + size;

	for (p = pool->current; p->d.next; p = p->d.next) {
		if (p->d.failed++ > 4) {
			// 失败4次以上移动current指针
			pool->current = p->d.next;
		}
	}

	// 将当前分配的内存块new加入内存池
	p->d.next = new;

	return m;
}

void *
luo_palloc_large(luo_pool_t *pool, size_t size) {
	void *p;
	luo_uint_t n;
	luo_pool_large_t *large;

	p = luo_alloc(size, pool->log);

	if (p == NULL) {
		return NULL;
	}

	n = 0;

	for (large = pool->large; large; large = large->next) {
		if (large->alloc == NULL) {
			large->alloc = p;
			return p;
		}

		if (n++ > 3) {
			break;
		}
	}

	large = luo_palloc(pool, sizeof(luo_pool_large_t));

	if (large == NULL) {
		luo_free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}

void *
luo_pmemalign(luo_pool_t *pool, size_t size, size_t alignment) {
	void *p;
	luo_pool_large_t *large;

	p = luo_memalign(alignment, size, pool->log);

	if (p == NULL) {
		return NULL;
	}

	large = luo_palloc(pool, sizeof(luo_pool_large_t));

	if (large == NULL) {
		luo_free(p);
		return NULL;
	}

	large->alloc = p;
	large->next = pool->large;
	pool->large = large;

	return p;
}

luo_int_t luo_pfree(luo_pool_t *pool, void *p) {
	luo_pool_large_t *l;

	for (l = pool->large; l; l = l->next) {
		if (p == l->alloc) {
			luo_free(l->alloc);
			l->alloc = NULL;

			return LUO_OK;
		}
	}

	return LUO_DECLINED;
}

void *
luo_pcalloc(luo_pool_t *pool, size_t size) {
	void *p;

	p = luo_palloc(pool, size);

	if (p) {
		luo_memzero(p, size);
	}

	return p;
}

luo_pool_cleanup_t *
luo_cleanup_pool_add(luo_pool_t *pool, size_t size) {
	luo_pool_cleanup_t *c;

	c = luo_palloc(pool, sizeof(luo_pool_cleanup_t));

	if (c == NULL) {
		return NULL;
	}

	if (size) {
		c->data = luo_palloc(pool, size);

		if (c->data == NULL) {
			return NULL;
		}
	} else {
		c->data = NULL;
	}

	c->handler = NULL;
	c->next = pool->cleanup;

	pool->cleanup = c;

	return c;
}

void
luo_pool_run_cleanup_file(luo_pool_t *pool, luo_fd_t fd) {
	luo_pool_cleanup_t *c;
	luo_pool_cleanup_file_t *cf;

	for (c = pool->cleanup; c; c = c->next) {
		if (c->handler == luo_pool_cleanup_file) {
			cf = c->data;

			if (cf->fd == fd) {
				c->handler(cf);
				c->handler = NULL;

				return;
			}
		}
	}
}


void
luo_pool_cleanup_file(void *data) {
	luo_pool_cleanup_file_t *cf = data;

	if (luo_close_file(cf->fd) == LUO_FILE_ERROR) {
		// todo error_log
	}
}

