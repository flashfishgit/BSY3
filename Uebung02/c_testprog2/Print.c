#include <stdio.h>  // include printf, putc
#include <stdlib.h> // include malloc, free
#include <string.h> // include strlen, strcpy, strcat

static char const *mErrorText[] = {"OK", "NOK"};

// Builds and prints a result string
// but output is always "OK" since the testfunction always returns 0
// an enum would be better here for the return codes
void PrintResult(char const *const text, unsigned const errorCode) {
  // +2 because of space and null terminator
  char *out = (char *)malloc(strlen(text) + strlen(mErrorText[errorCode]) + 2);
  strcpy(out, text);
  strcat(out, " "); // Appends to out a space
  strcat(out, mErrorText[errorCode]);
  out[strlen(out)] = '\0'; // Null terminator
  printf("%s\n\n", out);
  free(out);
}

void PrintHeader(char const header[]) {
  unsigned headLen = 0;

  printf("\n%s\n", header);
  headLen = strlen(header);
  while (headLen > 0) {
    putc('=', stdout);
    headLen--;
  }
  printf("\n");
}

// Prints an array of strings
void PrintStrArr(char const *const *const arr, unsigned const len) {
  size_t i = 0;
  for (; i < len; i++) {
    printf("%s\n", arr[i]);
  }
}

// Prints an array of integers
void PrintIntArr(int const *const arr, int len) {
  for (int i = 0; i < len; ++i) {
    printf("%d\n", arr[i]);
  }
  return;
}
