#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Ausgabe mit unterschiedlichen Ausgabefunktionen
void PrintArray(char *const argv[], int argc) {
  assert(argv != NULL && argc > 0);

  fprintf(stdout, "number: %d\n", argc);

  for (int i = 0; i < argc; ++i) {

    // schreibt kein /n damit wird nicht gleich geschrieben
    // wiel wir zeilen bufferung haben
    fputs(argv[i], stdout);

    // fflush(stdout); // erzwingt sofortige Ausgabe
    fputc('\n', stdout); // leert auch den Puffer bei Zeilenpufferung

    puts(argv[i]);
  }

  char output[256];
  memset(output, 0, sizeof(output));

  sprintf(output, "address of param: %p\n", &argc);
  fprintf(stderr, "%s", output);
  fputs(argv[0], stdout);
}

int main(int argc, char *argv[]) {
  puts("Hello World!");
  PrintArray(argv, argc);

  // stdout wird voll gepuffert. Interner Puffer wird zur Verfuegung gestellt.
  // Parameter 2 und 4 werden fuer einen eigenen Puffer benoetigt. (Addresse und
  // groesse)
  setvbuf(stdout, 0, _IOFBF, 0);
  puts("\nVoll-Pufferung  von stdout mit internen Puffer");
  PrintArray(argv, argc);

  // stdout wird nicht gepuffert.
  setvbuf(stdout, 0, _IONBF, 0);
  puts("\nKein Pufferung  von stdout mit internen Puffer");
  PrintArray(argv, argc);

  // stdout wird Line gebuffert
  setvbuf(stdout, 0, _IONBF, 0);
  puts("\nLine Pufferung  von stdout mit internen Puffer");
  PrintArray(argv, argc);

  return 0;
}
