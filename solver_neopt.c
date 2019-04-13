/*
 * Tema 2 ASC
 * 2019 Spring
 * Catalin Olaru / Vlad Spoiala
 */
#include "utils.h"

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

/*
 * 1 1 1		1 2 3
 * 2 2 2  --->	1 2 3
 * 3 3 3		1 2 3
*/
double *transpose_matrix(int N, double *A) {
	int i, j;

	double *aux = init_aux(N);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			aux[i * N + j] = A[j * N + i]; 
		}
	}

	return aux;
}

double *multiply_matrix(int N, double *A, double *B) {
	int i, j, k;
	double sum = 0;

	double *aux = init_aux(N);
	if (!aux) return NULL;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			sum = 0;
			for (k = 0; k < N; k++) {
				sum += A[i * N + k] * B[k * N + j];
			}
			aux[i * N + j] = sum;
		}
	}

	return aux;
}

double *power_matrix(int N, double *A) {
	return multiply_matrix(N, A, A);
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

void print_matrix(int N, double *A) {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			printf("%lf ", A[i * N + j]);
		}
		printf("\n");
	}
}

/*
 * Add your unoptimized implementation here
 */
double *my_solver(int N, double *A, double* B) {
	printf("NEOPT SOLVER\n");

	double *AT = transpose_matrix(N, A);
	double *BT = transpose_matrix(N, B);

	double *ATB = multiply_matrix(N, AT, B);
	double *BTA = multiply_matrix(N, BT, A);

	double *ATBPBTA = add_matrix(N, ATB, BTA);

	zerotr_matrix(N, ATBPBTA);

	double *C = power_matrix(N, ATBPBTA);

	free(AT);
	free(BT);
	free(ATB);
	free(BTA);
	free(ATBPBTA);
	
	print_matrix(N, C);
	
	return C;
}
