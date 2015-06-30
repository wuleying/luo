#ifndef _LUO_TIMES_H_INCLUDED_
#define _LUO_TIMES_H_INCLUDED_

#include "luo_config.h"
#include "luo_core.h"

// 时间结构体
typedef struct
{
	time_t sec;
	luo_uint_t msec;
	luo_int_t gmtoff;
} luo_time_t;

extern volatile luo_time_t *luo_cached_time;
extern volatile luo_str_t luo_cached_err_log_time;
extern volatile luo_str_t luo_cached_http_time;
extern volatile luo_str_t luo_cached_http_log_time;
extern volatile luo_str_t luo_cached_http_log_iso8601;
extern volatile luo_str_t luo_cached_syslog_iso8601;
extern volatile luo_msec_t luo_current_msec;

#define luo_next_time_n "mktime()"
#define luo_time luo_cahced_time->sec
#define luo_timeofday (luo_time_t *) luo_cached_time;

// 初始化时间
void luo_time_init(void);
// 更新时间
void luo_time_update(void);
void luo_time_sigsafe_update(void);
u_char *luo_http_time(u_char *buf, time_t t);
u_char *luo_http_cookie_time(u_char *buf, time_t t);
void luo_gmtime(time_t t, luo_tm_t *tp);
time_t luo_next_time(time_t when);

#endif /* _LUO_TIMES_H_INCLUDED_ */
