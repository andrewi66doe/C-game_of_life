/*
 * =====================================================================================
 *
 *       Filename:  curses.c
 *
 *    Description:  The game of life implemented in c and curses
 *
 *        Version:  1.0
 *        Created:  01/16/14 12:06:39
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Andrew Walker (), andrewg.walker@outlook.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define ALIVE 1
#define DEAD 0

int ret_neighbors(char* grid, int dim_x, int dim_y, int x, int y);
void calc_next_gen(char* curr_gen, char* next_gen, int dim_x, int dim_y);
void display_grid(char* grid, int dim_x, int dim_y, int x, int y, int color);



/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  display_grid
 *  Description:  Accepts a char array and it's dimensions along with an x and y 
 *  position for the array state to printed to 
 * =====================================================================================
 */
void display_grid(char* grid, int dim_x, int dim_y, int x, int y, int color)
{
	int xx, yy;
	
	init_pair(1, color, color);	

	for(yy=0;yy<dim_y;yy++){				//For every element in grid if it is zero display nothing else display a character
		for(xx=0;xx<dim_x;xx++){
			if(grid[yy*dim_x + xx] == DEAD){
				mvaddch(xx+x, yy+y, ' ');
			}
			else{
				attron(COLOR_PAIR(1));
				mvaddch(xx+x, yy+y, '1');
				attroff(COLOR_PAIR(1));
				
			}
		}
	}
}
	

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  ret_neighbors
 *  Description:  Accepts a char array, it's dimensions and a position in the array
 *  and then calculates the number of neighbors around a cell that are alive the passed
 *  in array is unchanged
 * =====================================================================================
 */

int ret_neighbors(char* grid, int dim_x, int dim_y, int x, int y)
{
	int y_combos[] = {-1,-1,-1,0,1,1,1,0};
	int x_combos[] = {-1,0,1,1,1,0,-1,-1}; 
	
	int i;
	int r, c;
	int neighbors = 0;

	for(i=0;i<8;i++){
		r = y + y_combos[i];
		c = x + x_combos[i];

		if((r >= 0 && c >= 0) && (r < dim_y && c < dim_x)){
			if(grid[r * dim_x + c])
				neighbors += 1; 
		}
	}
	return neighbors;

}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  calc_next_gen
 *  Description:  Takes two arrays as input curr_gen is used to calculate the next 
 *  generation which is copied into next_gen.
 * =====================================================================================
 */
void calc_next_gen(char* curr_gen, char* next_gen, int dim_x, int dim_y)
{
	assert((sizeof(curr_gen)/sizeof(curr_gen[0])) == (sizeof(next_gen)/sizeof(next_gen[0])));	//throw an error if the two arrays are not the same size
	
	int x,y;
	int neighbors = 0;

	for(y=0;y<dim_y;y++)
	{
		for(x=0;x<dim_x;x++){
			neighbors = ret_neighbors(curr_gen, dim_x, dim_y, x, y);
			if(curr_gen[y * dim_x + x]){
				if(neighbors < 2 || neighbors > 3)
					next_gen[y * dim_x + x] = DEAD;
				else
					next_gen[y * dim_x + x] = ALIVE;
			}else{
				if(neighbors == 3)
					next_gen[y * dim_x + x] = ALIVE;
				else
					next_gen[y * dim_x + x] = DEAD;
			}
			
		}
	}

}	


int main(int argc, char *argv[])
{
	int i, width, height;
	int gen = 1;
	int color = COLOR_GREEN;

	if(argc < 3){
		printf("USAGE: %s <width> <height>\n", argv[0]);
		exit(0);
	}
	
	width = atoi(argv[1]);
	height = atoi(argv[2]);

	char *current_generation = malloc(width * height);			    //Arrays of size width*height are allocated for current and next generations
   	 if(current_generation == NULL){
        	printf("Failed to allocate memory\n");
        	exit(1);
    	}

	char *next_generation = malloc(width * height);
	if(next_generation == NULL){
        	printf("Failed to allocate memory\n");
        	exit(1);
    	}
 	
	memset(current_generation, 0, sizeof(current_generation)); 		//memset both malloc'd arrays to zero
	memset(next_generation, 0, sizeof(next_generation)); 

	srand(time(NULL));							                    //Seed for random values	

	for(i=0;i<width*height;i++){						            //For loop initializes the current_gen array with random values between 1 and 0
		current_generation[i] = rand() % 2;
	}
	

	initscr();
	raw();
	noecho();
	start_color();
	timeout(0);

	while(getch() != 'q')
	{
		usleep(50000);				
		if(gen){
			calc_next_gen(current_generation, next_generation, width, height);
			display_grid(next_generation, width, height, 0, 0, color);
			gen = 0;
		}
		else{
			calc_next_gen(next_generation, current_generation, width, height);
			display_grid(current_generation, width, height, 0, 0, color);
			gen = 1;	
		}
		refresh();
	}
	free(current_generation);						                //Free the arrays that hold the generation data
	free(next_generation);
	endwin();	
	return 0;
}
