#ifndef SUDOKU_H
#define SUDOKU_H


#include <stdio.h>
#include <stdlib.h>


extern char POSSIBLE;
extern int SIZE_ROWS;
extern int SIZE_COLUMNS;

typedef struct Box {

  struct Box * next;

} Box;

typedef struc Square {

  int number;
  int possible[9];
  int solvable;
  Box * box;
  int row;
  in column;sudoku[i][j]->number = puzzle[i][j];

} Square;
