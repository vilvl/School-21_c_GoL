#include <stdio.>

#define FIELD_W 80
#define FIELD_H 25

int count_heighbor(int** *a, int i, int j);
int check_heighbor(int** *a, int i, int j);
int transfer_w(int i);
int transfer_h(int j);
void swap(int** *a, int** *b);
void logic(int** *a);

int main() {
    
}

int check_heighbor(int **field, int i, int j) {
    if (field[i][j] != 0) {
        i = (i + FIELD_H) % FIELD_H;
        j = (j + FIELD_W) % FIELD_W;
        return 1;
    } else {
        i = (i + FIELD_H) % FIELD_H;
        j = (j + FIELD_W) % FIELD_W;
        return 0;
    }
}

int count_neighbor(int **a, int i, int j) {

    int count = 0;

    if (check_heighbor(field, i - 1, j - 1)) count++;
    if (check_heighbor(field, i - 1, j)) count++;
    if (check_heighbor(field, i - 1, j + 1)) count++;
    if (check_heighbor(field, i, j - 1)) count++;
    if (check_heighbor(field, i, j + 1)) count++;
    if (check_heighbor(field, i + 1, j - 1)) count++;
    if (check_heighbor(field, i + 1, j)) count++;
    if (check_heighbor(field, i + 1, j + 1)) count++;
  
  return count;
}

void logic(int** *m_cur, int** *m_nxt) {
    int field = *m_cur;
    int field_num2 = *m_nxt;
    int i, j, nbr;
    for (i = 0; i < FIELD_W; i++) {
        for (j = 0; j < FIELD_H; j++) {
            nbr = count_heighbor(field, i, j);
            if (nbr == 2)
                field_num2[i][j] = field[i][j];
            if (nbr == 3)
                field_num2[i][j] =  1;
            if (nbr < 2 || nbr > 3)
                field_num2[i][j] = 0;
        }
    }
    swap(m_cur, m_nxt);
}

void swap(int** *a, int** *b) {
    int s = *a;
    *a = *b;
    *b = s;
}

/*
int transfer_w(int i) {
    while (i == 0) i = i + FIELD_W;
    while (i >= FIELD_W) i = i - FIELD_W;
    return i;
}

int transfer_h(
*/
