/**
PROJECT 2
**************

File: methods1.h

Date: 3/27/2017

Authors: Ryan Joiner (810638856)
         Emma Fick   (810609119)

Prof. Michael Cotterell
**************

<NEED DESCRIPTION>

This is the supporting method file for Project2.

<NEED DESCRIPTION>
**/

#ifndef METHODS1_H
#define METHODS1_H

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

//These functions deal with IO
bool file_exists(string filename);
bool save_file(string filename, string data);
string load_file(const char * fileptr);
bool close_file(int &fd);
string get_filename(const char * fileptr);


/**Test if file exists using stat
   true if exists, false if not
*/
bool file_exists(string filename){
  struct stat buf;
  return((stat(filename.c_str(), &buf)) == 0);
}


/**Saves file with specified filename and contents s
   Returns true if successful and false otherwise
   Creates a new, blank file and writes the string to it
*/
bool save_file(string filename, string s){
  bool success = true;
  int fd = open(filename.c_str(), O_RDWR| O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  if(fd < 0){
    success = false;
  } else {
    string data = s;
    int n = s.size();
    char * buffer = new char[n];
    buffer = (char*)data.c_str();
    if(write(fd, buffer, n)< 0){
      success = false;
    }
  }
  return success;
}


/**Loads file specified by filename
   Returns a string representing the file contents
 */
string load_file(string filename){
  string data;
  int fd = open(filename.c_str(), O_RDWR | O_APPEND);
  if(fd < 0){
    if(filename == "<N/A>"){
      return "";
    } else {
      data = "LOAD_ERR";
    }
  }
  else{
    const int BUFF_SIZE = 1024;
    char buffer[BUFF_SIZE];
    int n = 0;
    data = "";
    while((n = read(fd, buffer, BUFF_SIZE)) > 0){
      for(int i = 0; i < n; i++){
        data += buffer[i];
      }
    }
    if(n < 0){
      return "LOAD_ERR";
    }else {
    return data;
    }
  }
}

/* Closes file specified by filename
   Returns whether this was successful
*/
bool close_file(string filename){
  return((close(*filename.c_str())) != -1);
}


/*Uses pointer to access filename
  Returns string of filename if pointer not null, and "<N/A>" otherwise
*/
string get_filename(const char * fileptr){
  if (fileptr == nullptr){
    return "<N/A>";
  }
  else {
    string fn = fileptr;
    return fn;
  }
}


#endif
