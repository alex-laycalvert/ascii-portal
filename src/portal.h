#ifndef PORTAL_H_
#define PORTAL_H_

typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void init_level(const int level, const int rows, const int cols,
                char grid[rows][cols]);
void print_grid(const int rows, const int cols, const char grid[rows][cols]);
void clean_grid(const int rows, const int cols, char grid[rows][cols]);
void recheck_aim(const int rows, const int cols, char grid[rows][cols]);
void shoot_portal(const int rows, const int cols, char grid[rows][cols]);
void update_grid(const int rows, const int cols, char grid[rows][cols]);
void move_up(const int rows, const int cols, const char grid[rows][cols]);
void move_down(const int rows, const int cols, const char grid[rows][cols]);
void move_left(const int rows, const int cols, const char grid[rows][cols]);
void move_right(const int rows, const int cols, const char grid[rows][cols]);
void play(const int rows, const int cols, char grid[rows][cols]);

#endif // PORTAL_H_
