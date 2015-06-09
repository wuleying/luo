#ifndef _LUO_STRING_H_INCLUDED_
#define _LUO_STRING_H_INCLUDED_

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
#define luo_str_set(str, text) (str)->len = sizeof(text) - 1; (str)->data = (u_char *) text;
// 给一个字符串赋空值
#define luo_str_null(str) (str)->len = 0; (str)->data = NULL;

#endif
