eig: eigenvalue.c
	icc -mmic -mkl eigenvalue_short.c -o eigen1 -lm