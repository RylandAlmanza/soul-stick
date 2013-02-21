#include <stdio.h>
#include "tilemap.h"
#include "colorpairs.h"

struct tile create_tile(char *name, char *description, int x, int y,
                        walkable_t walkable, char character, int color_pair) {
  struct tile tile;
  tile.name = name;
  tile.description = description;
  tile.x = x;
  tile.y = y;
  tile.character = character;
  tile.walkable = walkable;
  tile.color_pair = color_pair;

  return tile;
}

struct tile create_water_tile(int x, int y) {
  return create_tile("Water", "It's water. Swim swim.", x, y, UNWALKABLE, '~',
	      WHITE_ON_BLUE);
}

struct tile create_bridge_tile(int x, int y) {
  return create_tile("Bridge",
		     "It's a bridge. You use it to walk over things.", x, y,
		     WALKABLE, '=', BLACK_ON_YELLOW);
}

struct tile create_grass_tile(int x, int y) {
  return create_tile("Grass", "Little blades of plant.", x, y, WALKABLE, '.',
	      WHITE_ON_GREEN);
}

struct tile create_wall_tile(int x, int y) {
  return create_tile("Wall",
		     "Walls. Always keeping you out of places. Fuck 'em.", x,
		     y, UNWALKABLE, '#', WHITE_ON_WHITE);
}

struct tilemap create_tilemap(char *map_location) {
  FILE *map_file;
  char char_buffer;

  map_file = fopen(map_location, "r");
  struct tilemap tilemap;
  
  int x=0, y=0;
  char_buffer = getc(map_file);
  
  while (char_buffer != EOF) {
    if (char_buffer == '.') {
      tilemap.matrix[y][x] = create_grass_tile(x, y);
    } else if (char_buffer == '~') {
      tilemap.matrix[y][x] = create_water_tile(x, y);
    } else if (char_buffer == '=') {
      tilemap.matrix[y][x] = create_bridge_tile(x, y);
    } else if (char_buffer == '#') {
      tilemap.matrix[y][x] = create_wall_tile(x, y);
    }

    x++;
    char_buffer = getc(map_file);

    if (x == 50) {
      char_buffer = getc(map_file);
      y++;
      x = 0;
    }
  }

  fclose(map_file);
  return tilemap;
}

struct tile get_tile(struct tilemap tilemap, int x, int y) {
  return tilemap.matrix[y][x];
}

void print_map(struct tilemap tilemap) {
  char map_string[751];
  
  int x, y;
  for (y = 0; y < 15; y++) {
    for (x = 0; x < 50; x++) {
      map_string[(y*50)+x] = tilemap.matrix[y][x].character;
    }
  }
  map_string[750] = 0;

  printf("%s", map_string);
}
