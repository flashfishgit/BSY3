#include <fcntl.h>    // fuer Elementare I/O Funktionen
#include <string.h>   // fuer strlen
#include <sys/stat.h> //mode_t --> Zugriffsrechte fuer die Datei
#include <unistd.h>   //fuer die Filedeskriptoren

// return Werte
static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;

// Fehlermeldungen
static char const *const USAGE_CMD = "useage: write_file <filename>\n";
static char const *const ERR_OPEN = "error in open\n";
static char const *const ERR_CLOSE = "error in close\n";
static char const *const ERR_WRITE = "error in write\n";

int main(int argc, char *argv[]) {
  if (argc != 2) {
    // schreibt Fehlermeldung auf die Fehlerausgabe
    write(STDERR_FILENO, USAGE_CMD, strlen(USAGE_CMD));
    return EXIT_FAILURE;
  }

  umask(0); // Vorgriff -> setzt die Einschraenkuzngsmakse auf 0.
  // => Der Prozess darf alle Rechte fuer die Datei vergeben

  // Zugriffsrechte fuer die Datei -> -rw-rw-r--
  mode_t const mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH;
  int const fd = open(argv[1], O_WRONLY | O_EXCL | O_CREAT, mode);
  if (fd == -1) {
    write(STDERR_FILENO, ERR_OPEN, strlen(ERR_OPEN));
    return EXIT_FAILURE;
  }

  // einfache Daten in die Datei schreiben
  char const *buffer = "Hallo IO-Funktion\n";
  ssize_t const bytesWritten = write(fd, buffer, strlen(buffer));
  if (bytesWritten == -1) {
    write(STDERR_FILENO, ERR_WRITE, strlen(ERR_WRITE));
    close(fd); // keine neue Fehlerbehandlung
    return EXIT_FAILURE;
  }

  // Datei schliessen
  if (close(fd) == -1) {
    write(STDERR_FILENO, ERR_CLOSE, strlen(ERR_CLOSE));
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
