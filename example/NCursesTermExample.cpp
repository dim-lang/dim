// Copyright 2019- <fastype.org>
// Apache License Version 2.0

#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ncurses.h>
#include <string>
#include <thread>
#include <unordered_map>

int main(int argc, char *argv[]) {
  int ch, prev, row, col;
  prev = EOF;
  FILE *fp;
  int y, x;

  if (argc != 2) {
    printf("Usage: %s <a c file name>\n", argv[0]);
    exit(1);
  }
  fp = fopen(argv[1], "r");
  if (fp == NULL) {
    perror("Cannot open input file");
    exit(1);
  }
  initscr();                      /* Start curses mode */
  getmaxyx(stdscr, row, col);     /* find the boundaries of the screeen */
  while ((ch = fgetc(fp)) != EOF) /* read the file till we reach the end */
  {
    getyx(stdscr, y, x); /* get the current curser position */
    if (y == (row - 1))  /* are we are at the end of the screen */
    {
      printw("<-Press Any Key->"); /* tell the user to press a key */
      getch();
      clear();    /* clear the screen */
      move(0, 0); /* start at the beginning of the screen */
    }
    printw("%c", ch);
    refresh();
    prev = ch;
  }
  endwin(); /* End curses mode */
  fclose(fp);
  return 0;
}
