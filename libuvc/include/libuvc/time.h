#define _CRT_NO_TIME_T
#define HAVE_STRUCT_TIMESPEC

//#include <time.h>
#include <winsock.h>
#include <pthread.h>

//#define clock_gettime(a,b) timespec_get(b,TIME_UTC) 

#include <stdio.h>

extern FILE _iob[];

#define CLOCK_MONOTONIC 0
int clock_gettime(int id, struct timespec *spec);
