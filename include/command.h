#ifndef _COMMAND_H
#define _COMMAND_H

#define MAX_COMMAND_STR_LEN 23

enum Command {
  INVALID = -1,
  ON = 1580545,
  AUTO,
  OFF,
  S_PLUS,
  M_PLUS,
  BRIGHTNESS_UP,
  S_MINUS,
  M_MINUS,
  BRIGHTNESS_DOWN,
  RED,
  GREEN,
  BLUE,
  PINK,
  LIGHT_GREEN,
  LIGHT_BLUE,
  ORANGE,
  PURPLE,
  TEAL,
  YELLOW,
  LIGHT_PURPLE,
  WHITE,
};

const char* cmd_to_str(enum Command cmd);
const enum Command cmd_from_str(char *str);
const enum Command cmd_from_int(int i);

#endif
