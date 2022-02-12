#include <stdio.h>
#include <stdlib.h>

#define FIELD_W
#define FIELD_H

#define CELL '*'
#define NO_CELL '.'

#define NEXT ' '
#define EXIT 'q'

#define DEFAULT_FILLNESS 30

int** input_array(int *n, int *m);
void output(int **matrix, int n, int m);
int** get_matrix(int rws, int cls);
void free_matrix(int **m);

int main() {

    int **cond = get_matrix(FIELD_W, FIELD_H);
    int **next_cond = get_matrix(FIELD_W, FIELD_H);
    if (!cond || !next_cond) {
        printf("n/a");
        return 1;
    }
    
    int iter = 0;
    int gameover = 0;

    // invite    
    // input 
    while (1) {
        draw(matr, iter);
        gameover = get_input();
        next_iter();
        iter++;
    }

    free_matrix(matr)
    return 0;
}

int** input_array(int* rws, int* cls) {
    for (int i = 0; i < *rws; ++i) {
        for (int j = 0; j < *cls; ++j) {
            if (scanf("%d", &m[i][j]) != 1) {
                free_matrix(m);
                return NULL;
            }
        }
    }
    return m;
}

// Array of pointers to single array
int** get_matrix(int rws, int cls) {
    int **matrix = malloc(sizeof(int*) * rws);
    if (!matrix)
        return NULL;
    int *arr = malloc(sizeof(int) * rws * cls);
    if (!arr) {
        free(matrix);
        return NULL;
    }
    for (int i = 0; i < rws; i++) {
        matrix[i] = arr + i * cls;
    }
    return matrix;
}

void free_matrix(int **m) {
    free(m[0]);
    free(m);
}