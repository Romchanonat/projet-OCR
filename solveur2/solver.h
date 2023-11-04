#ifndef SUDOKU_H
#define SUDOKU_H


#include <stdio.h>
#include <stdlib.h>


extern int UNSOLVED;
extern int SIZE_ROWS;
extern int SIZE_COLUMNS;

typedef struct Sudoku {

  struct Square *** squares;
  struct Box ** boxes;
  
} Sudoku;

typedef struct Box {

  struct Square ** squares;
  int numbers;
  int possible[9];
  int solvable;
  struct Box * next;

} Box;

typedef struct Square {

  int number;
  /* If possible[x] == 1 then the number can't go in*/
  int possible[9];
  int solvable;
  Box * box;
  int row;
  int column;

} Square;

void printPuzzle(int ** puzzle);

int ** CreatePuzzleFromTxt(const char *name);
Sudoku * SetUpPuzzle(int ** puzzle);

Sudoku * CreateSudoku(Square *** squares, Box ** boxes);

int SolveSquare(Square* square);
int CheckPuzzle(Square *** sudoku, Box ** boxes);
int UpdateSudoku(Square *** sudoku, int row, int column);

void IntoFile(FILE* result, Sudoku * sudoku);
int **CreatePuzzleFromTxt(const char *name);



//Box functions

Box** CreateBoxes();
int UpdateBoxes(Square *** sudoku, int row, int column);

#endif // SUDOKU_H
