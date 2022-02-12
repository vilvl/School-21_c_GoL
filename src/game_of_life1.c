#include <stdio.h>
#include <stdlib.h>
#define FILED_W 80
#define FILED_H 25

void init(int** mas, int param);
void clear_mas(int** mas);
int scan_mas(int** mas);
void generate_field(int** mas, int param);

int main() {
    int **mas = NULL;

    init(int** mas, param)

    return 0;
}

int init (int** mas, int param) {
    clear_mas(mas);
    if (param == 0)
        return scan_mas(mas);
    else
        generate_field(mas);
    return 0;
}

void clear_mas(int** mas) {
    for(int i = 0; i < FILED_H; i++)
        for(int j = 0; j < FILED_W; j++)
            mas[i][j] = 0;
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
