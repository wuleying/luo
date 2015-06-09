#ifndef _LUO_STRING_H_INCLUDED_
#define _LUO_STRING_H_INCLUDED_

#include <luo_config.h>

// 字符串
typedef struct {
	// 字符串长度
	size_t len;
	// 字符串值
	u_char *data;
} luo_str_t;

// 键值对
typedef struct {
	// 键
	luo_str_t key;
	// 值
	luo_str_t value;
} luo_keyval_t;

// 变量值
typedef struct {
	// 字符串长度
	unsigned len:28;
	// 是否有效
	unsigned valid:1;
	// 是否可缓存
	unsigned no_cacheable:1;
	// 是否不存在
	unsigned not_found:1;
	// todo
	unsigned escape:1;
	// 字符串值
	u_char *data;
} luo_variable_value_t;

// 设置一个字符串
#define luo_string(str) { sizeof(str) - 1, (u_char *) str }
// 设置一个空字符串
#define luo_null_string { 0, NULL }
// 给一个字符串赋值
#define luo_str_set(str, text) (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text
// 给一个字符串赋空值
#define luo_str_null(str) (str)->len = 0; (str)->data = NULL

// 大写转小写
#define luo_tolower(c) (u_char) ((c >= 'A' && C <= 'Z') ? (c | 0x20) : c)
// 小写转大写
#define luo_toupper(c) (u_char) ((c >= 'a' && C <= 'z') ? (c & ~0x20) : c)

// 比较两个字符串的n个字符
#define luo_strncmp(s1, s2, n) strncmp((const char *) s1, (const char *) s2, n)
// 比较两个字符串
#define luo_strcmp(s1, s2) strcmp((const char *) s1, (const char *) s2)
// 查找s2在s1中第一次出现的位置
#define luo_strstr(s1, s2) strstr((const char *) s1, (const char *) s2)
// 获取字符串长度
#define luo_strlen(s) strlen((const char *) s)
// 查找字符串s中首次出现字符c的位置
#define luo_strchr(s, c) strchr((const char *) s, (int) c)

#endif
