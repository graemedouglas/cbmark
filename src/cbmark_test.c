/**
@file		cbmark_test.c
@author		Graeme Douglas
@brief		Quick unit test for the c benchmark utility.
*/

#include <stdio.h>
#include "cbmark.h"

int test1(void)
{
	printf("Timing sum of first 100 numbers. \n");
	fflush(stdout);
	//-- General variable declaration.
	// The trial control variable.
	cbm_trial_t trial;
	
	// Loop variables.
	int i;
	
	// Sum variable.
	int sum = 0;
	
	// Time the length of time it takes to loop 100 times.
	startbenchmark(&trial);
	for (i = 0; i < 100; ++i)
	{
		sum += i;
	}
	endbenchmark(&trial);
	
	// Print the result.
	cbm_printresult(&trial);
	return 0;
}

int test2(void)
{
	printf("Timing sum of first 100000 numbers. \n");
	fflush(stdout);
	//-- General variable declaration.
	// The trial control variable.
	cbm_trial_t trial;
	
	// Loop variables.
	int i;
	
	// Sum variable.
	int sum = 0;
	
	// Time the length of time it takes to loop 100 times.
	startbenchmark(&trial);
	for (i = 0; i < 100000; ++i)
	{
		sum += i;
	}
	endbenchmark(&trial);
	
	// Print the result.
	cbm_printresult(&trial);
	return 0;
}

int test3(void)
{
	printf("Timing sum of first 1000000 numbers. \n");
	fflush(stdout);
	//-- General variable declaration.
	// The trial control variable.
	cbm_trial_t trial;
	
	// Loop variables.
	int i;
	
	// Sum variable.
	int sum = 0;
	
	// Time the length of time it takes to loop 100 times.
	startbenchmark(&trial);
	for (i = 0; i < 1000000; ++i)
	{
		sum += i;
	}
	endbenchmark(&trial);
	
	// Print the result.
	cbm_printresult(&trial);
	return 0;
}

int test4()
{
	printf("Getting system resolution.\n");
	fflush(stdout);
	//-- General variable declaration.
	// Resolution trial variable.
	cbm_trial_t resolution_trial;
	
	// Loop variables.
	int i;
	
	benchmark_getresolution(&resolution_trial, 1);
	cbm_printresult(&resolution_trial);
	return 0;
}

// Main Function.
int main(void)
{
	test1();
	test2();
	test3();
	test4();
	return 0;
}
