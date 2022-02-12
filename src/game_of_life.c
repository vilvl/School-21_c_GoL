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

void draw(int** field, int iter) {
    clear_screen(20);
    for (int y = 0; y <= FIELD_HIGHT - 1; y++) {
        for (int x = 0; x <= FIELD_WIDTH - 1; x++) {
            // score
            if ( (y == 0) && (x == FIELD_WIDTH / 2 - 3 ) ) {
                    printf(" %02d||%02d ", score1, score2);
                    x += 7;
            // borders
            } else if  ((y == 0 || y == FIELD_HIGHT - 1)
                    && (x == 0 || x == FIELD_WIDTH - 1)) {
                printf("#");
            } else if (y == 0 || y == FIELD_HIGHT - 1) {
                printf("-");
            } else if (x == 0 || x == FIELD_WIDTH - 1) {
                printf("|");
            // graz
            } else if (graz && y == 7 && x == FIELD_WIDTH/2 - 14) {
                printf("CONGRATULATIONS PLAYER %d WIN", graz);
                x += 27;
            // rackets
            } else if (check_racket(x, y, RACKET_SHIFT, racket1)) {
                printf("]");
            } else if (check_racket(x, y, FIELD_WIDTH - 1 - RACKET_SHIFT, racket2)) {
                printf("[");
            // ball
            } else if (x == ball_x && y == ball_y) {
                printf("*");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void clear_screen(int counter) {
    while (counter > 0) {
        --counter;
        printf("\n");
    }
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