//1730ed Joshua Montag Aniket Shinde
#include <string>
#include <iostream>
#include <cstdlib>
#include <climits>
#include <ncurses.h>
#include <menu.h>
#include <stdlib.h>
#include <cstring>
#include <sstream>
#include <vector>
#include <fstream>
#include <iterator>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4;

using namespace std;

void load(char *arg);
void printFile(FILE *fp);
void load();
WINDOW *createWindow(int height, int width, int starty, int startx);
void makeMenu();
void print_in_middle(WINDOW *win, int starty, int startx, int width, char const * string);
void save(char * name);

WINDOW *myWin;
FILE *fp;
int height,width;
char *fileName;
int starty;
int startx;
bool isSaved=true;

/**
 *Main Entry point for program
 *@param argc amount of arguments in command line
 *@param *argv array of char arguments in command line1
 *Initializes windows, handles key input for text editor
 */
int main(int argc, char *argv[]){
  int xCursor=1;
  int yCursor=1;
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  printw("F1: Menu \t F2: Close Menu");

  //Color! for bonus
  start_color();
  init_pair(1,COLOR_GREEN,COLOR_BLACK);
  attron(COLOR_PAIR(1));  
  mvwprintw(stdscr, 1, (COLS-8)/2,"CSCI 1730 EDITOR!");
  attroff(COLOR_PAIR(1));
  
  height = LINES - 4;
  width = COLS - 10;
  starty = (LINES - height) / 2;/* Calculating for a center placement */
  startx = (COLS - width) / 2;/* of the window*/
  refresh();
  myWin = createWindow(height, width, starty, startx);
  keypad(myWin, TRUE);
    
  load(argv[1]);//loads file
  refresh();
  wrefresh(myWin);

  int key;
  do{
    key = getch();
    if(key== KEY_F(1)){//opens menu
      makeMenu();
      wmove(myWin,1,1);
      wrefresh(myWin);
      refresh();
    }
    else if(key == KEY_F(2)){}
    else if(key == KEY_RESIZE){//resize 
      refresh();
      wrefresh(myWin);
    }
    else if(key == 10 ){//handles enter
      
      string temp="";
      
      for(int i=xCursor;i<width-1;i++){
	signed int ui =mvwinch(myWin,yCursor,i);
	if(ui!=EOF) 
	  temp.push_back(mvwinch(myWin,yCursor,i));
      }//for

      //mvwinsch(myWin,yCursor,xCursor,'\n');
      wmove(myWin,yCursor+1,1);
      yCursor++;
      xCursor=1;
      wrefresh(myWin);
    }
    else if(key == KEY_BACKSPACE){//handles backspace
      isSaved=false;
      mvwdelch(myWin,yCursor, xCursor-1);
      xCursor--;
      wrefresh(myWin);
    }
    //arrow key handlers
    else if(key == KEY_RIGHT){
      wmove(myWin,yCursor,xCursor+1);
      xCursor++;
      wrefresh(myWin);
    }
    else if(key == KEY_LEFT){
      wmove(myWin,yCursor,xCursor-1);
      xCursor--;
      wrefresh(myWin);
    }
    else if(key == KEY_UP){
      wmove(myWin,yCursor-1,xCursor);
      yCursor--;
      wrefresh(myWin);
    }
    else if(key == KEY_DOWN){
      wmove(myWin,yCursor+1,xCursor);
      yCursor++;
      wrefresh(myWin);
    }
    else{//prints out other keys
      isSaved=false;
      echo();
      wrefresh(myWin);
      mvwinsch(myWin,yCursor,xCursor,key);
      xCursor++;
      wrefresh(myWin);
      noecho();
    }//else
    key=0;
  }while(true);
  
  endwin();
  fclose(fp);
  return 0;
  
}

/*
 *Loads a specified file
 *Creates a window and border.
 *Calls method to fill 
 *@param *arg the name of input file
 */
void load(char *inputName){

  if(fopen(inputName,"r")==NULL){ //if file is invalid print out error message
    
    int height = 5;
    int width = 30;
    int starty = (LINES - height) / 2;/* Calculating for a center placement */
    int startx = (COLS - width) / 2;/* of the window*/

    WINDOW *errWin = createWindow(height,width,starty,startx);//creates error window
    mvwprintw(errWin,1,1,"File does not exist!\n Press Any Key!");
    wrefresh(errWin);
    getch();
    wclear(errWin);
    wrefresh(errWin);
    delwin(errWin);
    wrefresh(myWin);
    refresh();
    if(fopen(fileName,"r")==NULL){//if last file cant be reached, exits
      WINDOW *escWin = createWindow(height,width,starty,startx);
      mvwprintw(escWin,1,1,"Cannot find previous file.\n Exiting...");
      wrefresh(escWin);
      getch();
      endwin();
      exit(1);
    }//if fileName==null
    else
      load(fileName);//loads last file if possible
  }//if fill does not exist

  else{//loads normally
    fileName=inputName;
    fp = fopen(inputName,"r");
    printFile(fp);//prints the contents
    move(LINES-1,1);
    wclrtoeol(stdscr);
    mvwprintw(stdscr,LINES-1,1,fileName);
    wmove(myWin,1,1);
    wrefresh(myWin);
  }//else (normal load)
}//load

/**
 *Prints the contents of the file 
 *@param  *fp the file inputted to read
 */
void printFile(FILE *fp){
  int ch;
  int y, x;
  
  wclear(myWin);
  myWin = createWindow(height, width, starty, startx);
  wrefresh(myWin);
  //getmaxyx(myWin, row, col);/* find the boundaries of the screeen */
  wmove(myWin,1,1);
  int xCursor2 =1;
  int yCursor2 =1;
  
  //Loop that reads the file. Taken from the attributes page of the ncurses documentation
  while((ch = fgetc(fp)) != EOF)/* read the file till we reach the end */
    {
      getyx(myWin, y, x);/* get the current curser position */
      if(y == (height - 1))/* are we are at the end of the screen */
	{
	  printw("<-Press Any Key Except Arrow Keys->");/* tell the user to press a key */
	  getch();
	  wclear(myWin);/* clear the screen */
	  wmove(myWin,1,1);/* start at the beginning of the screen */
	  xCursor2 =1;
	  yCursor2 =1;
	}
      else if(ch=='\n'){//enter moves down and resets
	yCursor2++;
	xCursor2 =1;
      }
      else{//prints
	mvwprintw(myWin,yCursor2,xCursor2,"%c", ch);
	xCursor2++;
      }
      refresh();
      wrefresh(myWin);
    }
  refresh();
  wrefresh(myWin);
}//printFile

/**
 *Creates a window along with the frame of specified size
 *@param height is the height of the window
 *@param width is the width of the window
 *@param starty is the y position of the window
 *@param startx is the x position of the window
 *@return returns the window created
 */
WINDOW *createWindow(int height, int width, int starty, int startx){

  WINDOW *tempWin;
  tempWin = newwin(height, width, starty, startx);
  box(tempWin, 0,0);
  wrefresh(tempWin);
  return tempWin;
}//createWindow

/**
 *Makes the menu for the program with handling for each option:
 *open, save, save as, and exit
 **/
void makeMenu(){
  noecho();
  const  char *choices[] = { "Open",    "Save",    "Save As",      "Exit",  };
  ITEM **my_items;
  int c;
  MENU *my_menu;
  int n_choices, i;
  WINDOW *menuWindow;

  //taken from attributes
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
  
  for(i = 0; i < n_choices; ++i)
    my_items[i] = new_item(choices[i],'\0');

  my_menu = new_menu((ITEM **)my_items);
  menuWindow = newwin(10, 40, 6, 6);
  keypad(menuWindow, TRUE);
  set_menu_win(my_menu, menuWindow);
  set_menu_sub(my_menu, derwin(menuWindow,6,38,3,1));
  set_menu_mark(my_menu, " * ");

  //Sets border
  box(menuWindow, 0, 0);
  print_in_middle(menuWindow, 1, 0, 40, "Menu");
  mvwaddch(menuWindow, 2, 0, ACS_LTEE);
  mvwhline(menuWindow, 2, 1, ACS_HLINE, 38);
  mvwaddch(menuWindow, 2, 39, ACS_RTEE);
  refresh();

  //posts the menu in correct window
  post_menu(my_menu);
  refresh();
  wrefresh(menuWindow);

  //declaring booleans needed for handling
  char temp[80]="";
  bool shouldILoad =false;
  bool shouldISave=false;
  bool shouldISaveAs=false;

  //do while opens menu and handles choices
  do{
    c = wgetch(menuWindow);
    switch(c){
    case KEY_DOWN://moves highlight down
      menu_driver(my_menu, REQ_DOWN_ITEM);
      break;
    case KEY_UP://moves highlight up
      menu_driver(my_menu, REQ_UP_ITEM);
      break;
    case KEY_RESIZE://resizes
      refresh();
      wrefresh(myWin);
      wrefresh(menuWindow);
      break;
    case 10://enter button
      
      if(item_name(current_item(my_menu))==choices[3]){//exit
	if(isSaved==false){
	  char answer;
	  wmove(menuWindow,1,1);
	  wclrtoeol(menuWindow);
	  mvwprintw(menuWindow,1,1,"Would you like to save (Y/N): ");//prompt
	  echo();
	  wrefresh(menuWindow);
	  wmove(menuWindow,1,30);
	  answer =wgetch(menuWindow);//only handles char inputs
	  noecho();
	  if(answer=='y'||answer=='Y'){//extra save if yes
	    refresh();
	    save(fileName);
	    isSaved=true;
	  }//answer yes

	}

        //takes away menu and terminates the program
	unpost_menu(my_menu);
	free_menu(my_menu);
	for(i = 0; i < n_choices; ++i)
	  free_item(my_items[i]);
	endwin();
	exit(1);
      }//if enter on exit

      if(item_name(current_item(my_menu))==choices[0]){//open
	if(isSaved==false){//if the file was edited before saving, user is prompted
	  char answer;
	  wmove(menuWindow,1,1);
	  wclrtoeol(menuWindow);
	  mvwprintw(menuWindow,1,1,"Would you like to save (Y/N): ");//prompt
	  echo();
	  wrefresh(menuWindow);
	  wmove(menuWindow,1,30);
	  answer =wgetch(menuWindow);//only handles char inputs
	  noecho();
	  if(answer=='y'||answer=='Y'){//extra save if yes
	    refresh();
	    save(fileName);
	    isSaved=true;
	  }//answer yes
	}//ifisSavedFalse
	
	wmove(menuWindow,1,1);
	wclrtoeol(menuWindow);
	mvwprintw(menuWindow,1,1,"File Name: ");//prompts file name
	echo();
	wrefresh(menuWindow);
	wmove(menuWindow,1,12);	       
       	wgetstr(menuWindow,temp);//reads in filename
	noecho();
	wmove(myWin,1,1);
	wrefresh(myWin);
	refresh();
	fclose(fp);//closes old file
	wclear(myWin);
	wrefresh(myWin);
	shouldILoad=true;
	wrefresh(myWin);
	//fileName=temp;//setting global var
	c = KEY_F(2);//break out of while
      }//open is selected

      if(item_name(current_item(my_menu))==choices[2]){//saveAs
	isSaved=true;
	wmove(menuWindow,1,1);
	wclrtoeol(menuWindow);
	mvwprintw(menuWindow,1,1,"File Name: ");//prompts file name
	echo();
	wrefresh(menuWindow);
	wmove(menuWindow,1,12);	       
       	wgetstr(menuWindow,temp);
	noecho();
	wmove(myWin,1,1);
	wrefresh(myWin);
	shouldISaveAs=true;
	//shouldILoad=true;
	c = KEY_F(2);//breaks out of while
      }//save as is selevted

      if(item_name(current_item(my_menu))==choices[1]){//save
	//fileName=temp;
	//shouldILoad=true;
	
	isSaved=true;
	wmove(myWin,1,1);
	wrefresh(myWin);
	shouldISave=true;
	c = KEY_F(2);//breaks out of while
      }//save is selected

      break;
    }//SWITCH
    
  }while(c!=KEY_F(2));//keeps in menu until f2 is pressed

  c=0;//resets character
  noecho();
  keypad(menuWindow,FALSE);//listener off
  wmove(myWin,1,1);
  wrefresh(myWin);
  //free up memory
  unpost_menu(my_menu);  
  free_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  free (my_items);
  wclear(menuWindow);
  wrefresh(menuWindow);
  wdelch(menuWindow);
  wrefresh(menuWindow);
  delwin(menuWindow);
  
  //load/save 
  if(shouldILoad){load(temp);}
  if(shouldISave){save(fileName);}
  if(shouldISaveAs){save(temp);}

  shouldILoad=false;
  shouldISave=false;
  shouldISaveAs=false;
  //load(fileName);
  //    fileName=temp;
  wrefresh(myWin);
  refresh();
}

/**
 *Saves the file to the input file name
 *@param name of new file to be saved
 **/
void save(char * name){
  stringstream ss;
  string temp;

  vector <string> vt;

  //putting all characters on a line into a string
  for(int i=1;i<height-1;i++){
    for(int j=1;j<width-1;j++){
      signed int si=mvwinch(myWin,i,j);
       if(si!=EOF) 
	 temp.push_back(mvwinch(myWin,i,j));
    }//forj
    vt.push_back(temp);//string into vector
    temp="";
  }//fori
  
  //creating new file and writing the vector to it
  ofstream output_file(name);
  ostream_iterator<string> output_iterator(output_file, "\n");
  copy(vt.begin(), vt.end(), output_iterator);

  fileName=name;

  //changing name at the bottom of the screen
  move(LINES-1,1);
  wclrtoeol(stdscr);
  mvwprintw(stdscr,LINES-1,1,fileName);
  wmove(myWin,1,1);
  wrefresh(myWin);

  wrefresh(myWin);
  refresh();
}//save

/**
 *Print menu into the center of the box
 *@param win is the window used
 *@param starty is the y position
 *@param startx is the x position
 *@param width is the width of the box
 *@string is the string used
**/
void print_in_middle(WINDOW *win, int starty, int startx, int width, char const* string){
  int length, x, y;
  float temp;

  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  //halving
  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  mvwprintw(win, y, x, "%s", string);
  refresh();
}//print in middle





