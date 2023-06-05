#include "hdhelper.h"

#ifndef HDLOG_H_2365
#define HDLOG_H_2365

#define	STRINGIFY(x) #x
#define	TOSTRING(x)  STRINGIFY(x)

#ifdef _WIN32
#	define WIN32_LEAN_AND_MEAN
#	define NOMINMAX
#	include <Windows.h>
#	define platform_log(text) OutputDebugString(TEXT(":" __FILE__ "(" TOSTRING(__LINE__) "){" __FUNCTION__ "}:" TEXT(text "\n")))	
#else
#	define platform_log(text)
#endif

#define log_if(condition)							\
	if(condition) do {								\
		platform_log(#condition);					\
		fprintf(stderr, "%s\n", #condition);		\
	} while(0)

#define fail_with_val_if(val, condition)			\
	if(condition) do {								\
		platform_log(#condition);					\
		fprintf(stderr, "%s\n", #condition);		\
		return (val);								\
	} while(0)

#define log_if_failed(condition)	log_if(0 > (int32_t)(condition))
#define fail_if(condition)			fail_with_val_if(-1, condition)
#define fail_if_failed(condition)	fail_if(0 > (int32_t)(condition))
#define exit_if_failed(condition)	fail_with_val_if(EXIT_FAILURE, condition)

#endif // HDLOG_H_2365
