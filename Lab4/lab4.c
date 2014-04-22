#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define OVERHEAD 0.153

typedef struct _task {
	double	runtime, period;
	double	semaphore[2];
	double	max_blocking;
} TASK;

void
printErr ( char *, int );

void
RMA	( TASK [], int, bool );

int
main ( int argc, char *argv[] )
{
	//////////////////////////
	/// SET DEFAULT VALUES ///
	//////////////////////////

	bool VERBOSE = false;

	//////////////////////////
	/// VERIFY/PARSE INPUT ///
	//////////////////////////

	int i;

	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-v") == 0)
			VERBOSE = true;
		else
			printErr(argv[i], EINVAL);
	}

	/* /////////////|\\\\\\\\\\\\\ */
	/* |||      DO STUFFS      ||| */
	/* \\\\\\\\\\\\\|///////////// */


	// Build a task set
	TASK task[6];

	task[0].runtime      = 1.3;
	task[0].period       = 2.56;
	task[0].semaphore[0] = 0.2;
	task[0].semaphore[0] = 0.0;
	task[0].max_blocking = 0.3;

	task[1].runtime      = 4.7;
	task[1].period       = 40.96;
	task[1].semaphore[0] = 0.2;
	task[1].semaphore[0] = 0.0;
	task[1].max_blocking = 0.3;

	task[2].runtime      = 9.0;
	task[2].period       = 61.44;
	task[2].semaphore[0] = 0.15;
	task[2].semaphore[0] = 3.00;
	task[2].max_blocking = 6.00;

	task[3].runtime      = 23.0;
	task[3].period       = 1000.0;
	task[3].semaphore[0] = 0.30;
	task[3].semaphore[0] = 0.0;
	task[3].max_blocking = 6.00;

	task[4].runtime      = 38.3;
	task[4].period       = 1024.0;
	task[4].semaphore[0] = 0.30;
	task[4].semaphore[0] = 6.00;
	task[4].max_blocking = 0.2;

	task[5].runtime      = 3.0;
	task[5].period       = 1280.0;
	task[5].semaphore[0] = 0.2;
	task[5].semaphore[0] = 0.0;
	task[5].max_blocking = 0.0;

	RMA( task, 6, VERBOSE );

	exit(0);
}

//function definitions

void
RMA( TASK task[], int count, bool VERBOSE )
{
	int i, j, k, l;
	int storedk, storedl;

	double LHS, RHS, inner;

	bool SCHEDULABLE;

	if ( VERBOSE ) printf( "Testing for possible scheduling...\n\n" );

	for ( i = 1; i <= count; i++ )
	// i iterates from 1 to n
	{
		SCHEDULABLE = false;
		for ( k = 1; k <= i; k++ )
		// k iterates from 1 to i
		{
			for ( l = 1; l <= (int) floor( task[i-1].period / task[k-1].period ); l++ )
			// k iterates from 1 to floor of Ti / Tk
			{
				// setup RHS and LHS for maths
				LHS = 0.0; // clear for loops
				RHS = (l) * task[k-1].period; // l * Tk

				// if ( VERBOSE ) printf( "\t\tLHS = " );

				// peform summation loop
				for ( j = 1; j < i; j++ )
				// sum from j=1 to i-1
				{
					inner = ( task[j-1].runtime + OVERHEAD );
					inner *= (int) ceil( l * task[k-1].period / task[j-1].period );
					LHS += inner;
				}

				// add in runtimes and blocking
				LHS = LHS + ( task[i-1].runtime + OVERHEAD + task[i-1].max_blocking );

				if ( LHS <= RHS ) {
					// set conditions to break two inner loops and continue i
					storedl = l;
					storedk = k;
					l = (int) floor( task[i-1].period / task[k-1].period ) + 1;
					k = i + 1;
					if ( VERBOSE ) printf( "Pass for i: %d\n", i );
					SCHEDULABLE = true;
				} else {
					if ( VERBOSE ) printf( "\tFail with i: %d\tk: %d\tl: %d\n", i, k, l );
				}

			} // end l
		} // end k
		if ( !SCHEDULABLE )
		{
			if ( VERBOSE ) printf( "\n" );
			printf ( "System is unschedulable at i: %d.\n", i );
			if ( VERBOSE ) printf( "\n" );
			exit(1);
		}
	} // end i

	if ( SCHEDULABLE )
	{
		if ( VERBOSE ) printf( "\n" );
		printf( "System is schedulable with k: %d and l: %d.\n", storedk, storedl );
		if ( VERBOSE ) printf( "\n" );
	}
}

void
printErr(char *str, int err)
{
	errno      = err;
	char *msg  = (char *)"Error:\n    ";
	char *pstr = NULL;
	pstr       = (char *)malloc(strlen(str) + strlen(msg));

	strcat(pstr, msg);
	strcat(pstr, str);
	perror(pstr);

	exit(err);
}