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

double *init_aux(int N) {
	double *aux = (double *) malloc (N * N * sizeof(double));
	if (!aux) {
		printf("Malloc failed\n");
		return NULL;
	}

	return aux;
}

// void transpose_matrix_inplace(int N, double *A) {
//     int i, j;
//     double aux;
//     int line;
    
// 	for (i = 0; i < N - 2; i++) {
//         line = i * N;
// 		for (j = i + 1; j < N - 1; j++) {
//             aux = A[line + j];
//             A[line + j] = A[j * N + i];
//             A[j * N + i] = aux;
// 		}
// 	}
// }

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
	int i, j, k, ib, jb, kb;
	int BS = 40; // block size
	register double sum;
	register double *pa, *pb;
	double *aux = init_aux(N);
	if (!aux) return NULL;

	for (i = 0; i < N; i += BS) {
		for (j = 0; j < N; j += BS) {
			for (k = 0; k < N; k += BS) {
				for (ib = 0; ib < BS; ib++) {
					pa = &A[ib * N];
					for (kb = 0; kb < BS; kb++) {
						sum = 0;
						pb = &B[kb * N];
						for (jb = 0; jb < BS; jb += 10) {
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
							sum += *pa * *pb;
							pb++;
						}
						pa++;
						aux[ib * N + jb] = sum;
					}
				}
			}
		}
	}

	return aux;
}

double *power_matrix(int N, double *A) {
	int i, j, k, ib, jb, kb;
	int BS = 40; // block size
	register double sum;
	double *pa;
	double *aux = init_aux(N);
	if (!aux) return NULL;
	
	for (i = 1; i < N; i++) {
		pa = &aux[i * N];
		for (j = 0; j < i; j++) {
			*pa = 0;
			pa++;
		}
	}

	for (i = 0; i < N; i++) {
		sum = A[i * N + i];
		aux[i * N + i] = sum * sum;
	}

	for (i = 0; i < N; i += BS) {
		for (j = 0; j < N; j += BS) {
			for (k = 0; k < N; k += BS) {
				for (ib = 0; ib < BS; ib++) {
					for (jb = i + 1; jb < BS; jb++) {
						sum = 0;
						for (kb = 0; kb < j + 1; kb++) {
							sum += A[ib * N + kb] * A[kb * N + jb];
						}
						aux[ib * N + jb] = sum;
					}
				}
			}
		}
	}

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
			if (j > i) {
				A[i * N + j] = 0;
			}
		}
	}
}

/* 
 * Add your BLAS implementation here
 */
double *my_solver(int N, double *A, double *B) {
	printf("NEOPT SOLVER\n");

	double *AT = transpose_matrix(N, A);
	double *BT = transpose_matrix(N, B);

	double *ATB = multiply_matrix(N, AT, B);
	double *BTA = transpose_matrix(N, ATB);

	double *ATBPBTA = add_matrix(N, ATB, BTA);

	zerotr_matrix(N, ATBPBTA);

	double *C = power_matrix(N, ATBPBTA);

	free(AT);
	free(BT);
	free(ATB);
	free(BTA);
	free(ATBPBTA);
	
	return C;
}
