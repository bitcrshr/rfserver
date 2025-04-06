#include "command.h"
#include <string.h>

const char* cmd_to_str(enum Command cmd) {
  switch (cmd) {
    case ON:
      return "COMMAND_ON";
    case AUTO:
      return "COMMAND_AUTO";
    case OFF:
      return "COMMAND_OFF";
    case S_PLUS:
      return "COMMAND_S_PLUS";
    case M_PLUS:
      return "COMMAND_M_PLUS";
    case BRIGHTNESS_UP:
      return "COMMAND_BRIGHTNESS_UP";
    case S_MINUS:
      return "COMMAND_S_MINUS";
    case M_MINUS:
      return "COMMAND_M_MINUS";
    case BRIGHTNESS_DOWN:
      return "COMMAND_BRIGHTNESS_DOWN";
    case RED:
      return "COMMAND_RED";
    case GREEN:
      return "COMMAND_GREEN";
    case BLUE:
      return "COMMAND_BLUE";
    case PINK:
      return "COMMAND_PINK";
    case LIGHT_GREEN:
      return "COMMAND_LIGHT_GREEN";
    case LIGHT_BLUE:
      return "COMMAND_LIGHT_BLUE";
    case ORANGE:
      return "COMMAND_ORANGE";
    case PURPLE:
      return "COMMAND_PURPLE";
    case TEAL:
      return "COMMAND_TEAL";
    case YELLOW:
      return "COMMAND_YELLOW";
    case LIGHT_PURPLE:
      return "COMMAND_LIGHT_PURPLE";
    case WHITE:
      return "COMMAND_WHITE";
    default:
      return "COMMAND_INVALID";
  }
}

const enum Command cmd_from_str(char *s) {
  if (!strcmp(s, "COMMAND_ON")) {
    return ON;
  }
  if (!strcmp(s, "COMMAND_AUTO")) {
    return AUTO;
  }
  if (!strcmp(s, "COMMAND_OFF")) {
    return OFF;
  }
  if (!strcmp(s, "COMMAND_S_PLUS")) {
    return S_PLUS;
  }
  if (!strcmp(s, "COMMAND_M_PLUS")) {
    return M_PLUS;
  }
  if (!strcmp(s, "COMMAND_BRIGHTNESS_UP")) {
    return BRIGHTNESS_UP;
  }
  if (!strcmp(s, "COMMAND_S_MINUS")) {
    return S_MINUS;
  }
  if (!strcmp(s, "COMMAND_M_MINUS")) {
    return M_MINUS;
  }
  if (!strcmp(s, "COMMAND_BRIGHTNESS_DOWN")) {
    return BRIGHTNESS_DOWN;
  }
  if (!strcmp(s, "COMMAND_RED")) {
    return RED;
  }
  if (!strcmp(s, "COMMAND_GREEN")) {
    return GREEN;
  }
  if (!strcmp(s, "COMMAND_BLUE")) {
    return BLUE;
  }
  if (!strcmp(s, "COMMAND_PINK")) {
    return PINK;
  }
  if (!strcmp(s, "COMMAND_LIGHT_GREEN")) {
    return LIGHT_GREEN;
  }
  if (!strcmp(s, "COMMAND_LIGHT_BLUE")) {
    return LIGHT_BLUE;
  }
  if (!strcmp(s, "COMMAND_ORANGE")) {
    return ORANGE;
  }
  if (!strcmp(s, "COMMAND_PURPLE")) {
    return PURPLE;
  }
  if (!strcmp(s, "COMMAND_TEAL")) {
    return TEAL;
  }
  if (!strcmp(s, "COMMAND_YELLOW")) {
    return YELLOW;
  }
  if (!strcmp(s, "COMMAND_LIGHT_PURPLE")) {
    return LIGHT_PURPLE;
  }
  if (!strcmp(s, "COMMAND_WHITE")) {
    return WHITE;
  }

  return INVALID;
}

const enum Command cmd_from_int(int i) {
  switch (i) {
    case ON:
      return ON;
    case AUTO:
      return AUTO;
    case OFF:
      return OFF;
    case S_PLUS:
      return S_PLUS;
    case M_PLUS:
      return M_PLUS;
    case BRIGHTNESS_UP:
      return BRIGHTNESS_UP;
    case S_MINUS:
      return S_MINUS;
    case M_MINUS:
      return M_MINUS;
    case BRIGHTNESS_DOWN:
      return BRIGHTNESS_DOWN;
    case RED:
      return RED;
    case GREEN:
      return GREEN;
    case BLUE:
      return BLUE;
    case PINK:
      return PINK;
    case LIGHT_GREEN:
      return LIGHT_GREEN;
    case LIGHT_BLUE:
      return LIGHT_BLUE;
    case ORANGE:
      return ORANGE;
    case PURPLE:
      return PURPLE;
    case TEAL:
      return TEAL;
    case YELLOW:
      return YELLOW;
    case LIGHT_PURPLE:
      return LIGHT_PURPLE;
    case WHITE:
      return WHITE;
    default:
      return INVALID;
  }
}
