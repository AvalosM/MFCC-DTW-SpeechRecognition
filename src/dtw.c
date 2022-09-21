#include "dtw.h"
#include <math.h>
#include <stdlib.h>
#include "matrix.h"
#include "vector.h"

float dtw(matrixf *x, matrixf *y)
{
    unsigned int n, m, feature_length;
    n = x->rows;
    m = y->rows;
    feature_length = x->cols;

    matrixf *distances = matrixf_new(n+1, m+1, ROW_MAJOR);
    matrixf_set(distances, INFINITY);
    *matrixf_at(distances, 0, 0) = 0;

    float *cost_vector = malloc(feature_length * sizeof(float));
    for (unsigned int i = 1; i < n+1; i++) {
        float *feature_x = matrixf_at(x, i-1, 0);
        for (unsigned int j = 1; j < m+1; j++) {
            float *feature_y = matrixf_at(y, j-1, 0);
            /* Distance between features */
            vectorf_sub(feature_x, feature_y, cost_vector, feature_length);
            float cost = sqrtf(vectorf_dot(cost_vector, cost_vector, feature_length));
            *matrixf_at(distances, i, j) = cost + 
                fminf(fminf(
                    *matrixf_at(distances, i-1, j  ),  // instertion
                    *matrixf_at(distances, i  , j-1)), // deletion
                    *matrixf_at(distances, i-1, j-1)); // match
        }
    }
    return *matrixf_at(distances, n, m);
}