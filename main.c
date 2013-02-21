#include <ncurses.h>
#include "tilemap.h"
#include "being.h"
#include "colorpairs.h"

static const NUMBER_OF_BEINGS = 1;

struct tilemap tilemap;
struct being player;
struct being beings[1];

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

void display_dialog(char *dialog) {
  attron(COLOR_PAIR(WHITE_ON_BLACK));
  mvprintw(21, 0, dialog);
  attroff(COLOR_PAIR(WHITE_ON_BLACK));
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
          display_dialog(beings[i].dialog);
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
  init_pair(WHITE_ON_GREEN, COLOR_WHITE, COLOR_GREEN);
  init_pair(BLACK_ON_YELLOW, COLOR_BLACK, COLOR_YELLOW);
  init_pair(WHITE_ON_BLUE, COLOR_WHITE, COLOR_BLUE);
  init_pair(WHITE_ON_WHITE, COLOR_WHITE, COLOR_WHITE);

  tilemap = create_tilemap("map.txt");

  display_tilemap();

  player = create_being("Sapphire", "That's you, dummy.", "", '@', 1,
			1, WHITE_ON_BLACK);

  beings[0] = create_being("Wise Man",
			  "Over the years, this man has collected all of the "
                          "wisdom.\n",
			  "Talk to The Soul Tree to the east. It is your "
                          "destiny.",
			  '@',
			  20,
			  3,
			  YELLOW_ON_BLACK);
  
  attron(COLOR_PAIR(player.color_pair));
  mvaddch(player.y, player.x, player.character);
  attroff(COLOR_PAIR(player.color_pair));

  display_beings();

  int ch;
  while (ch != 113) {
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
