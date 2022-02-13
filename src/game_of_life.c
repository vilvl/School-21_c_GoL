#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define FIELD_W 80
#define FIELD_H 25

#define CELL '*'
#define NO_CELL ' '

#define KEY_NEXT ' '
#define KEY_EXIT 'q'

#define DEFAULT_FILLNESS 10

int get_input();
int get_cmd_args(int argc, char **argv, FILE** fp);

int** get_matrix(int rws, int cls);
void free_matrix(int **m);

void logic(int** *m_cur, int** *m_nxt);
int count_neighbor(int **field, int i, int j);
int check_neighbor(int **field, int i, int j);
void swap_matrix(int** *a, int** *b);

int init(int** mas, int param, FILE *file);
void clear_mas(int** mas);
int scan_mas(int** mas, FILE *file);
void generate_field(int** mas, int param);

int ret_end(int code, int **m1, int **m2);
int is_pos_int(char *number);

void draw(int** field, int iter, int param);
void clear_screen(int counter);

// param is a game mode
// -1 to read preset from file by path
// -2 for error
//  0 to read preset from stdin
// pos int to seed and generate field
int main(int argc, char **argv) {
    FILE *fp;
    int param = get_cmd_args(argc, argv, &fp);
    if (param == -2) {
        printf("arg should be positive integer or path to a preset file");
        return 1;
    }
    int **cond = get_matrix(FIELD_H, FIELD_W);
    int **next_cond = get_matrix(FIELD_H, FIELD_W);
    if (cond == NULL || next_cond == NULL) {
        printf("memory error");
        return ret_end(1, cond, next_cond);
    }
    if (init(cond, param, fp) == 1) {
        printf("file read error");
        return ret_end(1, cond, next_cond);
    }
    if (param == -1) {
        fclose(fp);
    }
    int iter = 0;
    draw(cond, iter, param);
    while (1) {
        int inp = get_input();
        if (inp == 1) {
            logic(&cond, &next_cond);
            iter++;
        } else if (inp == -1) {
            break;
        }
        draw(cond, iter, param);
    }
    return ret_end(0, cond, next_cond);;
}

//////////// INPUT ///////////////

int get_cmd_args(int argc, char **argv, FILE** fp) {
    int param = 0;
    *fp = stdin;
    if (argc > 1) {
        if (access(argv[1], F_OK) == 0) {
            *fp = fopen(argv[1], "r");
            param = -1;
        } else if ((param = is_pos_int(argv[1]))) {
        } else {
            param = -2;
        }
    }
    return param;
}

int get_input() {
    int inp;
    int ch = getchar();
    if (ch == KEY_NEXT)
        inp = 1;
    else if (ch == KEY_EXIT)
        inp = -1;
    else
        inp = 0;
    while (1) {
        ch = getchar();
        if (ch == '\n' || ch == EOF)
            break;
    }
    return inp;
}

/////////// DRAWING //////////////

void draw(int** field, int iter, int param) {
    clear_screen(20);
    if (param > 0)
        printf("SEED %d; ", param);
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

/////////// MEMORY FREE & RESERVE //////////////

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

/////////// LOGIC_TROUGH_TURNS //////////////////

void logic(int ***m_cur, int ***m_nxt) {
    int **field = *m_cur;
    int **field_num2 = *m_nxt;
    int i, j, nbr;
    for (i = 0; i < FIELD_H; i++) {
        for (j = 0; j < FIELD_W; j++) {
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

int check_neighbor(int **field, int i, int j) {
    i = (i + FIELD_H) % FIELD_H;
    j = (j + FIELD_W) % FIELD_W;
    return field[i][j];
}

void swap_matrix(int** *a, int** *b) {
    int **s = *a;
    *a = *b;
    *b = s;
}

/////////// INIT_FIELD_AT_START //////////////////

int init(int** mas, int param, FILE *fp) {
    clear_mas(mas);
    if (param == -1) {
        return scan_mas(mas, fp);
    } else if (param > 0) {
        generate_field(mas, param);
    } else if (param == 0) {
        int res = scan_mas(mas, fp);
        stdin = freopen("/dev/tty", "rw", stdin);
        return res;
    }
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

void generate_field(int** mas, int param) {
    srand(param);
    for (int i = 0; i < FIELD_H; i++)
        for (int j = 0; j < FIELD_W; j++)
            mas[i][j] = (rand() % 100 < DEFAULT_FILLNESS);
}

/////////// ERROR_CHECKERS //////////////////

int is_pos_int(char * const number) {
    int num = 0;
    for (int i = 0; number[i] != '\0'; i++) {
        if (number[i] > '9' || number[i] < '0')
            return -2;
        num *= 10;
        num += number[i] - '0';
        if (num >= RAND_MAX)
            return -2;
    }
    return num;
}

int ret_end(int code, int **m1, int **m2) {
    if (m1 != NULL)
        free_matrix(m1);
    if (m2 != NULL)
        free_matrix(m2);
    return code;
}
