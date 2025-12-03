#include "CommandInterpreter.h"
#include "CommandTable.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  char ch = '\0';

  // Test Command Interpreter function without init
  Process('a');  // should print error message
  Print("Test"); // should print error message

  Init(NULL, putchar); // should print error message
  Process('a');        // should print error message
  Print("Test");       // should print error message

  Init(GetCmdHndTable(), NULL); // should print error message
  Process('a');                 // should print error message
  Print("Test");                // should print error message

  // initialize the command interpreter
  Init(GetCmdHndTable(), putchar);

  // read characters from input
  do {
    ch = getchar(); // wait for key
  } while (Process(ch) != cESC); // ESC typed?

  return EXIT_SUCCESS;
}
