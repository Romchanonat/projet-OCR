//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//read
#include <unistd.h>

//printf
#include <stdio.h>

//exit
#include <stdlib.h>

//mempcy
#include <string.h>


int is_line_ok(int nb, int line, char s[])
{
	int pos = line * 12;
	int lim = pos + 12;

	//cette fonction vérifie que le nombre n'est pas déjà dans la ligne 
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

	//cette fonction vérifie que le nombre n'est pas déjà dans la colonne
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

	//cette fonction vérifie que le nombre n'est pas déjà dans la colonne ni dans la ligne
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
	//fin du sudoku
	if(pos == 131) return 0;

	if(s[pos] == ' ') return (sudoku(pos + 1, is_empty(pos + 1, s), s));
	//test : case déjà complété
	if(empty == 0) return (sudoku(pos + 1, is_empty(pos + 1, s), s));
	//remplissage de la case
	else
	{
		char sol[9] = "123456789";
		int sol_pos = 0;

		while(sol_pos < 9)
		{
			if(is_line_ok(sol[sol_pos], pos / 12, s) == 1 && is_col_ok(sol[sol_pos], pos % 12, s) == 1 && is_square_ok(sol[sol_pos], pos /12, pos % 12, s) == 1)
			{
				s[pos] = sol[sol_pos];
				if(sudoku(pos + 1, is_empty(pos + 1, s), s) == 0) return 0;
				else s[pos] = '.';
			}
			sol_pos ++;
		}
		if(sol_pos == 9) return -1;
	}
	
}


void print_sudoku(char s[])
{
	char display[131];
	int count = 0;
	int pos = 0;

	//Test compteur contenue dans le sudoku
	while(count < 108)
	{
		if(count != 0)
		{
			// le modulo correspond aux trois lignes de carré
			if(count % 27 == 0)
			{
				memcpy(&display[pos], "\n           \n", 13);
				pos += 13;
			}
			// ce modulo correspond aux différents carré
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
}


void error(char *av, char s[])
{
	//open
	int fd = 0;
	if((fd = open(av, O_RDONLY)) == -1)
	{
		printf("Cannot open file or file does not exist!\n");
		exit(1);
	}
	
	//read
	if(read(fd, s, 81) != 81)
	{
		printf("Incorrect line size\n");
		exit(1);
	}
	s[108] = 0;
	close(fd);
	
	//Verif sudoku
	int count = 0;
	while(s[count] != '\0')
	{
		if(s[count] != '.' & s[count] != ' ' & s[count] != '\n' & !(s[count] >= '0' && s[count] <= '9'))
		{
			printf("Wrong Sudoku!\n");
			exit(1);
		}
		count += 1;
	}
}


int main(int ac, char **av)
{
	char s[109];

	if(ac != 2)
	{
		printf("Wrong argument!\n");
		return 1;
	}
	//Gestion d'erreur 
	error(av[1], s);

	//Resolution
	sudoku(0, is_empty(0,s), s);

	//Display result
	print_sudoku(s);

	return 0;
}

// test : gcc main.c && ./a.out sudoku.txt
