/*
 * Tema 2 ASC
 * 2019 Spring
 * Catalin Olaru / Vlad Spoiala
 */
#include "utils.h"
#include <string.h>

/*
 * C = (zerotr(At * B + Bt * A))^2
 * 
 * A, B : N x N
 * zerotr - matrice in care partea de sub diagonala principala este 0
 */

#define compute_value(paux, pa, pb)  \
{									 \
	*paux += *pa * *pb; 			 \
 	pb++;							 \
 	paux++;							 \
}

#define compute_ten_values(paux, pa, pb) 				 \
{  							 \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
compute_value(paux, pa, pb); \
}

#define compute_4ten_values(paux, pa, pb) \
{			  						 \
	compute_ten_values(paux, pa, pb); \
	compute_ten_values(paux, pa, pb); \
	compute_ten_values(paux, pa, pb); \
	compute_ten_values(paux, pa, pb); \
}

double *init_aux(int N) {
	double *aux = (double *) malloc (N * N * sizeof(double));
	if (!aux) {
		printf("Malloc failed\n");
		return NULL;
	}

	return aux;
}

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
	int i, k, ib;

	// Block size
	int BS = 40;

	// Most used counters should be kept in registers
	register int j, kb;

	// To avoid computing j + BS for every comparison
	register int kpbs;
	int ipbs;

	// Pointers to matrix values are often used.
	register double *pa, *pb, *paux;
	register double *oldpaux;

	double *aux = init_aux(N);
	if (!aux) return NULL;

	for (i = 0; i < N; i += BS) {
		ipbs = i + BS;
		for (j = 0; j < N; j += BS) {
			for (k = 0; k < N; k += BS) {
				kpbs = k + BS;
				for (ib = i; ib < ipbs; ib++) {
					pa = &A[ib * N + k];
					oldpaux = &aux[ib * N + j];
					for (kb = k; kb < kpbs; kb++) {
						pb = &B[kb * N + j];
						paux = oldpaux;
						
						// Unroll the loop for extra thicc power
						compute_4ten_values(paux, pa, pb);

						pa++;
					}
				}
			}
		}
	}

	return aux;
}

double *power_matrix(int N, double *A) {
	int i, k;
	
	// Most used counter should be in register
	register int j;

	// Pointers to matrix values are often
	register double *oldpaux;
	register double *pa, *pb, *paux;
	register double *aux = init_aux(N);
	if (!aux) return NULL;
	
	for (i = 1; i < N; i++) {
		paux = &aux[i * N];
		for (j = 0; j < i; j++) {
			*paux = 0;
			paux++;
		}
	}

	for (i = 0; i < N; i++) {
		pa = &A[i * N];
		oldpaux = &aux[i * N + i];
		for (k = 0; k < j + 1; k++) {
			pb = &A[k * N + i];
			paux = oldpaux;
			for (j = i; j < N; j++) {
				*paux += *pa * *pb;
				pb++;
				paux++;
			}
			pa++;
		}
	}

	return aux;
}

double *add_matrix(int N, double *A, double *B) {
	register int i, j;

	register double *aux = init_aux(N);
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
	printf("OPT SOLVER\n");

	double *AT = transpose_matrix(N, A);

	double *ATB = multiply_matrix(N, AT, B);
	double *BTA = transpose_matrix(N, ATB);

	double *ATBPBTA = add_matrix(N, ATB, BTA);

	zerotr_matrix(N, ATBPBTA);

	double *C = power_matrix(N, ATBPBTA);

	free(AT);
	free(ATB);
	free(BTA);
	free(ATBPBTA);

	return C;
}
