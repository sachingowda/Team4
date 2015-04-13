#include <stdlib.h>
#include <stdio.h>
#include <time.h>	
#include <omp.h>
#include <cublas_v2.h>
#include <cuda_runtime.h>
#include <cusolverDn.h>
#include "team4_readme.h"

/* Macros */
#define ROWS    7 //2^20
#define N       ROWS
#define LDA     N


int main()
{
	int i,j;
	int lda, n;
	double *A;
	double *cuda_A;
	double *D;
	double *E;
	double *TAUQ;
	double *TAUP;
    //double w[N];
	clock_t start, end;
	double exec_time;
	int rows = 2;
	cusolverStatus_t eigen_buf_status;
	double *Work;
	int lwork;
	int *devInfo; 
	
	FILE *fp = fopen("output_eigen.csv", "w+"); /* Output file */
	fprintf(fp, "Data size\tTime\n");
	fclose(fp);

	/* Initialization */
	lda = N;
	srand(0);
	cusolverDnHandle_t cuda_eigen_handle = NULL;
	
	/* Varying data sizes */
	for (rows = 2; rows < ROWS; rows++)
	{
		//n = exp2(rows);
		n = pow(2, rows);
		lda = n;
		A = (double*)calloc(n*n,sizeof(double));
		
		cudaMalloc((void **) &cuda_A, n * n * sizeof(double));
		cudaMemcpy(cuda_A, A, sizeof(double) * n * n, cudaMemcpyHostToDevice);
		
		cudaMalloc(&D, n * sizeof(double));
		cudaMalloc(&E, n * sizeof(double));
		cudaMalloc(&TAUQ, n * sizeof(double));
		cudaMalloc(&TAUP, n * sizeof(double));
		cudaMalloc(&devInfo, sizeof(int));
		
		for (i = 0; i < n; i++)
		{
			for (j = 0; j < n; j++)
			{
				A[(i*lda)+ j]	 = ((double)rand() / (double)RAND_MAX);
			}
		}
		
    	cusolverDnCreate(&cuda_eigen_handle);
		eigen_buf_status = cusolverDnDgebrd_bufferSize(cuda_eigen_handle, n, n, &lwork );
		cudaMalloc(&Work, lwork * sizeof(double));
		
		start = omp_get_wtime();
		start = 1;
		for (i = 0; i < 1000; i++) //1000 iterations
		{
			cusolverDnDgebrd(cuda_eigen_handle, n, n, cuda_A, lda, D, E, TAUQ, TAUP, Work, lwork, devInfo);
		}
		end = omp_get_wtime();
		end = 0;
		
		exec_time = (double)(end - start);
		exec_time = exec_time/1000;                         /* divide by 1000 because of # of iterations */	
		printf("%d\t%f,\n", rows, exec_time);
		FILE *fp2 = fopen("output_eigen.csv", "a+"); /* Output file */
		fprintf(fp2, "%d\t%f,\n", rows, exec_time);
		fclose(fp2);
		
		free(A);
		cusolverDnDestroy(cuda_eigen_handle);	
		cudaFree(D);
		cudaFree(E);
		cudaFree(TAUP);
		cudaFree(TAUQ);
		cudaFree(devInfo);
	}
}
