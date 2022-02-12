#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIELD_W 80
#define FIELD_H 25

#define CELL '*'
#define NO_CELL ' '

#define KEY_NEXT ' '
#define KEY_EXIT 'q'

#define DEFAULT_FILLNESS 30

int** input_array(int *n, int *m);
void output(int **matrix, int n, int m);
int** get_matrix(int rws, int cls);
void free_matrix(int **m);

int count_heighbor(int **field, int i, int j);
int check_heighbor(int **field, int i, int j);
void swap_matrix(int** *a, int** *b);
void logic(int** *m_cur, int** *m_nxt);

int init(int** mas, int param, FILE *file);
void clear_mas(int** mas);
int scan_mas(int** mas, FILE *file);
void generate_field(int** mas, int param);

int ret_end(int code, int **m1, int **m2);

void draw(int** field, int iter);
int get_input();
void clear_screen(int counter);

int main(int argc, char *argv[]) {
    int param = 0;
    
    FILE *fp = stdin;

    if (argc == 2) {
        if (access(argv[1], F_OK) == 0) {
            fp = fopen(argv[1], "r");
            param = -1;
        } else {
            printf("file does not exist");
            return ret_end(1, NULL, NULL);
        }
    }

    int **cond = get_matrix(FIELD_H, FIELD_W);
    if (!cond) {
        printf("memory error");
        return ret_end(1, NULL, NULL);
    }
    int **next_cond = get_matrix(FIELD_H, FIELD_W);
    if (!next_cond) {
        printf("memory error");
        return ret_end(1, cond, NULL);
    }
    if (init(cond, param, fp) == 1) {
        printf("file read error");
        return ret_end(1, cond, next_cond);
    }
    if (param == -1) {
        fclose(fp);
    }
    int iter = 0;
    while (1) {
        draw(cond, iter);
        int inp = get_input();
        if (inp == 1) {
            logic(&cond, &next_cond);
            iter++;
        } else if (inp == -1) {
            break;
        }
    }
    return ret_end(0, cond, next_cond);;
}

int get_input() {
    char ch;
    int inp;
    scanf("%c", &ch);
    if (ch == KEY_NEXT)
        inp = 1;
    else if (ch == KEY_EXIT)
        inp = -1;
    else
        inp = 0;
    fseek(stdin, 0, SEEK_END);
    return inp;
}

void draw(int** field, int iter) {
    clear_screen(20);
    printf("TURN %d\n", iter);
    printf("#");
    for (int i = 0; i < FIELD_W; ++i)
        printf("-");
    printf("#\n");
    for (int i = 0; i <= FIELD_H - 1; i++) {
        printf("|");
        for (int j = 0; j <= FIELD_W - 1; j++) {
            if (field[i][j])
                printf("%c", CELL);
            else
                printf("%c", NO_CELL);
        }
        printf("|\n");
    }
    printf("#");
    for (int i = 0; i < FIELD_W; ++i)
        printf("-");
    printf("#\n");
}

void clear_screen(int counter) {
    while (counter > 0) {
        --counter;
        printf("\n");
    }
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

int check_neighbor(int **field, int i, int j) {
    i = (i + FIELD_H) % FIELD_H;
    j = (j + FIELD_W) % FIELD_W;
    return field[i][j];
}

int count_neighbor(int **field, int i, int j) {
    int count = 0;

    count += check_neighbor(field, i - 1, j - 1);
    count += check_neighbor(field, i - 1, j);
    count += check_neighbor(field, i - 1, j + 1);
    count += check_neighbor(field, i, j - 1);
    count += check_neighbor(field, i, j + 1);
    count += check_neighbor(field, i + 1, j - 1);
    count += check_neighbor(field, i + 1, j);
    count += check_neighbor(field, i + 1, j + 1);

  return count;
}

void logic(int** *m_cur, int** *m_nxt) {
    int **field = *m_cur;
    int **field_num2 = *m_nxt;
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
    swap_matrix(m_cur, m_nxt);
}

void swap_matrix(int** *a, int** *b) {
    int **s = *a;
    *a = *b;
    *b = s;
}

void generate_field(int** mas, int param) {
    srand(param);
    for (int i = 0; i < FIELD_H; i++)
        for (int j = 0; j < FIELD_W; j++)
            mas[i][j] = (rand() % 100 < DEFAULT_FILLNESS);
}

int init(int** mas, int param, FILE *fp) {
    clear_mas(mas);
    if (param == -1)
        return scan_mas(mas, fp);
    else
        generate_field(mas, param);
    return 0;
}

void clear_mas(int** mas) {
    for (int i = 0; i < FIELD_H; i++)
        for (int j = 0; j < FIELD_W; j++)
            mas[i][j] = 0;
}

int scan_mas(int** mas, FILE *fp) {
    int i = 0, j = 0;
    while (1) {
        int ch = fgetc(fp);
        if (ch == '1') {
            if (j < FIELD_W && i < FIELD_H) {
                mas[i][j] = 1;
                j++;
            } else {
                return 1;
            }
        } else if (ch == '0') {
            if (j < FIELD_W && i < FIELD_H) {
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

int ret_end(int code, int **m1, int **m2) {
    if (m1 != NULL)
        free_matrix(m1);
    if (m2 != NULL)
        free_matrix(m2);
    return code;
}
