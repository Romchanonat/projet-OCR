#include "sudoku.h"

int **CreatePuzzleFromTxt(char s[])
{
  
}


Square** SetUpPuzzle(int ** puzzle)
{
  Square*** sudoku;
  int i, j;

  sudoku = (Square***)malloc(sizeof(Square**)*9);

  //Loop in order to move through the rows
  for (i = 0; i < SIZE_ROWS; i++)
    {
      sudoku[i] = (Square**)malloc(sizeof(Square*)*9);

      //Loop in order to move through the columns
      for(j = 0; j < SIZE_COLUMNS; j++)
	{
	  sudoku[i][j] = (Square*)malloc(sizeof(Square)*9);

	  //Initalities everything
	  sudoku[i][j]->number = puzzle[i][j];
	  sudoku[i][j]->row = i;
	  sudoku[i][j]->column = j;

	  //If the number is not 0, the square is not empty
	  if(sudoku->number != 0)
	    sudoku[i][j]->code = POSSIBLE;
	  else
	    sudoku[i][j]->code = 0x0;
 
	  
	}
    }
}
