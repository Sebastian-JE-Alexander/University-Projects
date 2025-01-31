#include "backend.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

/*
 * Removes from the list or returns false
 */
//Searches for element elt in the list 
//and removes it (updating the list pointers)
//As the original list is modified, the function accesses it
//via pointer (thus the **list)
bool remove_from_list(PointList* elt, PointList** list) {
  PointList *currP, *prevP;
  prevP = NULL;

  for (currP = *list;
      currP != NULL;
      prevP = currP, currP = currP->next) {
    if (is_same_place(currP, elt)) {
      if (prevP == NULL) {
        *list = currP->next;
      } else {
        prevP->next = currP->next;
      }
      free(currP);
      return true;
    }
  }
  return false;
}

//This function tries to move the full snake in the given direction
//returns FAILURE if a game termination condition arises 
enum Status move_snake(Board* board, enum Direction dir) {
  // Create a new beginning for the snake (head) with next_move
  PointList* beginning = next_move(board, dir);
  if (beginning == NULL) { //wall hit, next_move returned NULL
    return FAILURE;
  }

  // If we've gone backwards, don't do anything
  if (board->snake->next && is_same_place(beginning, board->snake->next)) {
    beginning->next = NULL;
    free(beginning);
    return SUCCESS;
  }

  // Check for collisions of snake head with other snake points
  if (list_contains(beginning, board->snake)) {
    return FAILURE; //snake collided with itself!
  }

  // Check for collision with food (snake eats it!)
  if (list_contains(beginning, board->foods)) {
    // Attach the beginning to the rest of the snake
	//as the snake's lenght increases by 1 character
    beginning->next = board->snake;
    board->snake = beginning;
	//remove the eaten food from the food points list
    remove_from_list(beginning, &(board->foods));
	//add a new food at a random location
    add_new_food(board); 

  board->score = board->score + 1; //increment the score count by 1 for each food "eaten"
    return SUCCESS; 
  }
  // No food was eaten, snake keeps its lenght
  // Snake's head is added to the snake point list
  // Attach the beginning to the rest of the snake
  beginning->next = board->snake;
  board->snake = beginning;
  // Cut off the end
  //Remove the last "tail" point of the snake
  PointList* end = board->snake;
  while(end->next->next) { //while not NULL
    end = end->next;
  }
  free(end->next); //release point memory allocation
  end->next = NULL;

  return SUCCESS; //all done for this movement
}

//Checks if two point structures have the same coordinates
bool is_same_place(PointList* cell1, PointList* cell2) {
  return ((cell1->x == cell2->x) && (cell1->y == cell2->y));
}

//Creates and returns a new snake point (the new snake's head position)
//in the corresponding move direction
//If the snake hits the screen limits, it returns NULL instead
PointList* next_move(Board* board, enum Direction dir) {
  PointList* snake = board->snake;
  int new_x = snake->x;
  int new_y = snake->y;
  switch(dir) {
    case UP:
      new_y = snake->y - 1;
      break;
    case DOWN:
      new_y = snake->y + 1;
      break;
    case LEFT:
      new_x = snake->x - 1;
      break;
    case RIGHT:
      new_x = snake->x + 1;
      break;
  }
  //check screen limits (figured walls)
  if (new_x < 0 || new_y < 0 || new_x >= board->xmax || new_y >= board->ymax) {
    return NULL; //Snake hits one of the walls!
  } else { //correct position, create and return new point for the snake's head
    return create_cell(new_x, new_y);
  }
}

//creates a cell in a random location
PointList* create_random_cell(int xmax, int ymax) {
  return create_cell(rand() % xmax, rand() % ymax);
}

//Adds a new food point to the food list in a random location
void add_new_food(Board* board) {
  PointList* new_food;
  //UPDATED by Mario: original code (now commented) was not releasing memory
  // for cells allocated in non-valid positions, they were just ignored (mem leak!)
   bool ok = false;
   while(!ok){
	new_food = create_random_cell(board->xmax, board->ymax);
	if(list_contains(new_food, board->foods) || list_contains(new_food, board->snake))
	  free(new_food); //not a valid location!
    else //valid location for new food
	  ok = true;
   }
  /*do {
    new_food = create_random_cell(board->xmax, board->ymax);
  } while(list_contains(new_food, board->foods) || list_contains(new_food, board->snake));
  */
  //update food list with the newly created food cell
  new_food->next = board->foods;
  board->foods = new_food;
}

// Checks if a given point is contained in a list of points
bool list_contains(PointList* cell, PointList* list) {
  PointList* s = list;
  while (s) { //while not NULL
    if (is_same_place(s, cell)) {
      return true;
    }
    s = s->next; //select next element in the list
  }
  return false;
}
//Creates a new point element dynamically allocating it
//Initializes x and y elements to the ones given
//pointer to next element is set to NULL so far
PointList* create_cell(int x, int y) {
  PointList* cell = malloc(sizeof(*cell));
  cell->x = x;
  cell->y = y;
  cell->next = NULL;
  return cell;
}

//fills in the Board structure from the given lists and dimensions
Board* create_board(PointList* snake, PointList* foods, int xmax, int ymax) {
  Board* board = malloc(sizeof(*board));
  board->foods = foods;
  board->snake = snake;
  board->xmax = xmax;
  board->ymax = ymax;
  return board;
}

//Generates the initial snake (length 2)
PointList* create_snake() {
  //create the initial 2 point elements of the snake
  PointList* a = create_cell(2,3); //head
  PointList* b = create_cell(2,2); //tail
  a->next = b; //point head element to tail element
  return a; //return head element address (start of list)
}

