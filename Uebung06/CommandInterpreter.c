/*CommandInterpreter.c*/
#include "CommandInterpreter.h"
#include <stdio.h>
#include <string.h>

// module internal members
static TCmdHndTable const *gcmds = 0; // table of available commands
static int (*gpPutChar)(int); // function pointer for a put-char-function
// struct that holds all entered params to last cmd
static TParameters gParams = {0};       // set all to /0
static char gInput[MAX_INPUTLEN] = {0}; // modul internal puffer
static int gInputLen = 0;               // length of input string

// error messages
static char *cErrPrint = "Error in Print(...) -> null pointer in parameter\n";
static char *cErrInit = "Must initialize module before use!\n";
static char *cErrNoCmds =
    "Error in Init(...) -> null pointer given for command table\n";
static char *cErrNoPutCh =
    "Error in Init(...) -> null pointer given for put-char-function\n";
static char *cErrBufferOverflow =
    "Input buffer overflow! Max input length exceeded.\n";
static char *cErrTooManyParams =
    "Too many parameters! Max number of parameters exceeded.\n";
static char *cErrParamTooLong =
    "Parameter too long! Max length of parameter exceeded.\n";
static char *cErrUnknownCmd = "Unknown command!\n";

static char *HelloText = "Welcome Command-Interpreter\n>";

//////////////////////////////
// interface module functions
//////////////////////////////

void Init(TCmdHndTable const cmds[], int (*PutCh)(int)) {
  // for error checking here use fprintf to stderr because Print can't be used
  // yet
  if (cmds == 0) {
    fprintf(stderr, "%s", cErrNoCmds);
    return; // error: null pointer given
  }
  if (PutCh == 0) {
    fprintf(stderr, "%s", cErrNoPutCh);
    return; // error: null pointer given
  }

  gcmds = cmds;      // store command table
  gpPutChar = PutCh; // store put-char-function pointer

  Print(HelloText);
}

// pre declaration
static void evaluateCmd();
static void checkInput();

int Process(char const ch) {
  if (gcmds == 0 || gpPutChar == 0) {
    fprintf(stderr, "%s",
            cErrInit); // Only here because can't really Print without init
    return ch;         // error: module not initialized
  }
  if (ch == cESC) {
    return cESC;
  }

  if (gInputLen >= MAX_INPUTLEN) {
    Print(cErrBufferOverflow);
    gInputLen = 0; // reset input length
    return ch;
  }

  if (ch == '\r' || ch == '\n') {
    gInput[gInputLen] = '\0';
    checkInput();

    gInputLen = 0;
    Print("\n>");
    return ch;
  }

  gInput[gInputLen++] = ch;

  return ch;
}

TParameters GetParams() { return gParams; }

void Print(char const *str) {
  if (gcmds == 0 || gpPutChar == 0) {
    fprintf(stderr, "%s",
            cErrInit); // Only here because can't really Print without init
  }

  // check parameter
  if (str == 0) {
    Print(cErrPrint);
    return;
  }

  int i = 0;
  if (gpPutChar != 0) {      // put char function pointer given?
    while (str[i] != '\0') { // iterate given string
      gpPutChar(str[i++]);
    }
  }
}

/////////////////////////////
// internal module functions
/////////////////////////////

// evaluates a completed command
static void evaluateCmd() {
  int i = 0;

  while (gcmds[i].func != 0) {
    if (strcmp(gParams.params[0], gcmds[i].command) == 0) {
      int ret = gcmds[i].func();
      if (ret == 1) {
        Print("OK");
      } else {
        Print("ERROR");
      }
      return;
    }
    i++;
  }

  Print(cErrUnknownCmd);
}

// checks the input for correctness
static void checkInput() {
  char *ptr = gInput;
  // Must do before to ensure paramIndex = 0 correct
  while ((*ptr == ' ' || *ptr == '\t') && *ptr != '\0') {
    ptr++; // skip leading spaces
  }

  if (*ptr == '\0') {
    Print(">");
    return;
  }

  memset(&gParams, 0, sizeof(TParameters));

  int paramIndex = 0;
  int charIndex = 0;

  while (*ptr != '\0' && paramIndex < MAX_PARAMS + 1) {

    if (*ptr != ' ' && *ptr != '\t') {
      // stop at -1 because we need space for null-terminator
      if (charIndex >= MAX_PARAMLEN - 1) {
        Print(cErrParamTooLong);
        return; // stop processing on error
      }

      gParams.params[paramIndex][charIndex] = *ptr;
      charIndex++;
      ptr++;

    } else {
      gParams.params[paramIndex][charIndex] = '\0'; // null-terminate parameter
      paramIndex++;

      if (paramIndex >= MAX_PARAMS) {
        Print(cErrTooManyParams);
        return; // stop processing on error
      }

      charIndex = 0;
      while ((*ptr == ' ' || *ptr == '\t') && *ptr != '\0') {
        ptr++; // skip leading spaces
      }
    }
  }

  for (int i = 0; i <= paramIndex; i++) {
    Print(gParams.params[i]);
    Print(" ");
  }

  Print("\n");
  evaluateCmd();
}
