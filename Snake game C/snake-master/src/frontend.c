#include <ncurses.h>
#include "backend.h"
#include "frontend.h"

//print character "symbol" in a list of points
void display_points(PointList* snake, const chtype symbol) {
  while(snake) { //while list not NULL
    //print symbol in point position
    mvaddch(snake->y, snake->x, symbol);
	//point to next point in the list
    snake = snake->next;
  }
}

//Handle keyboard commands in-game
enum Direction get_next_move(enum Direction previous) {
  int ch = getch();
  switch (ch) {
    case KEY_LEFT:
      if (previous != RIGHT) return LEFT;
	  else return previous; //Avoid entering remaining cases
    case KEY_RIGHT:
      if (previous != LEFT) return RIGHT;
  	  else return previous; //Avoid entering remaining cases
    case KEY_DOWN:
      if (previous != UP) return DOWN;
	  else return previous; //Avoid entering remaining cases 
    case KEY_UP:
      if (previous != DOWN) return UP;
    default: //nothing pressed or non-direction key pressed
      return previous;
  }
}


