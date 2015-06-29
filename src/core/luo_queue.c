#include "luo_config.h"
#include "luo_core.h"

// 获取队列的中间节点
luo_queue_t *
luo_queue_middle(luo_queue_t *queue)
{
	luo_queue_t *middle, *next;

	middle = luo_queue_head(queue);

	if (middle == luo_queue_last(queue))
	{
		return middle;
	}

	next = luo_queue_head(queue);

	for (;;)
	{
		middle = luo_queue_next(middle);

		next = luo_queue_next(next);

		if (next == luo_queue_last(queue))
		{
			return middle;
		}

		next = luo_queue_next(next);

		if (next == luo_queue_last(queue))
		{
			return middle;
		}
	}
}

// 队列排序
void luo_queue_sort(luo_queue_t *queue,
		luo_int_t (*cmp)(const luo_queue_t *, const luo_queue_t *))
{
	luo_queue_t *q, *prev, *next;

	q = luo_queue_head(queue);

	if (q == luo_queue_last(queue))
	{
		return;
	}

	for (q = luo_queue_next(q); q != luo_queue_sentinel(queue); q = next)
	{
		// 前驱节点
		prev = luo_queue_prev(q);
		// 后缀节点
		next = luo_queue_next(q);

		// 删除当前节点
		luo_queue_remove(q);

		do
		{
			if (cmp(prev, q) <= 0)
			{
				break;
			}

			prev = luo_queue_prev(prev);

		} while (prev != luo_queue_sentinel(queue));

		// 将q节点插入到prev节点之前
		luo_queue_insert_after(prev, q);
	}
}
