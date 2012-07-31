/******************************************************************************/
/**
@file		cbmark.h
@author		Graeme Douglas
@brief		A simple cross-platform(ish) benchmarking utility: the
		interface.
@details	A benchmarking tool born out of necessity for some of my
		database research.  I wanted to make a tool that was simple to
		use on many platforms, and allowed for some basic statistical
		calculations over multiple runs.  I will use a linked-list
		for such performance calculations.
*/
/******************************************************************************/

#include <stdio.h>

/* Control structure type containing benchmarking data for a single trial. */
/**
@struct		cbm_trial_t
@brief		Structure containing benchmarking information for a single
		trial.
*/
typedef struct cbm_trial
{
	/*@{*/
	long wctime_sec;		/**< The seconds portion of the
					     wallclock time for this benchmark
					     trial. */
	long wctime_nsec;		/**< The nanoseconds portion of the
					     wallclock time for this benchmark
					     trial. */
	long cputime_user_sec;		/**< The number of seconds the CPU spent
					     in user mode for this trial. */
	long cputime_user_usec;		/**< The number of microseconds the CPU
					     spent in user mode for this trial.
					*/
	long cputime_kernel_sec;	/**< The number of seconds the CPU spent
					     in kernel mode for this trial. */
	long cputime_kernel_usec;	/**< The number of microseconds the CPU
					     spent in kernel mode for this
					     trial. */
	/*@}*/
} cbm_trial_t;

/* Start the benchmark for a given trial. */
/**
@brief		Begins the benchmark for a given trial.
@details	Note that all times stored in the structure pointed by @p p
		are non-sensical until a matching endbenchmark(p) is called.
@param		p	Pointer to the benchmark control structure that
			is to track all the data for this trial.
@returns	@c 0 on success, @c -1 otherwise.
*/
int startbenchmark(cbm_trial_t *p);

/* End the benchmark for a given trial. */
/**
@brief		Ends the benchmark for a given trial.
@details	Given that a matching startbenchmark() is called, this function
		will correctly set all the time values in the control structure
		represented by p.
@param		p	Pointer to the benchmark control structure that
			is to track all the data for this trial.
@returns	@c 0 on success, @c -1 otherwise.
*/
int endbenchmark(cbm_trial_t *p);

/* Start the benchmark for a given trial. */
/**
@brief		Prints out the result of a given trial.
@param		p	Pointer to the benchmark control structure that
			contains the time information to be printed.
*/
void cbm_printresult(cbm_trial_t *p);