#include "CommandTable.h"
#include "CommandInterpreter.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

// return code for command functions
int const OK = 1;
int const NOK = 0;

// internal command functions
static int CmdHelp();
static int CmdParamInt();
static int CmdParamFloat();
static int CmdParamString();
static int CmdAddInt();

// CommandTable
static TCmdHndTable const cmds[] = {
    {CmdHelp, "help", "show help text"},
    {CmdHelp, "?", "show help text"},
    {CmdParamString, "string", "[value] first parameter as string"},
    {CmdParamInt, "int", "[value] first parameter as integer"},
    {CmdParamFloat, "float", "[value] first parameter as float"},
    {CmdAddInt, "addint", "[value1] + [value1] adds two integer values"},
    {0, 0, 0} // end criteria
};

// returns address of command table
TCmdHndTable const *GetCmdHndTable() { return cmds; }

//////////////////////////////
// internal command functions
//////////////////////////////

// prints help text of all commandos
// I trust that cmds was filled correctly so no internal null-pointer checks
static int CmdHelp() {
  int i = 0;
  Print("(press <ESC> to exit)\n");
  Print("available commands:\n");

  while (cmds[i].func != 0) {
    Print(cmds[i].command);
    Print(": ");
    Print(cmds[i].description);
    Print("\n");
    i++;
  }

  return OK;
}

// interprets 1st parameter as integer and prints it
static int CmdParamInt() {
  TParameters p = GetParams();
  char *param = p.params[1];

  // kein Parameter
  if (param[0] == '\0') {
    return NOK;
  }

  char *endptr;
  strtol(param, &endptr, 10);

  // there was something that is not a number
  if (param == endptr) {
    return NOK;
  }

  // there are trailing characters
  if (*endptr != '\0') {
    return NOK;
  }

  Print("Parameter: ");
  Print(param);
  Print("\n");

  return OK;
}

// interprets 1st parameter as float and prints it
static int CmdParamFloat() {
  TParameters p = GetParams();
  char *param = p.params[1];

  // kein Parameter
  if (param[0] == '\0') {
    return NOK;
  }

  char *endptr;
  strtof(param, &endptr);

  // there was something that is not a number
  if (param == endptr) {
    return NOK;
  }

  // there are trailing characters
  if (*endptr != '\0') {
    return NOK;
  }

  Print("Parameter: ");
  Print(param);
  Print("\n");

  return OK;
}

// interprets 1st parameter as string and prints it
static int CmdParamString() {
  TParameters p = GetParams();
  char *param = p.params[1];

  // kein Parameter
  if (param[0] == '\0') {
    return NOK;
  }

  Print("Parameter: ");
  Print(param);
  Print("\n");

  return OK;
}

// adds two integer parameter and prints the result
static int CmdAddInt() {
  TParameters p = GetParams();
  char *param1 = p.params[1];
  char *param2 = p.params[2];

  // brauchen zwei Parameter
  if (param1[0] == '\0' || param2[0] == '\0') {
    return NOK;
  }

  char *endptr1;
  char *endptr2;

  int value1 = (int)strtol(param1, &endptr1, 10);
  if (param1 == endptr1 || *endptr1 != '\0') {
    return NOK;
  }

  int value2 = (int)strtol(param2, &endptr2, 10);
  if (param2 == endptr2 || *endptr2 != '\0') {
    return NOK;
  }

  int sum = value1 + value2;

  char buf[32];
  sprintf(buf, "%d", sum);

  Print("Result: ");
  Print(buf);
  Print("\n");

  return OK;
}
