#include <stdio.h>
#include <stdlib.h>
#define FILED_W 80
#define FILED_H 25
int scan_mas(int **mas);

int** get_matrix(int rws, int cls);
void free_matrix(int **m);
void clear_mas(int** mas) {
    for(int i = 0; i < FILED_H; i++)
        for(int j = 0; j < FILED_W; j++)
            mas[i][j] = 0;
}

int main() {
    
    int **mas = get_matrix(FILED_H, FILED_W);
     clear_mas(mas);
    // for(int i = 0; i < FILED_H; i++) {
    //     for(int j = 0; j < FILED_W; j++){
    //         mas[i][j] = 0; 
    //         mas[10][j] = 5;
    //         printf("%d", mas[i][j]);
    //     }
    //     printf("\n");
    // }
    int e = scan_mas(mas);
    printf("%d %d %p %p\n\n", e, mas[1][2], &mas[1][2], &mas[0][27]);
    for(int i = 0; i < FILED_H; i++) {
        for(int j = 0; j < FILED_W; j++) {
            printf("%d", mas[i][j]);
        }
        printf("\n");
    }
    return 0;
}

int scan_mas(int** mas) {
    int i = 0, j = 0; 
    while(1) {
        int ch = getchar();
        if (ch == '1') {
            if (j < FILED_W && i < FILED_H) {
                mas[i][j] = 1;
                j++;
            } else {
                return 1;
            }
        } else if ( ch == '0') {
            if (j < FILED_W && i < FILED_H) {
                mas[i][j] = 0;
                j++;
            } else {
                return 1;
            }
        } else if (ch == EOF) {
            return 0;
        } else if (ch == '\n') {
            i++; 
            j = 0;
        } else {
            return 1;
        }
    }
    return 0;
}

// Array of pointers to single array
int** get_matrix(int rws, int cls) {
    int **matrix = malloc(sizeof(int*) * rws);
    if (!matrix)
        return NULL;
    matrix[0] = malloc(sizeof(int) * rws * cls);
    if (!matrix[0]) {
        free(matrix);
        return NULL;
    }
    for (int i = 0; i < rws; i++) {
        matrix[i] = matrix[0] + i * cls;
    }
    return matrix;
}

void free_matrix(int **m) {
    free(m[0]);
    free(m);
}
