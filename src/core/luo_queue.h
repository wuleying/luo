#ifndef _LUO_QUEUE_H_INCLUDED_
#define _LUO_QUEUE_H_INCLUDED_

#include "luo_config.h"
#include "luo_core.h"

typedef struct luo_queue_s luo_queue_t;

// 队列结构体 双向链接
struct luo_queue_s
{
	// 前驱
	luo_queue_t *prev;
	// 后续
	luo_queue_t *next;
};

// 初始化队列
#define luo_queue_init(q)									\
	(q)->prev = q;											\
	(q)->next = q

// 空队列
#define luo_queue_empty(h)									\
	(h == (h)->prev)

// 在头节点之后插入新节点
#define luo_queue_insert_head(h, x)							\
	(x)->next = (h)->next;									\
	(x)->next->prev = x;									\
	(x)->prev = h;											\
	(h)->next = x

#define luo_queue_insert_after	luo_queue_insert_head

// 在尾节点之后插入新节点
#define luo_queue_insert_tail(h, x)							\
	(x)->prev = (h)->prev;									\
	(x)->prev->next = x;									\
	(x)->prev = (h);										\
	(h)->next = (x)

// 头节点
#define luo_queue_head(h)									\
	(h)->next

// 尾节点
#define luo_queue_last(h)									\
	(h)->prev

// 头部标志节点
#define luo_queue_sentinel(h)								\
	(h)

// 下一个节点
#define luo_queue_next(h)									\
	(h)->next

// 上一个节点
#define luo_queue_prev(h)									\
	(h)->prev

// 删除一个节点
#define luo_queue_remove(x)									\
	(x)->prev->next = (x)->next;							\
	(x)->next->prev = (x)->prev

// 分隔队列
#define luo_queue_split(h, q, n)							\
    (n)->prev = (h)->prev;									\
    (n)->prev->next = n;									\
    (n)->next = q;											\
    (h)->prev = (q)->prev;									\
    (h)->prev->next = (h);									\
    (q)->prev = n

// 链接队列
#define luo_queue_add(h, n)									\
	(h)->prev->next = (n)->next;							\
	(n)->next->prev = (h)->prev;							\
	(h)->prev = (n)->prev;									\
	(h)->prev->next = h

// 获取队列中节点数据 q是队列中的节点 type是队列类型 link是队列类型中luo_queue_t的元素名
#define luo_queue_data(q, type, link)						\
	(type *) ((u_char *) q - offsetof(type, link))

// 队列的中间节点
luo_queue_t *luo_queue_middle(luo_queue_t *queue);

void luo_queue_sort(luo_queue_t *queue,
		luo_int_t (*cmp)(const luo_queue_t *, const luo_queue_t *));

#endif /* _LUO_QUEUE_H_INCLUDED_ */
