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

/**
@brief		A ceiling macro for negative numbers.
*/
#define CEILING_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))
/**
@brief		A ceiling macro for positive numbers.
*/
#define CEILING_NEG(X) ((X-(int)(X)) < 0 ? (int)(X-1) : (int)(X))
/**
@brief		The ceiling macro.
*/
#define CEILING(X) ( ((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X) )

#include "cbmark.h"
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <sys/resource.h>
#include <time.h>
#endif

int benchmark_getresolution(cbm_trial_t *resolution_p, int dotimes)
{
	if (dotimes < 1)
	{
		// TODO: Print out an error.
		return -1;
	}
	
	/* Create variables for running averages. */
	double wctime_sec = 0;
	double wctime_nsec = 0;
	double cputime_user_sec = 0;
	double cputime_user_usec = 0;
	double cputime_kernel_sec = 0;
	double cputime_kernel_usec = 0;
	
	cbm_trial_t temp;
	int i;
	for (i = 0; i < dotimes; ++i)
	{
		/* Do a quick benchmark. */
		startbenchmark(&temp);
		endbenchmark(&temp);
		
		/* Calculate the running average. */
		wctime_sec = ((wctime_sec * i) + (double)(temp.wctime_sec))
				/ (double)(i + 1);
		wctime_nsec = ((wctime_nsec * i) + (double)(temp.wctime_nsec))
				/ (double)(i + 1);
		cputime_user_sec = ((cputime_user_sec * i) +
					(double)(temp.cputime_user_sec))
					/ (double)(i + 1);
		cputime_user_usec = ((cputime_user_usec * i) +
					(double)(temp.cputime_user_usec))
					/ (double)(i + 1);
		cputime_kernel_sec = ((cputime_kernel_sec * i) +
					(double)(temp.cputime_kernel_sec))
					/ (double)(i + 1);
		cputime_kernel_usec = ((cputime_kernel_usec * i) +
					(double)(temp.cputime_kernel_usec))
					/ (double)(i + 1);
	}
	
	/* Set the resolution structure values. */
	resolution_p->wctime_sec = CEILING(wctime_sec);
	resolution_p->wctime_nsec = CEILING(wctime_nsec);
	resolution_p->cputime_user_sec = CEILING(cputime_user_sec);
	resolution_p->cputime_user_usec = CEILING(cputime_user_usec);
	resolution_p->cputime_kernel_sec = CEILING(cputime_kernel_sec);
	resolution_p->cputime_kernel_usec = CEILING(cputime_kernel_usec);
	
	return 0;
}

int startbenchmark(cbm_trial_t *p)
{
#ifdef WIN32
#else
	/* Time value structures. */
	struct timespec wctime;
	struct rusage cputime;
	
	/* Get the wallclock time. */
	clock_gettime(CLOCK_MONOTONIC, &wctime);
	
	/* Get CPU time. */
	getrusage(RUSAGE_SELF, &cputime);
	
	/* Set times. */
	p->wctime_sec = wctime.tv_sec;
	p->wctime_nsec = wctime.tv_nsec;
	p->cputime_user_sec = cputime.ru_utime.tv_sec;
	p->cputime_user_usec = cputime.ru_utime.tv_usec;
	p->cputime_kernel_sec = cputime.ru_stime.tv_sec;
	p->cputime_kernel_usec = cputime.ru_stime.tv_usec;
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
	
	p->cputime_user_sec = cputime.ru_utime.tv_sec - p->cputime_user_sec;
	p->cputime_user_usec = cputime.ru_utime.tv_usec - p->cputime_user_usec;
	
	p->cputime_kernel_sec = cputime.ru_stime.tv_sec -
					p->cputime_kernel_sec;
	p->cputime_kernel_usec = cputime.ru_stime.tv_usec -
					p->cputime_kernel_usec;
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
