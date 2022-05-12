// Copyright 2022 Maria Sfiraiala (maria.sfiraiala@stud.acs.upb.ro)

#ifndef UTILS_H__
#define UTILS_H__

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

/* useful macro for handling error codes */
#define DIE(assertion, call_description)				\
	do {								\
		if (assertion) {					\
			fprintf(stderr, "(%s, %d): ",			\
					__FILE__, __LINE__);		\
			perror(call_description);			\
			exit(errno);				        \
		}							\
	} while (0)

#endif  // UTILS_H__
