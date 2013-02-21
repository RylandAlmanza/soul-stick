#ifndef BEING_H_
#define BEING_H_

struct being {
  char* name;
  char* description;
  int x;
  int y;
  int old_x;
  int old_y;
  char character;
  int color_pair;
};

struct being create_being(char *name, char *description, char character, int x,
			  int y, int color_pair); 

#endif
