#include <ncurses.h>
#include <string.h>
#include "tilemap.h"
#include "being.h"
#include "colorpairs.h"

static const NUMBER_OF_BEINGS = 1;

struct tilemap tilemap;
struct being player;
struct being beings[1];
char *areas[2][2];
int area_x, area_y;

void display_tilemap() {
  int x, y;
  for (y = 0; y < 15; y++) {
    for (x = 0; x < 50; x++) {
      struct tile tile = get_tile(tilemap, x, y);
      attron(COLOR_PAIR(tile.color_pair));
      mvaddch(y, x, tile.character);
      attroff(COLOR_PAIR(tile.color_pair));
    }
  }
}

void display_beings() {
  int i;
  for (i = 0; i < NUMBER_OF_BEINGS; i++) {
    attron(COLOR_PAIR(beings[i].color_pair));
    mvaddch(beings[i].y, beings[i].x, beings[i].character);
    attroff(COLOR_PAIR(beings[i].color_pair));
  }
}

void display_dialog(struct being being) {
  char dialog_string[240];
  strcpy(dialog_string, being.name);
  strcat(dialog_string, ": ");
  strcat(dialog_string, being.dialog);
  attron(COLOR_PAIR(WHITE_ON_BLACK));
  mvprintw(21, 0, dialog_string);
  attroff(COLOR_PAIR(WHITE_ON_BLACK));
  getch();
  move(21, 0);
  clrtobot();
}

void load_area(int delta_x, int delta_y) {
  tilemap = create_tilemap(areas[area_y + delta_y][area_x + delta_x]);

  display_tilemap();

  area_x += delta_x;
  area_y += delta_y;

  if (delta_x == 1) {
    player.x = 0;
  } else if (delta_x == -1) {
    player.x = 49;
  }

  if (delta_y == 1) {
    player.y = 0;
  } else if (delta_y == -1) {
    player.y = 14;
  }

  attron(COLOR_PAIR(player.color_pair));
  mvaddch(player.y, player.x, player.character);
  attroff(COLOR_PAIR(player.color_pair));
}

void move_being(struct being *being, int delta_x, int delta_y) {
  int old_x = (*being).x;
  int old_y = (*being).y;
  int new_x = (*being).x + delta_x;
  int new_y = (*being).y + delta_y;
  int i;
  for (i = 0; i < NUMBER_OF_BEINGS; i++) {
    if (being != &beings[i] && beings[i].x == new_x && beings[i].y == new_y) {
      if (being == &player) {
        //if (beings[i].dialog != "") {
          display_dialog(beings[i]);
	  //}
      }
      return;
    }
  }
  struct tile old_tile = get_tile(tilemap, old_x, old_y);
  struct tile new_tile = get_tile(tilemap, new_x, new_y);
  if (new_tile.walkable == WALKABLE) {
    (*being).old_x = old_x;
    (*being).old_y = old_y;
    (*being).x = new_x;
    (*being).y = new_y;
    attron(COLOR_PAIR((*being).color_pair));
    mvaddch(new_y, new_x, (*being).character);
    attroff(COLOR_PAIR((*being).color_pair));
    attron(COLOR_PAIR(old_tile.color_pair));
    mvaddch(old_y, old_x, old_tile.character);
    attroff(COLOR_PAIR(old_tile.color_pair));
  }
}

int main() {
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);

  start_color();
  init_pair(YELLOW_ON_BLACK, COLOR_YELLOW, COLOR_BLACK);
  init_pair(WHITE_ON_BLACK, COLOR_WHITE, COLOR_BLACK);
  init_pair(GREEN_ON_RED, COLOR_GREEN, COLOR_RED);
  init_pair(YELLOW_ON_RED, COLOR_YELLOW, COLOR_RED);
  init_pair(RED_ON_GREEN, COLOR_RED, COLOR_GREEN);
  init_pair(YELLOW_ON_GREEN, COLOR_YELLOW, COLOR_GREEN);
  init_pair(WHITE_ON_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(BLACK_ON_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(RED_ON_YELLOW, COLOR_RED, COLOR_YELLOW);
  init_pair(GREEN_ON_YELLOW, COLOR_GREEN, COLOR_YELLOW);
  init_pair(WHITE_ON_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(WHITE_ON_WHITE, COLOR_WHITE, COLOR_WHITE);

  areas[0][0] = "startmap.txt";
  areas[0][1] = "soultreemap.txt";

  area_x = 0;
  area_y = 0;

  tilemap = create_tilemap("startmap.txt");

  display_tilemap();

  player = create_being("Sapphire", "That's you, dummy.", "", '@', 1,
			1, WHITE_ON_BLACK);

  /*beings[0] = create_being("Wise Man",
			  "Over the years, this man has collected all of the "
                          "wisdom.\n",
			  "Talk to The Soul Tree to the east. It is your "
                          "destiny.",
			  '@',
			  20,
			  3,
			  YELLOW_ON_BLACK);*/
  
  attron(COLOR_PAIR(player.color_pair));
  mvaddch(player.y, player.x, player.character);
  attroff(COLOR_PAIR(player.color_pair));

  //display_beings();

  int ch;
  while (ch != 113) {
    if (player.x == 49) {
      load_area(1, 0);
    } else if (player.x == 0) {
      load_area(-1, 0);
    }

    if (player.y == 14) {
      load_area(0, 1);
    } else if (player.y == 0) {
      load_area(0, -1);
    }
    ch = getch();
    int delta_x = 0;
    int delta_y = 0;
    if (ch == KEY_UP) {
      delta_y = -1;
    }
    if (ch == KEY_DOWN) {
      delta_y = 1;
    }
    if (ch == KEY_LEFT)
    {
      delta_x = -1;
    }
    if (ch == KEY_RIGHT) {
      delta_x = 1;
    }
    move_being(&player, delta_x, delta_y);
  }
  curs_set(1);
  endwin();

  return 0;
}
