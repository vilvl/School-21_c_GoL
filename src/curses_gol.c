#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ncurses.h>
// #include <time.h>

#define FIELD_W 80
#define FIELD_H 25

#define CELL '*'
#define NO_CELL ' '

#define KEY_NEXT_ ' '
#define KEY_EXIT_ 'q'
#define KEY_STEP_MODE 'e'
#define KEY_INCREASE_SPEED 'i'
#define KEY_SLOW_DOWN_SPEED 'j'

#define KEY_UP_ 'w'
#define KEY_RIGHT_ 'd'
#define KEY_DOWN_ 's'
#define KEY_LEFT_ 'a'
#define KEY_CHANGE_CELL 'c'

#define DEFAULT_FILLNESS 10

#define DEFAULT_SLEEP 500
#define SLEEP_CHANGE_STEP 1.2
#define MIN_SLEEP 25
#define MAX_SLEEP 2500

#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void game_loop(int **cond, int **next_cond, int param);

int get_cmd_args(int argc, char **argv, FILE** fp);
int move_cursor(int ch, int *curs_x, int *curs_y);
int change_step_by_step(int *mode, int *curs_x, int *curs_y, int **cond, int iter, int param);
int change_auto_mode(int *mode, int *sleep_ms);

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

void draw(int** field, int iter, int param,
          int mode, int ms_sleep, int curs_x, int curs_y);

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
    draw(cond, 0, param, 0, DEFAULT_SLEEP, 0, 0);
    game_loop(cond, next_cond, param);

    return ret_end(0, cond, next_cond);
}

void game_loop(int **cond, int **next_cond, int param) {
    int sleep_ms = DEFAULT_SLEEP;
    int iter = 1;
    int mode = 0;
    int curs_x = FIELD_W/2;
    int curs_y = FIELD_H/2;

    while (getch() != KEY_NEXT_) {}
    while (1) {
        if (mode) {
            if (change_step_by_step(&mode, &curs_x, &curs_y, cond, iter, param))
                return;
        } else {
            if (change_auto_mode(&mode, &sleep_ms))
                return;
        }
        logic(&cond, &next_cond);
        draw(cond, iter, param, mode, sleep_ms, curs_x, curs_y);
        iter++;
    }
}



//////////// INPUT ///////////////

int change_step_by_step(int *mode, int *curs_x, int *curs_y, int **cond, int iter, int param) {
    while (1) {
        int ch = getch();
        if (ch == KEY_NEXT_) {
            break;
        } else if (ch == KEY_STEP_MODE) {
            *mode = !(*mode);
            break;
        } else if (ch == KEY_EXIT_) {
            return 1;
        } else if (ch == KEY_CHANGE_CELL) {
            cond[*curs_y][*curs_x] = !cond[*curs_y][*curs_x];
            draw(cond, iter, param, *mode, 0, *curs_x, *curs_y);
        } else if (move_cursor(ch, curs_x, curs_y)) {
            draw(cond, iter, param, *mode, 0, *curs_x, *curs_y);
        }
    }
    return 0;
}

int change_auto_mode(int *mode, int *sleep_ms) {
    usleep(1000 * (*sleep_ms));
    switch (getch()) {
        case KEY_EXIT_:
            return 1;
        case KEY_INCREASE_SPEED:
            if (*sleep_ms > MIN_SLEEP)
                *sleep_ms /= SLEEP_CHANGE_STEP;
            break;
        case KEY_SLOW_DOWN_SPEED:
            if (*sleep_ms < MAX_SLEEP)
                *sleep_ms *= SLEEP_CHANGE_STEP;
            break;
        case KEY_STEP_MODE:
            *mode = !(*mode);
            break;
    }
    return 0;
}


int move_cursor(int ch, int *curs_x, int *curs_y) {
    if (ch == KEY_UP_)
        *curs_y = ((*curs_y - 1) + FIELD_H) % FIELD_H;
    else if (ch == KEY_DOWN_)
        *curs_y = (*curs_y + 1) % FIELD_H;
    else if (ch == KEY_LEFT_)
        *curs_x = ((*curs_x - 1) + FIELD_W) % FIELD_W;
    else if (ch == KEY_RIGHT_)
        *curs_x = (*curs_x + 1) % FIELD_W;
    else
        return 0;
    return 1;
}

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

/////////// DRAWING //////////////

void draw(int** field, int iter, int param, int mode, int ms_sleep, int curs_x, int curs_y) {
    clear();
    if (param > 0)
        printw("SEED: %d; ", param);
    if (mode)
        printw("MODE: STEP; ");
    else
        printw("MODE: AUTO; SLEEP %d; ", ms_sleep);
    printw("TURN: %d\n", iter);
    printw("#");
    for (int i = 0; i < FIELD_W; ++i)
        printw("-");
    printw("#\n");
    for (int i = 0; i <= FIELD_H - 1; i++) {
        printw("|");
        for (int j = 0; j <= FIELD_W - 1; j++) {
            if ((i == curs_y) && (j == curs_x) && mode)
                attron(COLOR_PAIR(2));
            if (field[i][j])
                printw("%c", CELL);
            else
                printw("%c", NO_CELL);
            if ((i == curs_y) && (j == curs_x) && mode)
                attron(COLOR_PAIR(1));
        }
        printw("|\n");
    }
    printw("#");
    for (int i = 0; i < FIELD_W; ++i)
        printw("-");
    printw("#\n");
    if (mode) {
        printw("SPACE - NEXT TURN, E - CHANGE TO AUTO MODE, Q - EXIT\n");
        printw("WASD - MOVEMENT, C - KILL or BORN CELL BELOW CURSOR\n");
    } else {
        printw("E - CHANGE TO STEP BY STEP MODE, Q - EXIT\n");
        printw("I - INCREASE SPEED, J - DECREASE SPEED\n");
    }
    refresh();
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
    int res_err = 0;
    if (param == -1) {
        res_err = scan_mas(mas, fp);
    } else if (param > 0) {
        generate_field(mas, param);
    } else if (param == 0) {
        res_err = scan_mas(mas, fp);
        stdin = freopen("/dev/tty", "rw", stdin);
    }
    // ncurses init
    if (!res_err) {
        initscr();
        start_color();
        noecho();
        curs_set(0);
        nodelay(stdscr, TRUE);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_WHITE);
        attron(COLOR_PAIR(1));
}
    return res_err;
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
