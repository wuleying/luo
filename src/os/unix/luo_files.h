#ifndef _LUO_FILES_H_INCLUDED_
#define _LUO_FILES_H_INCLUDED_

typedef int luo_fd_t;

#define LUO_INVALID_FILE		-1
#define LUO_FILE_ERROR			-1

#define luo_close_file 			close
#define luo_close_file_n 		"close()"

#define luo_delete_file(name)	unlink((const char *) name)
#define luo_delete_file_n		"unlink()"

#endif
