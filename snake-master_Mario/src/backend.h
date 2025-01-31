#include <stdbool.h> //bool type (uchar) and true/false defines

//enum to handle movement directions
enum Direction { UP, DOWN, LEFT, RIGHT };
enum Status { SUCCESS, FAILURE };
//Simply linked list of (x,y) points 
// x, y are text screen coordinates
// (0, 0) is top-left corner
// x = column
// y = row
struct PointList {
  int x;
  int y;
  struct PointList* next;
};

typedef struct PointList PointList;

//struct containing the full game board info
typedef struct {
  PointList* snake; //list of snake points
  PointList* foods; //list of food points
  int xmax; //screen cols
  int ymax; // screen rows
  int score; //add counter for scoring 
} Board;

//Checks if two point structures have the same coordinates
bool is_same_place(PointList* cell1, PointList* cell2);
//This function tries to move the full snake in the given direction
enum Status move_snake(Board* board, enum Direction dir);
//Creates and returns a new snake point (the new snake's head position)
PointList* next_move(Board* board, enum Direction dir);
//Creates a new point element dynamically allocating it
PointList* create_cell(int x, int y);
//creates a cell in a random location
PointList* create_random_cell(int xmax, int ymax);
//Generates the initial snake (length 2)
PointList* create_snake();
//fills in the Board structure from the given lists and dimensions
Board* create_board(PointList* foods, PointList* snake, int xmax, int ymax);
// Checks if a given point is contained in a list of points
bool list_contains(PointList* cell, PointList* list);
//remove element elt from list, update list pointer
bool remove_from_list(PointList* elt, PointList** list);
//Adds a new food point to the food list in a random location
void add_new_food(Board* board);
