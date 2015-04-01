#include <stdlib.h>
#include <stdio.h>
#include <time.h>	
/* Intel MKL library */
#include "mkl_lapacke.h"
#include "team4_readme.h"

/* Macros */
#define ROWS    20 //2^20
#define COLUMNS 20
#define N       20
#define LDA     N

int main()
{
	int i,j,return_eig;
	MKL_INT lda, n;
	double *A;
    double w[N];
	clock_t start, end;
	double exec_time;
	int rows, columns = 2;
	
	FILE *fp = fopen("output.csv", "w"); /* Output file */
	fprintf(fp, "Data size\tTime\n");
	fclose(fp);

	/* Initialization */
	lda = N;
	srand(0);
	
	/* Varying data sizes */
	for (rows = 2; rows < ROWS; rows++)
	{
		n = exp2(rows);
		A = (double*)calloc(n*n,sizeof(double));
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				A [(i*lda)+ j]	 = ((double)rand() / (double)RAND_MAX);
			}
		}		
		/* Execute eigenvalue calculation for 1000 times and record the time */
		start = clock();
		for (i = 0; i < 1000; i++) //1000 iterations
		{
			return_eig = LAPACKE_dsyev( LAPACK_ROW_MAJOR, 'V', 'U', n, A, lda, w );

		}
		free(A);
		end = clock();
		exec_time = (double)(end - start) / CLOCKS_PER_SEC;
		exec_time = exec_time/1000;                         /* divide by 1000 because of # of iterations */	
		FILE *fp2 = fopen("output.csv", "w"); /* Output file */
		fprintf(fp2, "%d\t%f,\n", rows, exec_time);
		fclose(fp2);
	}
}
