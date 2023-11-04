#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "solver.h"

int UNSOLVED = 81;
int SIZE_ROWS = 9;
int SIZE_COLUMNS = 9;


int main(int argc, char** argv)
{
 
  if(argc != 2)
  {
	printf("Wrong argument!\n");
	return 1;
  }
    
  
  int ** puzzle;
  int progress;
  Sudoku *  sudoku;

  puzzle = CreatePuzzleFromTxt(argv[1]);

  //printPuzzle(puzzle);
  //printf("Puzzle printed successfully.\n"); 

  sudoku = SetUpPuzzle(puzzle);
  //printf("puzzle set sucess\n");

  while (UNSOLVED > 0)
    {
      progress = CheckPuzzle(sudoku->squares, sudoku->boxes);

      if (progress == 0)
	{
	  break;
	}
    }

  //printf("résolu");
  
  FILE* result = fopen(strcat(argv[1], ".result"),"w+");
  
  IntoFile(result, sudoku);

  //printf("finito");

  fclose(result);

  return 0;
}
