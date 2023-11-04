#include "solver.h"


int CheckRows(Square *** sudoku, Box ** boxes)
{
  int i, j, k;

  int sum[9];
  int place[9];

  //Loop through the rows
  for (i = 0; i < 9; i++)
    {
      //Initialize sum
      for (j = 0; j < 9; j++)
	{
	  sum[j] = 0;
	  place[j] = 0;
	}
      
      //Loop through each square
      for (j = 0; j < 9; j++)
	{
	  if (sudoku[i][j]->number != 0)
	    continue;

	  //Loop through all possibles
	  for (k = 0; k < 9; k++)
	    {
	      //Check if number is possible
	      if (sudoku[i][j]->possible[k] == 0)
		{
		  sum[k]++;
		  place[k] = j;
		}
	    }
	}

      for (k = 0; k < 9; k++)
	{
	  if (sum[k] == 1)
	    {
	      sudoku[i][place[k]]->number = k + 1;
	      sudoku[i][place[k]]->solvable = 0;
	      UNSOLVED--;

	      UpdateSudoku(sudoku, i, place[k]);
	      UpdateBoxes(sudoku, i, place[k]);

	      return 1;
	      
	    }
	}
      
    }
  return 0;
}
	
