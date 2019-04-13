/*
 * Tema 2 ASC
 * 2019 Spring
 * Catalin Olaru / Vlad Spoiala
 */
#include "utils.h"
#include "cblas.h"
#include <string.h>

/*
 * C = (zerotr(At * B + Bt * A))^2
 * 
 * A, B : N x N
 * zerotr - matrice in care partea de sub diagonala principala este 0
 */

double *init_aux(int N) {
	double *aux = (double *) malloc (N * N * sizeof(double));
	if (!aux) {
		printf("Malloc failed\n");
		return NULL;
	}

	return aux;
}

void transpose_matrix_inplace(int N, double *A) {
    int i, j;
    double aux;
    int line;
    
	for (i = 0; i < N - 2; i++) {
        line = i * N;
		for (j = i + 1; j < N - 1; j++) {
            aux = A[line + j];
            A[line + j] = A[j * N + i];
            A[j * N + i] = aux;
		}
	}
}

double *multiply_matrix(int N, double *A, double *B) {
	double *aux = init_aux(N);
	if (!aux) return NULL;

	cblas_dgemm(CblasRowMajor, CblasTrans, CblasNoTrans, N, N, N, 1, A, N, B, N, 1, aux, N);

	return aux;
}

double *power_matrix(int N, double *A) {
	double *aux = init_aux(N);
	cblas_dtrmm(CblasRowMajor, CblasLeft, CblasUpper, CblasNoTrans, CblasNonUnit, N, N, 1, A, N, A, N);
	
	memcpy(aux, A, N * N * sizeof(double));
	return aux;
}

double *add_matrix(int N, double *A, double *B) {
	int i, j;

	double *aux = init_aux(N);
	if (!aux) return NULL;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			aux[i * N + j] = A[i * N + j] + B[i * N + j];
		}
	}

	return aux;
}

void zerotr_matrix(int N, double *A) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (j < i) {
				A[i * N + j] = 0;
			}
		}
	}
}

/* 
 * Add your BLAS implementation here
 */
double *my_solver(int N, double *A, double *B) {
	printf("BLAS SOLVER\n");

	double *ATB = multiply_matrix(N, A, B);
	
	double *BTA = init_aux(N);
	memcpy(BTA, ATB, N * N * sizeof(double));
	transpose_matrix_inplace(N, BTA);

	double *ATBPBTA = add_matrix(N, ATB, BTA);

	zerotr_matrix(N, ATBPBTA);

	double *C = power_matrix(N, ATBPBTA);

	free(ATB);
	free(BTA);
	free(ATBPBTA);
	
	return C;
}
