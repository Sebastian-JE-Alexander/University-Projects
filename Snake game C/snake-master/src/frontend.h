#include <ncurses.h>
//Handle keyboard commands in-game
enum Direction get_next_move(enum Direction previous);
//print character symbol in a list of points
void display_points(PointList* snake, const chtype symbol);
