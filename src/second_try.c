#include <stdio.>

#define FIELD_W 80
#define FIELD_H 25

int count_neighbor(int** *a, int i, int j);
int check_neighbor(int** *a, int i, int j);
void swap(int** *a, int** *b);
void logic(int** *a);

int main() {
    
}

int check_neighbor(int **field, int i, int j) {
    i = (i + FIELD_H) % FIELD_H;
    j = (j + FIELD_W) % FIELD_W;
    return field[i][j];
}

int count_neighbor(int **a, int i, int j) {

    int count = 0;

    count += check_neighbor(field, i - 1, j - 1);
    count += check_neighbor(field, i - 1, j);
    count += check_neighbor(field, i - 1, j + 1));
    count += check_neighbor(field, i, j - 1));
    count += check_neighbor(field, i, j + 1));
    count += check_neighbor(field, i + 1, j - 1));
    count += check_neighbor(field, i + 1, j));
    count += check_neighbor(field, i + 1, j + 1));
  
  return count;
}

void logic(int** *m_cur, int** *m_nxt) {
    int field = *m_cur;
    int field_num2 = *m_nxt;
    int i, j, nbr;
    for (i = 0; i < FIELD_W; i++) {
        for (j = 0; j < FIELD_H; j++) {
            nbr = count_neighbor(field, i, j);
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
