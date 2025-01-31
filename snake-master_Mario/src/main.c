// Additional comments by Mario Mata, GCU
// Additional comments by Sebastian Alexander, GCU
#include "backend.h" //core functionalities and data structures
#include "frontend.h" //paint functions and key processing
#include <ncurses.h>

int main() {
 char text[] = "READY PLAYER ONE?";  //create the text to print
 char *t;  //make a pointer 
 char t1[] = "PRESS ENTER TO BEGIN"; //create the text to print
 char str[30]; //for printing score in window
 //ncurses screen initialization
  initscr();
 
  t = text; //initialize the pointer
  while(*t)           //loop through the string 
    {
        
        addch(*t);      // one char to output 
        t++;            // increment the pointer 
        refresh();      // update the screen 
        napms(200);     // delay .2 sec 
    }
  
  move(2,0); //row 3, coloumn 1
  addstr(t1); //print the string
  refresh(); //display the changes
  getch(); //wait for key press to continue

  cbreak();
  noecho();
  keypad(stdscr, TRUE); // make ncurses handle key presses
  curs_set(0); // hide cursor
  timeout(100); //do not keep waiting for keypress longer than this
  
  start_color();    //made a change here for the colouring of objects
  init_pair(1, COLOR_BLACK,COLOR_GREEN);
  init_pair(2, COLOR_YELLOW,COLOR_BLACK); //create the colour sets for later use
  init_pair(3, COLOR_RED,COLOR_WHITE);
  

  int xmax;
  int ymax;
  //get current text screen size
  getmaxyx(stdscr, ymax, xmax); //it is a macro, no & needed in ymax, xmax
  //start snake moving to the right
  enum Direction dir = RIGHT;

  //create snake list, and pass it to create_board.
  //Food list is not passed yet (NULL passed instead).
  Board* board = create_board(create_snake(), NULL, xmax, ymax);
  board->score = 0; //sets the intial score to 0 and clears the variable of previous numbers
  int i;
  for (i = 0; i < 6; i++) {
    add_new_food(board); //add food pieces to the board
  }
  //MAIN LOOP
  //start time
  while(true) {
    
    clear(); //clear screen
    attrset(COLOR_PAIR(3)); //applies the colour to the next line of code
    box(stdscr, ACS_VLINE, ACS_HLINE);  //draws border along screen
    addstr("Try To Get a New Highscore"); //adds text string to top left corner of window
    
	//Draw snake and food lists
    attrset(COLOR_PAIR(1)); //applies the colour 
    display_points(board->snake, ACS_CKBOARD); //sets the character used for the snake
    attrset(COLOR_PAIR(2)); //applies the colour
    display_points(board->foods, ACS_PI); //sets the character used for the food
    refresh(); //paint screen
    dir = get_next_move(dir); //get next move direction
	//update snake direction
    enum Status status = move_snake(board, dir);
    if (status == FAILURE) break; //finish game
  }
  //end time
  //release ncurses resources
  
  getch();
  endwin(); //closes the window
  printf("----------------------GAME OVER----------------------\n"); //prints the text line in the terminal
  printf("score = %d\n", board->score);  //print the score of the player
  return 0; //ends the program
}
