#ifndef SOLVER_H
#define SOLVER_H

int is_line_ok(int nb, int line, char s[]);
int is_col_ok(int nb, int col, char s[]);
int is_square_ok(int nb, int line, int col, char s[]);
int is_empty(int pos, char s[]);
int sudoku(int pos, int empty, char s[]);
void print_sudoku(char s[]);
void error(char *av, char s[]);


#endif
