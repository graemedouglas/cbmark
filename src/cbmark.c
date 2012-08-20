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

/* Utility method that makes calculates the next highest order of magnitude,
   where
	0 is order of magnitude 0.
	1 is order of magnitude 10.
	7 is order of magnitude 10.
	10 is order of magnitude 100.
	11 is order of magnitude 100.
	... etc ...
*/
long orderofmagnitude(long l)
{
	if (l < 0)
	{
		return -1;
	}
	else if (l == 0)
	{
		return 0;
	}
	long temp = l;
	
	int i;
	for (i = 0; temp > 0; ++i)
	{
		temp /= 10;
	}
	
	temp = 1;
	for (; i > 0; --i)
	{
		temp *= 10;
	}
	return temp;
}

int benchmark_getresolution(cbm_trial_t *resolution_p, int iterations)
{
	if (iterations < 1)
	{
		// TODO: Print out an error.
		return -1;
	}
	
	/* Initialize the resolution structure values. */
	resolution_p->wctime_sec = 0;
	resolution_p->wctime_nsec = 0;
	resolution_p->cputime_user_sec = 0;
	resolution_p->cputime_user_usec = 0;
	resolution_p->cputime_kernel_sec = 0;
	resolution_p->cputime_kernel_usec = 0;
	
	cbm_trial_t temp;
	int i;
	for (i = 0; i < iterations; ++i)
	{
		/* Do a quick benchmark. */
		startbenchmark(&temp);
		endbenchmark(&temp);
		
		/* Calculate the running maximums. */
		// TODO: maybe abstract this logic into a macro?
		if (temp.wctime_sec > resolution_p->wctime_sec)
		{
			resolution_p->wctime_sec = temp.wctime_sec;
		}
		if (temp.wctime_nsec > resolution_p->wctime_nsec)
		{
			resolution_p->wctime_nsec = temp.wctime_nsec;
		}
		if (temp.cputime_user_sec > resolution_p->cputime_user_sec)
		{
			resolution_p->cputime_user_sec = temp.cputime_user_sec;
		}
		if (temp.cputime_user_usec > resolution_p->cputime_user_usec)
		{
			resolution_p->cputime_user_usec =
						temp.cputime_user_usec;
		}
		if (temp.cputime_kernel_sec > resolution_p->cputime_kernel_sec)
		{
			resolution_p->cputime_kernel_sec =
						temp.cputime_kernel_sec;
		}
		if (temp.cputime_kernel_usec >
				resolution_p->cputime_kernel_usec)
		{
			resolution_p->cputime_kernel_usec =
						temp.cputime_kernel_usec;
		}
	}
	
	/* Calculate the next highest order of magnitude for each resolution. */
	resolution_p->wctime_sec = orderofmagnitude(resolution_p->wctime_sec);
	resolution_p->wctime_nsec = orderofmagnitude(resolution_p->wctime_nsec);
	resolution_p->cputime_user_sec =
		orderofmagnitude(resolution_p->cputime_user_sec);
	resolution_p->cputime_user_usec =
		orderofmagnitude(resolution_p->cputime_user_usec);
	resolution_p->cputime_kernel_sec =
		orderofmagnitude(resolution_p->cputime_kernel_sec);
	resolution_p->cputime_kernel_usec =
		orderofmagnitude(resolution_p->cputime_kernel_usec);
	
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
