#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

static const char *const errUsage = "Usage: %s <directory>\n";
static const char *const errOpenDir = "opendir";
static const char *const errChdirSubdir = "chdir in Unterverzeichnis";
static const char *const errChdirBack = "chdir zurueck";
static const char *const errCloseDir = "closedir";
static const char *const errGetcwdStart = "getcwd (Startverzeichnis)";
static const char *const errChdirTarget = "chdir (Zielverzeichnis)";
static const char *const errGetcwdTarget = "getcwd (Zielverzeichnis)";
static const char *const errChdirBackStart =
    "chdir (zurueck zum Startverzeichnis)";
static const char *const errIsDirCheck = "isDirectory (chdir-Test)";
static const char *const errStartPathTooLong =
    "Fehler: Laenge des Startverzeichnispfads ist groesser als PATH_MAX "
    "(%ld).\n";
static const char *const errTargetPathTooLong =
    "Fehler: Laenge des Zielverzeichnispfads ist groesser als PATH_MAX "
    "(%ld).\n";

typedef enum { ISDIR, NODIR, ERROR } isDir_t;

static isDir_t isDirectory(char const *name) {
  if (chdir(name) == 0) {
    // erfolgreich reingegangen -> also Verzeichnis
    if (chdir("..") == -1) {
      perror(errChdirBack);
      return ERROR;
    }
    return ISDIR;
  }

  if (errno == ENOTDIR) {
    return NODIR;
  }

  return ERROR;
}

// go through directory and print its tree structure
static void print_tree(int depth) {
  DIR *dir;
  struct dirent *entry;

  dir = opendir(".");
  if (dir == NULL) {
    perror(errOpenDir);
    return;
  }

  // go through all elements in directory
  while ((entry = readdir(dir)) != NULL) {
    const char *name = entry->d_name;

    // skip two directorys that always exist
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
      continue;

    for (int i = 0; i < depth; ++i) {
      printf("| ");
    }

    printf("|---[%s", name);

    // checks if element is a directory
    isDir_t check = isDirectory(name);
    switch (check) {
    case NODIR:
      printf("]\n");
      break;

    case ISDIR:
      printf("/]\n");

      if (chdir(name) == -1) {
        perror(errChdirSubdir);
        closedir(dir);
        return;
      }

      print_tree(depth + 1);

      if (chdir("..") == -1) {
        perror(errChdirBack);
        closedir(dir);
        return;
      }
      break;

    case ERROR:
    default:
      printf("]\n");
      perror(errIsDirCheck);
      closedir(dir);
      return;
    }
  }

  if (closedir(dir) == -1) {
    perror(errCloseDir);
  }
}

int main(int argc, char *argv[]) {
  char startdir[PATH_MAX];

  if (argc != 2) {
    fprintf(stderr, errUsage, argv[0]);
    return EXIT_FAILURE;
  }

  // remember starting directory
  if (getcwd(startdir, sizeof(startdir)) == NULL) {
    perror(errGetcwdStart);

    if (errno == ERANGE) {
      fprintf(stderr, errStartPathTooLong, (long)PATH_MAX);
    }

    return EXIT_FAILURE;
  }

  // switch to target directory
  if (chdir(argv[1]) == -1) {
    perror(errChdirTarget);

    if (errno == ENOENT) {
      fprintf(stderr, "Fehler: Pfad '%s' wurde nicht gefunden.\n", argv[1]);
    } else if (errno == ENOTDIR) {
      fprintf(stderr, "Fehler: '%s' ist kein Verzeichnis.\n", argv[1]);
    } else if (errno == EACCES) {
      fprintf(stderr, "Fehler: Keine Berechtigung, um '%s' zu betreten.\n",
              argv[1]);
    }

    return EXIT_FAILURE;
  }

  // absolute path of target directory
  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof(cwd)) == NULL) {
    perror(errGetcwdTarget);

    if (errno == ERANGE) {
      fprintf(stderr, errTargetPathTooLong, (long)PATH_MAX);
    }

    // try to switch back to starting directory
    if (chdir(startdir) == -1) {
      perror(errChdirBackStart);
    }
    return EXIT_FAILURE;
  }

  // Print first line, the absolute path of the target directory
  printf("[%s/]\n", cwd);

  // recursively print the directory tree
  print_tree(0);

  // switch back to starting directory
  if (chdir(startdir) == -1) {
    perror(errChdirBackStart);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
