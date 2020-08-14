//! @file
//!  Performance test 
//!
//! @author
//!     Elias El Yandouzi - 2020

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "magic_exec.h"

#define CONV_REF magicfilter1d_naive_o1_
#define CONV magicfilter1d_sse_ 


#define SIZE 65536
#define X 31
#define Y 32
#define Z 33
#define TOTAL (2*X)*(2*Y)*(2*Z)


double* init_vector(unsigned int n){
    double* res = malloc(n*sizeof(double));

    double random;

    for(int i = 0; i < n; i++){
        random = (double)(rand() % 100) / (double)100;
        res[i] = random;
        #ifdef DEBUG
            printf("Random value in init_vector (main.c:21) : %lu", random);
        #endif
    }

    return res;
}


void check(double *arr_in, double *arr_out, size_t dim) {
	for (size_t i = 0; i < dim; i++) {
		assert(fabs(arr_in[i]-arr_out[i]) < 1e-14);
	}
}

int main(int argc, char** argv){
    
    time_t t;
    srand((unsigned) time(&t));
    
    unsigned int n = SIZE, ndat = 1;
    double* source = init_vector(n);
    double* dest = calloc(SIZE, sizeof(double));
    double* dest2 = calloc(SIZE, sizeof(double));

    printf("\n******* - MagicFilter sans transposition NAIVE - *******\n\n");
    magicfilter1d_naive_(&n, &ndat, source, dest);

    
    printf("\n******* - MagicFilter sans transposition NAIVE + Boucle déroulée - *******\n\n");
    magicfilter1d_naive_bis_(&n, &ndat, source, dest2);

    check(dest, dest2, SIZE);

	double * data_in = malloc(sizeof(double) * TOTAL);
	double * data_tmp = calloc(sizeof(double), TOTAL);
	double * data_out = calloc(sizeof(double), TOTAL);
	double * data_out2 = calloc(sizeof(double), TOTAL);

    init_rand(data_in, TOTAL);

	n = 2*X;
	ndat = 2*Y*2*Z;
	CONV(&n, &ndat, data_in, data_out);
	n = 2*Y;
	ndat = 2*Z*2*X;
	CONV(&n, &ndat, data_out, data_tmp);
	n = 2*Z;
	ndat = 2*X*2*Y;
	CONV(&n, &ndat, data_tmp, data_out);

	n = 2*X;
	ndat = 2*Y*2*Z;
	CONV_REF(&n, &ndat, data_in, data_out2);
	n = 2*Y;
	ndat = 2*Z*2*X;
	CONV_REF(&n, &ndat, data_out2, data_tmp);
	n = 2*Z;
	ndat = 2*X*2*Y;
	CONV_REF(&n, &ndat, data_tmp, data_out2);

	check(data_out, data_out2, TOTAL);
	return 0;

    free(dest);

    return 0;
}