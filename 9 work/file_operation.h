#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <dirent.h>

void write_message();
void read_message();
void handle_input(int ch, int *current_panel, char *paths[2], int highlight[2]);
void print_directory(const char *path, WINDOW *win, int highlight);
void update_panels(WINDOW *panel1, WINDOW *panel2, char *paths[2], int highlight[2], int current_panel);

#endif
