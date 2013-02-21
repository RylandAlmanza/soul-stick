#include <ncurses.h>
#include "tilemap.h"
#include "being.h"
#include "colorpairs.h"

void display_tilemap(struct tilemap tilemap) {
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

void move_being(struct being *being, int delta_x, int delta_y,
		struct tilemap tilemap) {
  int old_x = (*being).x;
  int old_y = (*being).y;
  int new_x = (*being).x + delta_x;
  int new_y = (*being).y + delta_y;
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

  struct tilemap tilemap = create_tilemap("map.txt");

  display_tilemap(tilemap);

  struct being player = create_being("Sapphire", "That's you, dummy.", '@', 1,
				     1, WHITE_ON_BLACK);
  attron(COLOR_PAIR(player.color_pair));
  mvaddch(player.y, player.x, player.character);
  attroff(COLOR_PAIR(player.color_pair));

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
    move_being(&player, delta_x, delta_y, tilemap);
  }
  curs_set(1);
  endwin();

  return 0;
}
