/******************************************************************************/
/**
@file		cbmark.c
@author		Graeme Douglas
@brief		A simple cross-platform(ish) benchmarking utility: the
		implementation.
@details	DO NOT FORGET TO ADD -lrt to end of gcc compilation arguments.
		
		For more information, see cbmark.h.
@todo		-Perform checks on all functions that return error codes.
*/
/******************************************************************************/

#include "cbmark.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#endif

int startbenchmark(cbm_trial_t *p)
{
#ifdef WIN32
#else
	/* Time value structures. */
	struct timespec wctime;
	
	/* Get the wallclock time. */
	clock_gettime(CLOCK_MONOTONIC, &wctime);
	
	/* Set times. */
	p->wctime_sec = wctime.tv_sec;
	p->wctime_nsec = wctime.tv_nsec;
	
#endif
	return 0;
}

int endbenchmark(cbm_trial_t *p)
{
#ifdef WIN32
#else
	/* Time value structures. */
	struct timespec wctime;
	struct rusage cputime;
	
	/* Get wallclock time. */
	clock_gettime(CLOCK_MONOTONIC, &wctime);
	
	/* Get CPU time. */
	getrusage(RUSAGE_SELF, &cputime);
	
	/* Calculate times. */
	p->wctime_sec = wctime.tv_sec - p->wctime_sec;
	p->wctime_nsec = wctime.tv_nsec - p->wctime_nsec;
	
	p->cputime_user_sec = cputime.ru_utime.tv_sec;
	p->cputime_user_usec = cputime.ru_utime.tv_usec;
	
	p->cputime_kernel_sec = cputime.ru_stime.tv_sec;
	p->cputime_kernel_usec = cputime.ru_stime.tv_usec;
#endif
	return 0;
}

void cbm_printresult(cbm_trial_t *p)
{
	printf("Wallclock time: %ld seconds, %ld nanoseconds\n", p->wctime_sec,
		p->wctime_nsec);
	printf("User CPU-time: %ld seconds, %ld microseconds\n",
		p->cputime_user_sec, p->cputime_user_usec);
	printf("Kernel CPU-time: %ld seconds, %ld microseconds\n",
		p->cputime_kernel_sec, p->cputime_kernel_usec);
	fflush(stdout);
}
