#include <stdlib.h>
#include <stdio.h>
#include <time.h>	
/* Intel MKL library */
#include "mkl_lapacke.h"
#include "team4_readme.h"

/* Macros */
#define ROWS    1048576 //2^20
#define COLUMNS 1048576
#define N       1048576

int main()
{
	int i,j;
	MKL_INT lda, n;
	double A[ROWS][COLUMNS]={0};     /* A is an nxn matrix */
    double w[N];
	clock_t start, end;
	double exec_time;
	int rows, columns = 2;
	
	FILE *fp = fopen("output.csv", "w"); /* Output file */
	fprintf(fp, "Data size\tTime\n");
	fclose(fp);

	/* Initialization */
	lda = N;
	n = N;
	srand(0);
	
	/* Varying data sizes */
	for (rows = 2; rows < ROWS; rows++)
	{
		columns++;
		for (i = 0; i < rows; i++)
		{
			for (j = 0; j < columns; j++)
			{
					A[i][j] = ((double)rand() / (double)RAND_MAX);
			}
		}
		
		/* Execute eigenvalue calculation for 1000 times and record the time */
		start = clock();
		for (i = 0; i < 1000; i++) //1000 iterations
		{
			if(LAPACKE_dsyev( LAPACK_ROW_MAJOR, 'V', 'U', n, A, lda, w ))
			{
				FILE *fp = fopen("output.csv", "w"); /* Output file */
				printf("Failed to calculate eigenvalues and eigenvectors\n");
				fclose(fp);
				exit(1);
			}
		}
		end = clock();
		exec_time = (double)(end - start) / CLOCKS_PER_SEC;
		exec_time = exec_time/1000;                         /* divide by 1000 because of # of iterations */	
		FILE *fp = fopen("output.csv", "w"); /* Output file */
		fprintf(fp, "%d\t%f,\n", rows, exec_time);
		fclose(fp);
	}
}
