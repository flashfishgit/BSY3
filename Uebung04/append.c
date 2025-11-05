#include <fcntl.h>
#include <stddef.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define _STR_HELPER(x) #x
#define _STR(x) _STR_HELPER(x)

#define ERR_LOG(msg)                                                           \
  do {                                                                         \
    const char *_m = (msg);                                                    \
    size_t _len = strlen(_m);                                                  \
    write(STDERR_FILENO, __FILE__, sizeof(__FILE__) - 1);                      \
    write(STDERR_FILENO, ":", 1);                                              \
    write(STDERR_FILENO, _STR(__LINE__), sizeof(_STR(__LINE__)) - 1);          \
    write(STDERR_FILENO, ": ", 2);                                             \
    write(STDERR_FILENO, _m, _len);                                            \
  } while (0)

#define ERR_LOG_WITHOUT_LINE(msg)                                              \
  do {                                                                         \
    const char *_m = (msg);                                                    \
    size_t _len = strlen(_m);                                                  \
    write(STDERR_FILENO, _m, _len);                                            \
  } while (0)

static short const EXIT_FAILURE = 1;
static short const EXIT_SUCCESS = 0;

static char const *const USAGE_CMD =
    "useage: ./append.c <inputfile> <outputfile>\n";
static char const *const ERR_OPEN = "error in open: ";
static char const *const ERR_CLOSE = "error in close: ";
static char const *const ERR_WRITE = "error in write";
static char const *const ERR_READ = "error in read";
static char const *const ERR_BUFFER_OVERFLOW = "error: buffer overflow\n";
static char const *const ERR_INPUT_OUTPUT_SAME =
    "error: input and output file must be different\n";

static char const *const OUTPUT_MESSAGE = "Total bytes written: ";

static size_t const BUFFER_SIZE = 512;

static int convertToString(unsigned int x, char *buf, size_t bufsize) {
  char tmp[32];
  size_t n = 0;

  if (bufsize == 0)
    return -1;

  do {
    tmp[n++] = (char)('0' + (x % 10));
    x /= 10;
  } while (x != 0);

  if (n + 1 > bufsize)
    return -1;

  // Reverse into output
  for (size_t i = 0; i < n; ++i) {
    buf[i] = tmp[n - 1 - i];
  }
  buf[n] = '\0';

  return (int)n;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    ERR_LOG(USAGE_CMD);
    return EXIT_FAILURE;
  }

  if (strcmp(argv[1], argv[2]) == 0) {
    ERR_LOG(ERR_INPUT_OUTPUT_SAME);
    return EXIT_FAILURE;
  }

  char buffer[BUFFER_SIZE];
  unsigned int totalBytesWritten = 0;

  umask(0);

  // ---------- Open Files ---------

  int readfile = open(argv[1], O_RDONLY);
  if (readfile == -1) {
    ERR_LOG(ERR_OPEN);
    ERR_LOG_WITHOUT_LINE(argv[1]);
    ERR_LOG_WITHOUT_LINE("\n");
    close(readfile); // keine neue Fehlerbehandlung
    return EXIT_FAILURE;
  }

  int writefile =
      open(argv[2], O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
  if (writefile == -1) {
    ERR_LOG(ERR_OPEN);
    ERR_LOG_WITHOUT_LINE(argv[2]);
    ERR_LOG_WITHOUT_LINE("\n");
    close(readfile);  // keine neue Fehlerbehandlung
    close(writefile); // keine neue Fehlerbehandlung
    return EXIT_FAILURE;
  }

  // ---------- Read from input and write to output ----------

  ssize_t bytesRead;
  do {
    bytesRead = read(readfile, buffer, BUFFER_SIZE);
    if (bytesRead == -1) {
      ERR_LOG(ERR_READ);
      ERR_LOG_WITHOUT_LINE("\n");
      close(readfile);  // keine neue Fehlerbehandlung
      close(writefile); // keine neue Fehlerbehandlung
      return EXIT_FAILURE;
    }

    ssize_t bytesWritten = write(writefile, buffer, bytesRead);
    if (bytesWritten == -1) {
      ERR_LOG(ERR_WRITE);
      ERR_LOG_WITHOUT_LINE("\n");
      close(readfile);
      close(writefile);
      return EXIT_FAILURE;
    }
    totalBytesWritten += (unsigned int)bytesWritten;
  } while ((size_t)bytesRead == BUFFER_SIZE);

  // ---------- Close Files ---------

  if (close(readfile) == -1) {
    ERR_LOG(ERR_CLOSE);
    ERR_LOG_WITHOUT_LINE(argv[1]);
    ERR_LOG_WITHOUT_LINE("\n");
    close(writefile); // keine neue Fehlerbehandlung
    return EXIT_FAILURE;
  }

  if (close(writefile) == -1) {
    ERR_LOG_WITHOUT_LINE(ERR_CLOSE);
    ERR_LOG_WITHOUT_LINE(argv[2]);
    ERR_LOG("\n");
    return -1;
  }

  char byteCountStr[32];
  int written =
      convertToString(totalBytesWritten, byteCountStr, sizeof(byteCountStr));
  if (written == -1) {
    ERR_LOG(ERR_BUFFER_OVERFLOW);
    return EXIT_FAILURE;
  }
  write(STDOUT_FILENO, OUTPUT_MESSAGE, strlen(OUTPUT_MESSAGE));
  write(STDOUT_FILENO, byteCountStr, (size_t)written);
  write(STDOUT_FILENO, "\n", 1);

  return EXIT_SUCCESS;
}
