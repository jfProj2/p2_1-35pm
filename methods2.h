/**
PROJECT 2
**************

File: methods2.h

Date: 3/27/2017

Authors: Ryan Joiner (810638856)
         Emma Fick   (810609119)

Prof. Michael Cotterell
**************

<NEED DESCRIPTION>

This is the supporting method file for Project2.

<NEED DESCRIPTION>
**/

#ifndef METHODS2_H
#define METHODS2_H

//########
//INCLUDES
//########
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <curses.h>
#include <ncurses.h>
#include <menu.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

#include "methods_main.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

//##########
//PROTOTYPES
//##########
using namespace std;

void run_editor(const char * fileptr);
void data_to_screen(string data, WINDOW * win, int h, int w);
string add_char(WINDOW *win, int &x , int &y , int &ux, int &uy, int ch,string data);
string remove_char(string s);

//###################
//METHOD DESCRIPTIONS
//###################

//NEEDS JAVADOC
void run_editor(const char *fileptr){
  WINDOW *term_win;
  WINDOW *edit_win;
  string filename = get_filename(fileptr);
  bool exists = file_exists(fileptr);
  bool saved = false;
  initscr();
  cbreak();
  noecho();
  int term_h, term_w;
  getmaxyx(stdscr, term_h, term_w);
  int menu_h = term_h/2;
  int menu_w = term_w/2;
  int menu_x = (term_w / 4), menu_y = (term_h / 4);
  int edit_x = 1, edit_y = 1;
  int edit_h = (term_h * 0.90);
  int edit_w = (term_w * 0.90);
  term_win = newwin(term_h, term_w, 0, 0);
  edit_win = derwin(term_win, edit_h, edit_w, edit_x, edit_y);

  int ex = 2;
  int ey = 2;
  //FIGURE OUT PADS?
  //edit_win = newpad(edit_h,edit_w);
  
  touchwin(edit_win);
  keypad(term_win, true);
  keypad(edit_win, true);
  refresh();
  mvwprintw(term_win, 0, 0, "F1: Menu/Cancel");
  mvwprintw(term_win, 0, (term_w/2)-8, "CSCI 1730 Editor");
  mvwprintw(term_win, (term_h-1), 0, filename.c_str());

  box(edit_win, 0, 0);
  string text = load_file(filename);
  data_to_screen(text, edit_win, edit_h, edit_w);
  wrefresh(term_win);
  wrefresh(edit_win);
  int key = 1;
  bool running = true;
  string appended = "";
  //Maybe create another set of x,y positions to keep track of
  //where the user has moved the cursor to... for the else if where
  //the user enters a valid key to add to the string we check if the
  //xy pair are the same and if not edit at the new location and set ex/ey
  //to new location
  int ux = 2, uy = 2;
  while(running){
    refresh();
    //bool saved = false;
    key = wgetch(edit_win);
    text = add_char(edit_win,ex, ey, ux, uy, key, text);
    //getyx(edit_win, ey, ex);
    //wmove(edit_win,ey, ex);
    //file_to_screen("", edit_win, edit_h, edit_w); //maybe
    wclear(edit_win);
    box(edit_win, 0, 0);
    refresh();
    wrefresh(term_win);
    touchwin(term_win);
    data_to_screen(text, edit_win, edit_h, edit_w);
    refresh();
    wrefresh(edit_win);
    touchwin(term_win);

    switch(key){
    case KEY_F(1):
      int selection = show_menu_window(menu_h, menu_w, menu_y, menu_x);
      wrefresh(term_win);
      if(selection == 0){
        wrefresh(term_win);
        wrefresh(edit_win);
      }
      else if(selection == 1){
	int savePrompt = 0;
	if(!saved){
	  savePrompt = show_save_window(menu_h, menu_w, menu_y, menu_x);
	  if(savePrompt == 1){
	    save_file(filename, text);
	    saved = true;
	    savePrompt = 0;
	  }
	}
	if(savePrompt == 0){
	  filename = "";
          filename += show_open_window(menu_h, menu_w, menu_y, menu_x);
          text = load_file(filename);
          data_to_screen(text, edit_win, edit_h, edit_w);
	  mvwprintw(term_win, (term_h-1), 0, filename.c_str());
	  refresh();
	  wrefresh(edit_win);
	  touchwin(term_win);
        }
   
        else{  //Cancel, return to current edit
	  wclear(edit_win);
	  box(edit_win, 0, 0);
	  
	   refresh();
	   wrefresh(term_win);
	   touchwin(term_win);
	   data_to_screen(text, edit_win, edit_h, edit_w);

        }
	}
      else if(selection == 2){
	if(strcmp(filename.c_str(), "<N/A>") == 0){
	  filename = "";
	  filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
	}
        save_file(filename, text);
	saved = true;
	data_to_screen(text, edit_win, edit_h, edit_w);
	mvwprintw(term_win, (term_h-1), 0, filename.c_str());
	refresh();
	wrefresh(edit_win);
	touchwin(term_win);
	
      }
      else if(selection == 3){
        filename = "";
        filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
	save_file(filename, text);
	saved = true;
	data_to_screen(text, edit_win, edit_h, edit_w);
	mvwprintw(term_win, (term_h-1), 0, filename.c_str());
	refresh();
	wrefresh(edit_win);
	touchwin(term_win);
      }
      else if(selection == 4){
	if(saved){
	  running = false;
	} else {
	  int savePrompt = show_save_window(menu_h, menu_w, menu_y, menu_x);
	  if(savePrompt == 0){
	    running = false;
	  }
	  else if(savePrompt == 1){
	    if(strcmp(filename.c_str(), "<N/A>") == 0){
	      filename = "";
	      filename += show_saveas_window(menu_h, menu_w, menu_y, menu_x);
	    }
	    save_file(filename, text);
	    saved = true;
	    running = false;
	  }
	  else {
	    wclear(edit_win);
	    box(edit_win, 0, 0);
	   refresh();
	   wrefresh(term_win);
	   touchwin(term_win);
	   data_to_screen(text, edit_win, edit_h, edit_w);
	  }
      }
	
      break;
    }
    wrefresh(term_win);
    wrefresh(edit_win);
  }
  }
  delwin(edit_win);
  delwin(term_win);;
  endwin();
}


//NEEDS JAVADOC
void data_to_screen(string data, WINDOW * win, int height, int width){
  int x = 1;
  int y = 1;
  int w = width - 1;
  for(unsigned int i = 0; i < data.size(); i++){
    char ch = data[i];
    mvwprintw(win, y, x, "%c", ch);
    if(x == (w - 1) || ch == '\n'){
      y++;
      x = 0;
      //mvwprintw(win, x, y,"%c", '\n');
    }
    x++;
  }
  //wmove(win, y, x);
  refresh();
  wrefresh(win);
  touchwin(win);

  //mvwprintw(win, x, y, "%c", '\0');
}

//NEEDS JAVADOC
string add_char(WINDOW * win, int& x, int& y, int &ux, int &uy, int ch, string s){
  //getyx(win, y, x);
  int max_x, max_y;
  getmaxyx(win, max_y, max_x);
  // mvwprintw(win,
  string data = "";
  //int len = s.size();

  //This portion attempts to get the cursor to move to the location that
  //the user has moved the cursor to... this would be useful in editing at
  //user chosen locations within the current file
  /**
  int cur_x, cur_y;
  getyx(win, cur_y, cur_x);
  if(cur_x != x || cur_y != y){
    x = cur_x;
    y = cur_y;
    move(y, x);
    refresh();
  }
  **/

  if(ch == KEY_BACKSPACE|| ch == 127){
    if(s.size() > 0){
      data = remove_char(s);
      if(x == 2 && y > 2){
        x = max_x -1;
        y--;
      }
      else{
        x--;
      }
      //wmove(win, y, x);
    }
  }
  else if(ch == 10){
    if(y < max_y-1){
      y++;
      x = 2;
    }
    data = s + "\n";
  }
  else if(ch == KEY_DOWN){
    if(y < max_y -1){
      y++;
      uy++;
    }
    //wmove(win, y, x);
    data = s;
  } else if(ch == KEY_UP){
    if(y > 2){
      y--;
      uy--;
    }
    //wmove(win, y, x);
    data = s;
  } else if(ch == KEY_LEFT){
    // wmove(win, y, x-1);
    if(x > 2){
      x--;
      ux--;
    }
    //wmove(win, y, x);
    data = s;
  } else if(ch == KEY_RIGHT){
    if(x < max_x - 1){
      x++;
      ux++;
    }
    //wmove(win, y, x);
    data = s;
  } else {
    if(x < max_x -1)
      x++;
    if(x == max_x-1){
      y++;
      x = 2;
    }
    //wmove(win,y,x);
    data = s + (char)ch;
  }
  move(y, x);
  refresh();
  return data;
}

//MIGHT NOT BE NEEDED
//NEEDS JAVADOC
string remove_char(string s){
  /* string data = "";
  for(unsigned int i = 0; i < (s.size() - 1); i++){
    data[i] += s[i];
  }
   data += '\b';
   return data;*/
  string data = s.substr(0,s.size()-1);
  return data;
}


#endif
