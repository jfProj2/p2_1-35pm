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

bool file_exists(const char * fileptr);
void save_file(string filename, string data);
string load_file(const char * fileptr);
bool close_file(int &fd);
string get_filename(const char * fileptr);

//###################
//METHOD DESCRIPTIONS
//###################

//MIGHT NOT BE NEEDED
//NEEDS JAVADOC
bool file_exists(const char * fileptr){
  return(fileptr != nullptr);
}


//NEEDS JAVADOC
void save_file(string filename, string s){
  int fd = open(filename.c_str(), O_RDWR| O_CREAT | O_TRUNC, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
  string data = s;
  int n = s.size();
  char * buffer = new char[n];
  buffer = (char*)data.c_str();
  if(write(fd, buffer, n) == -1){
    //error check here
  }
}


//MORE FUNCTIONALITY WITH ERRORS
//NEEDS JAVADOC
string load_file(string filename){
  int fd = open(filename.c_str(), O_RDWR | O_APPEND /*| O_CREAT, S_IRWXU*/);
  if(fd < 0){
    //int errorNum = errno;
    //error window
    //return int referring to type of error
    return "";
  }
  else{
    const int BUFF_SIZE = 1024;
    char buffer[BUFF_SIZE];
    int n = 0;
    string data = "";
    while((n = read(fd, buffer, BUFF_SIZE)) > 0){
      //write?
      for(int i = 0; i < n; i++){
        data += buffer[i];
      }
    }
    return data;
  }
}

//ERROR CHECK FUNCTIONALITY
//NEEDS JAVADOC
bool close_file(int &fd){
  fd = close(fd);
  if(fd < 0){
    int errorNum = errno;
    printf("\nclose_file: fd: %i: Error.\n", fd);
    printf("errno: %i - %s\n\n",errorNum, strerror(errorNum));
    return false;
  }
  return true;
}


//MIGHT NOT BE NEEDED
//NEEDS JAVADOC
string get_filename(const char * fileptr){
  if (fileptr == nullptr){
    return "Untitled";
  }
  else {
    string fn = fileptr;
    return fn;
  }
}


#endif
