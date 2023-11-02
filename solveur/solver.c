//Open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//Read
#include <unistd.h>

//printf
#include <stdio.h>

//Exit
#include <stdlib.h>

//mempcy
#include <string.h>




int is_line_ok(int nb, int line, char s[])
{
	int pos = line * 12;
	int lim = pos + 12;

	//Cette fonction vérifie que le nombre n'est pas déjà dans la ligne 
	while(pos < lim)
	{
		if(s[pos] == nb) return 0;
		pos ++;
	}
	return 1;
}


int is_col_ok(int nb, int col, char s[])
{
	int pos = col;
	int lim = 131;

	//Cette fonction vérifie que le nombre n'est pas déjà dans la colonne
	while(pos < lim)
	{
		if(s[pos] == nb) return 0;
		pos += 12;
	}
	return 1;
}


int is_square_ok(int nb, int line, int col, char s[])
{
	line -= (line % 3);
	col -= (col % 3);
	int line_lim = line + 3;
	int col_lim = col + 3;
	int save = col;

	//Cette fonction vérifie que le nombre n'est pas déjà dans la colonne ni dans la ligne
	while(line < line_lim)
	{
		while(col < col_lim)
		{
			if(s[line * 12 + col] == nb) return (0);
			col ++;
		}
		col = save;
		line ++;
	}
	return (1);
}


int is_empty(int pos, char s[])
{
	if(s[pos] == '.') return 1;
	else return 0;
}


int sudoku(int pos, int empty, char s[])
{
	//Fin du sudoku
	if(pos == 131)
	  {
	    return 0;
	  }
	
	if(s[pos] == ' ')
	  {
	    return (sudoku(pos + 1, is_empty(pos + 1, s), s));
	  }

	//Test : case déjà complété
	if(empty == 0)
	  {
	    return (sudoku(pos + 1, is_empty(pos + 1, s), s));
	  }
 
	//Remplissage de la case
	else
	{
		char sol[9] = "123456789";
		int sol_pos = 0;

		while(sol_pos < 9)
		{
			if(is_line_ok(sol[sol_pos], pos / 12, s) == 1 && is_col_ok(sol[sol_pos], pos % 12, s) == 1 && is_square_ok(sol[sol_pos], pos /12, pos % 12, s) == 1)
			{
				s[pos] = sol[sol_pos];
				if(sudoku(pos + 1, is_empty(pos + 1, s), s) == 0)
				  {
				    return 0;
				  }
				
				else
				  {
				    s[pos] = '.';
				  }
			}
			sol_pos ++;
		}
		if(sol_pos == 9)
		  {
		    return -1;
		  }
	}
}

/*
void print_sudoku(char s[])
{
	char display[132];
	int count = 0;
	int pos = 0;

	//Test compteur contenue dans le sudoku
	while(count < 108)
	{
		if(count != 0)
		{
			//Le modulo correspond aux trois lignes de carré
			if(count % 27 == 0)
			{
				memcpy(&display[pos], "\n           \n", 13);
				pos += 13;
			}
			//Ce modulo correspond aux différents carré
			else if(count % 12 == 0)
			{
				memcpy(&display[pos], "\n", 1);
				pos += 1;
			}
		}
		
		display[pos] = s[count];
		pos += 1;
		count += 1;
	}
	display[131] = 0;
	printf("%s\n", display);
	}*/


void print_sudoku(char s[]) {
    char display[132];
    int count = 0;
    int pos = 0;

    while (count < 132) {
        if (count != 0) {
            
        }

        display[pos] = s[count];

        pos += 1;
        count += 1;
    }

    display[131] = '\0'; 
    printf("%s\n", display);
}



/*
void error(char *av, char s[])
{
	//Open
	int fd = 0;
	
	if((fd = open(av, O_RDONLY)) == -1)
	{
		printf("Cannot open file or file does not exist!\n");
		exit(1);
	}
	
	//Read
	if(read(fd, s, 81) != 81)
	{
		printf("Incorrect line size\n");
		exit(1);
	}
	s[132] = 0;
	close(fd);
	
	//Verif sudoku
	int count = 0;
	
	while(s[count] != '\0')
	{
	  if((s[count] != '.') & (s[count] != ' ') & (s[count] != '\n') & (!(s[count] >= '0' && s[count] <= '9') ))
		{
			printf("Wrong Sudoku!\n");
			exit(1);
		}
		count += 1;
	}
}
*/

void into_file(char s[])
{
  FILE* finalsudo = fopen("result","w+");

  if (finalsudo == NULL)
    {
      perror("Error opening file");
      return;
    }

  fwrite(s, 1, 132, finalsudo);
	
  fclose(finalsudo);
}


void readDoc(const char *name, char s[], int taille)
{
    FILE *fichier = fopen(name, "r");

    if (fichier == NULL)
      {
        perror("Error opening file");
        return;
      }

    fread(s, 1, taille, fichier);
    s[taille] = '\0';

    fclose(fichier);
}

int main(int argc, char** argv)
{
  char s[132];

	if(argc != 2)
	{
		printf("Wrong argument!\n");
		return 1;
	}
	
	//Reading document
	readDoc(argv[1], s, 132);	
	
	//Resolution
	sudoku(0, is_empty(0,s), s);

	//Display result
	//print_sudoku(s);

	//Put the result into a file
	into_file(s);

	return 0;
}

// test : gcc main.c && ./a.out sudoku.txt
