#ifndef _LUO_LOG_H_INCLUDED_
#define _LUO_LOG_H_INCLUDED_

#include "luo_config.h"
#include "luo_core.h"

// 日志结构体
// todo 未完成
struct luo_log_s {
	// 日志级别
	luo_uint_t log_level;

	time_t disk_full_time;

	void *data;

	void *wdata;

	char *action;

	luo_log_t *next;
};

#endif
