#pragma once
#ifndef UTIL_H
#define UTIL_H

#include <errno.h>

#define ERRNOCHECK(condition, errmsg, lbl)                             \
	do {                                                           \
		if (condition) {                                       \
			int errsv = errno;                             \
			fprintf(stderr, "%s:%d:%s: Error: ", __FILE__, \
				__LINE__, __func__);                   \
			errno = errsv;                                 \
			perror((errmsg));                              \
			goto lbl;                                      \
		}                                                      \
	} while (0)

#endif // !UTIL_H