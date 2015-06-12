#include "luo_config.h"
#include "luo_core.h"
#include "luo.h"

int main()
{
	luo_str_t *str;
	luo_str_set(str, "hello world");

	printf("%zu\n%s", str->len, str->data);

	return 0;
}
