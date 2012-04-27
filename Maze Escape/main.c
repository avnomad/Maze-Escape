//	Copyright (C) 2006, 2008, 2012 Vaptistis Anogeianakis <el05208@mail.ntua.gr>
/*
 *	This file is part of Maze Escape.
 *
 *	Maze Escape is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	Maze Escape is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with Maze Escape.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct cell_tag
{
	char row;
	char col;
} cell;

void find_exit(cell current,cell direction,char *maze,char maxcol,char maxrow);

int main()
{
	char *filename,ch,*maze;
	FILE *inputfile;
	char maxrow,maxcol,row,col;
	cell start = {0,0},current,direction;


	printf("Give the name of the file describing the maze: ");
	if(!(filename = malloc(FILENAME_MAX*sizeof(char))))
	{
		perror("An error has occured: ");
	}
	gets(filename);
	if(!(inputfile = fopen(filename,"r")))
	{
		printf("could not open file: %s.\n",filename);
		system("PAUSE");
		return 0;
	}
	free(filename);



	maxrow = getc(inputfile)+2-'0';
	maxcol = getc(inputfile)+2-'0';
	getc(inputfile);

	if(!(maze = calloc(maxrow*maxcol,sizeof(char))))
	{
		perror("An error has occurred: ");
	}

	for(col=0;col<maxcol;col++)
		*(maze+col) = '0';
	row = 1;	
	while((ch=getc(inputfile))!=EOF)
	{
		*(maze+row*maxcol) = '0';
		col = 1;
		while(ch!='\n')
		{
			if(ch=='#')
			{
				start.row = row;
				start.col = col;
			}
			*(maze+row*maxcol+col) = ch;
			col++;
			ch = getc(inputfile);
		}
		*(maze+row*maxcol+col) = '0';
		row++;
	}
	for(col=0;col<maxcol;col++)
		*(maze+row*maxcol+col) = '0';

	if(start.row || start.col)
	{
		if(*(maze+(current.row = start.row)*maxcol+(current.col = start.col-1))!='0')
		{
			/*printf("Hi 1\n");*/
			direction.row = 0;
			direction.col = -1;
			find_exit(current,direction,maze,maxcol,maxrow);
		}
		if(*(maze+(current.row = start.row+1)*maxcol+(current.col = start.col))!='0')
		{
			/*printf("Hi 2\n");*/
			direction.row = 1;
			direction.col = 0;
			find_exit(current,direction,maze,maxcol,maxrow);
		}
		if(*(maze+(current.row = start.row)*maxcol+(current.col = start.col+1))!='0')
		{
			/*printf("Hi 3\n");*/
			direction.row = 0;
			direction.col = 1;
			find_exit(current,direction,maze,maxcol,maxrow);
		}
		if(*(maze+(current.row = start.row-1)*maxcol+(current.col = start.col))!='0')
		{
			/*printf("Hi 4\n");*/
			direction.row = -1;
			direction.col = 0;
			find_exit(current,direction,maze,maxcol,maxrow);
		}
		printf("This maze contains no paths from entry to exit.\n");
	}
	else
		printf("No entry point detected. Please check the input file for errors.\n");

	/**
	printf("(%d,%d)\n",start.row,start.col);
	for(row=0;row<maxrow;row++)
	{
		for(col=0;col<maxcol;col++)
			printf("%c",*(maze+row*maxcol+col));
		putchar('\n');
	}
	/**/


	fclose(inputfile);
	system("PAUSE");
	return 0;
}

void find_exit(cell current,cell direction,char *maze,char maxcol,char maxrow)
{
	char temp;
	FILE *outputfile;
	char row,col;

	while(*(maze+current.row*maxcol+current.col)!='#' && *(maze+current.row*maxcol+current.col)!='@')
	{
		/**/
		/*system("PAUSE");*/
		system("CLS");
		for(row=0;row<maxrow;row++)
		{
			for(col=0;col<maxcol;col++)
				printf("%c",*(maze+row*maxcol+col));
			putchar('\n');
		}
		system("PAUSE");
		/*printf("*********************************\n");*/
		/**/
		if(*(maze+(current.row+direction.col)*maxcol+(current.col-direction.row))!='0')
		{
			/*printf("left\n");*/
			temp = direction.col;
			direction.col = -direction.row;
			direction.row = temp;
		}
		else if(*(maze+(current.row+direction.row)*maxcol+(current.col+direction.col))!='0')
		{
			/*printf("onwords\n");*/
		}
		else if(*(maze+(current.row-direction.col)*maxcol+(current.col+direction.row))!='0')
		{
			/*printf("right\n");*/
			temp = -direction.col;
			direction.col = direction.row;
			direction.row = temp;
		}
		else if(*(maze+(current.row-direction.row)*maxcol+(current.col-direction.col))!='0')
		{
			/*printf("backwords\n");*/
			direction.col = -direction.col;
			direction.row = -direction.row;
		}
		if(*(maze+(current.row+direction.row)*maxcol+(current.col+direction.col))=='1' || *(maze+(current.row+direction.row)*maxcol+(current.col+direction.col))=='@')
		{
			*(maze+(current.row)*maxcol+(current.col)) = '*';
		}
		else
		{
			*(maze+(current.row)*maxcol+(current.col)) = '1';
		}
		current.row = current.row + direction.row;
		current.col = current.col + direction.col;
	}
	if(*(maze+current.row*maxcol+current.col)=='@')
	{
		outputfile = fopen("maze_exit.txt","w");		
		for(row=1;row<maxrow-1;row++)
		{
			for(col=1;col<maxcol-1;col++)
				putc(*(maze+row*maxcol+col),outputfile);
			putc('\n',outputfile);
		}
		printf("The path to the exit was found and saved in the file \'maze_exit.txt\'.\n");
		fclose(outputfile);
		system("PAUSE");
		exit(0);
	}
}