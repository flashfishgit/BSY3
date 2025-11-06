#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// error code
static short const OK = 0;
static short const NOK = 1;

// error and info messages
static char const *const cErrOpen = "error in fopen";
static char const *const cErrClose = "error in fclose";
static char const *const cErrWrite = "error in fwrite";
static char const *const cErrRead = "error in fgetc";
static char const *const cErrUsage = "error in commandline -> \
./readWrite fileName";
static char const *const cFileEnd = "end of file reached";

// writing a binary block to file
static short WriteABlock(char const *const fileName) {
  assert(fileName != NULL);

  size_t const cLen = 32;
  size_t const cMaxPers = 2;

  typedef struct {
    char name[cLen];
    unsigned short age;
  } Person;

  // array of Persons
  Person persGrp[cMaxPers];
  memset(&persGrp, 0, sizeof(Person) * cMaxPers); // inint array with 0 Byte

  // init persons
  strcpy(persGrp[0].name, "Robert \"Eisenfuss\" Pecl");
  persGrp[0].age = 51;
  strcpy(persGrp[1].name, "Anton \"Rambo\" Pfeffer");
  persGrp[1].age = 52;

  // open file for writting
  FILE *file = fopen(fileName, "w");
  if (file == 0) {
    perror(cErrOpen);
    return NOK;
  }

  if (fwrite(persGrp, sizeof(Person), cMaxPers, file) != cMaxPers) {
    perror(cErrWrite);
    fclose(file);
    return NOK;
  }

  if (fclose(file) == EOF) {
    perror(cErrClose);
    return NOK;
  }

  return OK;
}

// output file character by character
static short OutputChars(char const *const fileName) {
  assert(fileName != 0);

  FILE *file = fopen(fileName, "r");
  if (file == NULL) {
    perror(cErrRead);
    return NOK;
  }

  // read do end of file
  int ch = EOF;
  while ((ch = fgetc(file)) != EOF) {
    printf("Zeichen: '%c' -> ASCII: %d\n", ch, ch);
  }

  // check reading error
  if (ferror(file) != 0) {
    fclose(file);
    perror(cErrRead);
    return NOK;
  }

  // check end of file
  if (feof(file) != 0) {
    puts(cFileEnd); // keine Fehler, sondern eine Info
  }

  if (fclose(file) == EOF) {
    perror(cErrClose);
    return NOK;
  }

  return OK;
}

// main program
int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "%s\n", cErrUsage);
    return EXIT_FAILURE;
  }

  // uses the fopen, fclose and fwrite functions
  if (WriteABlock(argv[1]) == NOK) {
    return EXIT_FAILURE;
  }

  // reads character by character from a file
  if (OutputChars(argv[1]) == NOK) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
