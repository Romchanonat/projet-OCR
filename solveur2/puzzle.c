#include "solver.h"
#include <string.h>


void printPuzzle(int ** puzzle)
{
  int i, j;
  for (i = 0; i < 9; i++)
    {
      for (j = 0; j < 9; j++)
	{
	  printf(" %d ", puzzle[i][j]);
	  if ((j+1) % 3 == 0)
	    printf("|");
	}
      printf("\n");
      if ((i+1) % 3 == 0)
	printf("----------------------------\n");
    }
}

int **CreatePuzzleFromTxt(const char *name)
{
    int i = 0, j = 0;
    int **puzzle;
    int array[9][9];
    char currentChar;

    FILE *fichier = fopen(name, "r");

    if (fichier == NULL)
    {
        perror("Error opening file");
        return NULL;
    }

    while (i < 9)
    {
        currentChar = fgetc(fichier); // Read the character

        if (currentChar == EOF)
        {
            break; // Exit the loop if end of file is reached
        }

        switch (currentChar)
        {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            array[i][j] = currentChar - '0'; // Convert char to int
            j++;
            if (j == 9)
            {
                i++;
                j = 0;
            }
            break;

        case '.':
            array[i][j] = 0;
            j++;
            if (j == 9)
            {
                i++;
                j = 0;
            }
            break;

        default:
            break;
        }
    }

    puzzle = (int **)malloc(sizeof(int *) * 9);

    if (puzzle == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(fichier);
        return NULL;
    }

    for (int i = 0; i < 9; i++)
    {
        puzzle[i] = (int *)malloc(sizeof(int) * 9);

        if (puzzle[i] == NULL)
        {
            printf("Memory allocation failed.\n");
            // Vous voudrez peut-être libérer la mémoire précédemment allouée avant de retourner NULL.
            fclose(fichier);
            return NULL;
        }

        memcpy(puzzle[i], array[i], sizeof(int) * 9);
    }

    fclose(fichier);
    return puzzle; // Retourne la grille créée
}



void IntoFile(FILE* result, Sudoku * sudoku)
{
  int i, j;

  for (i = 0; i < 9; i++) {
    for (j = 0; j < 9; j++) {
      if (j % 3 == 0 && j > 0) {
        fprintf(result, " ");
      }

      fprintf(result, "%c", sudoku->squares[i][j]->number == 0 ? '.' : sudoku->squares[i][j]->number + '0');
    }

    fprintf(result, "\n");

    if ((i + 1) % 3 == 0 && i < 8) {
      fprintf(result, "\n");
    }
  }
}


Sudoku * CreateSudoku(Square *** squares, Box ** boxes)
{
  Sudoku * sudoku;
  sudoku = malloc(sizeof(Sudoku));

  sudoku->squares = squares;
  sudoku->boxes = boxes;

  return sudoku;
}

int UpdateSudoku(Square *** sudoku, int row, int column)
{
  int x;
    int number = sudoku[row][column]->number;

    for (x = 0; x < SIZE_ROWS; x++)
    {
        if (sudoku[x][column]->possible[number - 1] == 0)
        {
            sudoku[x][column]->solvable--;
        }
        sudoku[x][column]->possible[number -1] = 1;
    }

    for (x = 0; x < SIZE_COLUMNS; x++)
    {
        if (sudoku[row][x]->possible[number - 1] == 0)
        {
            sudoku[row][x]->solvable--;
        }
        sudoku[row][x]->possible[number -1] = 1;
    }

    return 1;

}

Sudoku * SetUpPuzzle(int ** puzzle)
{
    Square *** sudoku;
    Box ** boxes;
    int i, j, x;
    int currentBox = 0;

    sudoku = (Square***)malloc(sizeof(Square**)*9);
    boxes = CreateBoxes();

    /* loop through rows */
    for (i = 0; i < SIZE_ROWS; i++)
    {
        /* malloc space for each row */
        sudoku[i] = (Square**)malloc(sizeof(Square*)*9);

        /* loop through columns */
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            /* malloc space for each square */
            sudoku[i][j] = (Square*)malloc(sizeof(Square)*9);

            /* assign number to sudoke adt */
            sudoku[i][j]->number = puzzle[i][j];

            /* assign row and column numbers to each square */
            sudoku[i][j]->row = i;
            sudoku[i][j]->column = j;
            sudoku[i][j]->solvable = 9;

            boxes[currentBox]->squares[ boxes[currentBox]->numbers ] = sudoku[i][j];
            sudoku[i][j]->box = boxes[currentBox];
            boxes[currentBox]->numbers++;

            for (x = 0; x < SIZE_ROWS; x++)
            {
                sudoku[i][j]->possible[x] = 0;
            }

            if (j == 2)
                currentBox++;
            if (j == 5)
                currentBox++;

        }

        currentBox -= 2;
        if (i == 2)
            currentBox = 3;
        if (i == 5)
            currentBox = 6;
    }

    /* loop through rows */
    for (i = 0; i < SIZE_ROWS; i++)
    {
        /* loop through columns */
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            if (sudoku[i][j]->number != 0)
            {
                sudoku[i][j]->solvable = 0;
                UpdateSudoku(sudoku, i, j);
                UpdateBoxes(sudoku, i, j);
                UNSOLVED--;
            }
        }
    }

    return CreateSudoku(sudoku, boxes);
}

int CheckPuzzle(Square *** sudoku, Box** boxes)
{
  int i, j, x;

    /* loop through rows */
    for (i = 0; i < SIZE_ROWS; i++)
    {
        /* loop through columns */
        for (j = 0; j < SIZE_COLUMNS; j++)
        {
            if (sudoku[i][j]->solvable == 1)
            {
                SolveSquare(sudoku[i][j]);
                UpdateSudoku(sudoku, i, j);
                UpdateBoxes(sudoku, i, j);

                return 1;
            }
        }
    }

    return BoxSingles(sudoku, boxes);
}
