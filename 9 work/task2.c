#include "file_operation.h"

#define PANEL1 0
#define PANEL2 1

void handle_input(int ch, int *current_panel, char *paths[2],
                  int highlight[2]) {
  switch (ch) {
  case KEY_UP:
    highlight[*current_panel]--;
    if (highlight[*current_panel] < 0) {
      highlight[*current_panel] = 0;
    }
    break;
  case KEY_DOWN:
    highlight[*current_panel]++;
    break;
  case '\t':
    if (*current_panel == PANEL1) {
      *current_panel = PANEL2;
    } else {
      *current_panel = PANEL1;
    }
    break;
  case '\n': {
    DIR *dir;
    struct dirent *entry;
    int index = 0;
    char new_path[1024];
    snprintf(new_path, sizeof(new_path), "%s", paths[*current_panel]);
    dir = opendir(new_path);
    if (dir == NULL) {
      perror("opendir");
      return;
    }
    while ((entry = readdir(dir)) != NULL) {
      if (index == highlight[*current_panel]) {
        if (strcmp(entry->d_name, "..") == 0) {
          chdir("..");
          getcwd(new_path, sizeof(new_path));
        } else if (entry->d_type == DT_DIR) {
          snprintf(new_path, sizeof(new_path), "%s/%s", paths[*current_panel],
                   entry->d_name);
          if (chdir(new_path) == 0) {
            getcwd(new_path, sizeof(new_path));
          } else {
            perror("chdir");
          }
        }
        strncpy(paths[*current_panel], new_path, sizeof(new_path));
        closedir(dir);
        highlight[*current_panel] = 0;
        return;
      }
      index++;
    }
    closedir(dir);
  } break;
  }
}

void print_directory(const char *path, WINDOW *win, int highlight) {
  DIR *dir;
  struct dirent *entry;
  werase(win);
  dir = opendir(path);
  if (dir == NULL) {
    perror("opendir");
    return;
  }
  int row = 0;
  int selected = 0;
  while ((entry = readdir(dir)) != NULL) {
    if (selected == highlight) {
      wattron(win, A_REVERSE);
    }
    mvwprintw(win, row++, 0, "%s", entry->d_name);
    if (selected == highlight) {
      wattroff(win, A_REVERSE);
    }
    selected++;
  }
  closedir(dir);
  wrefresh(win);
}

void update_panels(WINDOW *panel1, WINDOW *panel2, char *paths[2],
                   int highlight[2], int current_panel) {
  if (current_panel == PANEL1) {
    print_directory(paths[PANEL1], panel1, highlight[PANEL1]);
    wrefresh(panel1);
  } else {
    print_directory(paths[PANEL2], panel2, highlight[PANEL2]);
    wrefresh(panel2);
  }
}

int main() {
  char current_path1[1024];
  char current_path2[1024];
  getcwd(current_path1, sizeof(current_path1));
  getcwd(current_path2, sizeof(current_path2));
  int current_panel = PANEL1;
  char *paths[2] = {current_path1, current_path2};
  int ch;
  int highlight[2] = {0, 0};

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  WINDOW *panel1 = newwin(LINES / 2, COLS / 2, 0, 0);
  WINDOW *panel2 = newwin(LINES / 2, COLS / 2, 0, COLS / 2);

  update_panels(panel1, panel2, paths, highlight, current_panel);

  while ((ch = getch()) != 'q') {
    handle_input(ch, &current_panel, paths, highlight);
    update_panels(panel1, panel2, paths, highlight, current_panel);
  }

  delwin(panel1);
  delwin(panel2);
  endwin();

  return 0;
}
