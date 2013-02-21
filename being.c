#include "being.h"

struct being create_being(char *name, char *description, char character, int x,
			  int y, int color_pair) {
  struct being being;
  being.name = name;
  being.description = description;
  being.character = character;
  being.x = x;
  being.y = y;
  being.old_x = being.x;
  being.old_y = being.y;
  being.color_pair = color_pair;

  return being;
}
