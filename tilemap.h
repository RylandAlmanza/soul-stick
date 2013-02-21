#ifndef TILEMAP_H_
#define TILEMAP_H_

typedef enum {
  WALKABLE,
  UNWALKABLE
} walkable_t;

struct tile {
  char *name;
  char *description;
  int x;
  int y;
  walkable_t walkable;
  char character;
  int color_pair;
};

struct tilemap {
  struct tile matrix[15][50];
};

struct tile create_tile(char *name, char *description, int x, int y,
			walkable_t walkable, char character, int color_pair);

struct tile create_water_tile(int x, int y);

struct tile create_bridge_tile(int x, int y);

struct tile create_grass_tile(int x, int y);

struct tile create_wall_tile(int x, int y);

struct tilemap create_tilemap(char *map_location);

struct tile get_tile(struct tilemap tilemap, int x, int y);

void print_map(struct tilemap tilemap);

#endif
