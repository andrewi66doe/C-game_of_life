#define LIVE 1
#define DEAD 0

int ret_neighbors(char* grid, int dim_x, int dim_y, int x, int y);
void calc_next_gen(char* curr_gen, char* next_gen, int dim_x, int dim_y);
void display_grid(char* grid, int dim_x, int dim_y, int x, int y, int color);
