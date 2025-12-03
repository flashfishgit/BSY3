#include <assert.h>
#include <memory.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 128

// ----------------- Debug Macros ----------------
// #define DEBUG
// Enable debug output when DEBUG is defined
#ifdef DEBUG

// cyan
#define DBG_COLOR "\x1b[36m"
#define DBG_RESET "\x1b[0m"

#define DEBUG_LOG(fmt, ...)                                                    \
  do {                                                                         \
    fprintf(stderr, DBG_COLOR "[DEBUG] %s:%d:%s(): " fmt DBG_RESET "\n",       \
            __FILE__, __LINE__, __func__, ##__VA_ARGS__);                      \
  } while (0)
#else
#define DEBUG_LOG(...)                                                         \
  do {                                                                         \
  } while (0)
#endif
// ----------------- End Debug Macros ----------------

// ------------------ Error Messages ----------------
static char const *const cErrOpen = "error in fopen";
static char const *const cErrClose = "error in fclose";
static char const *const cErrRead = "error in fgets";
static char const *const cErrUsage =
    "error in commandline -> ./ccheck fileName\n";
static char const *const cErrLineBufferOverflow =
    "error: line buffer overflow, increase BUFFER_SIZE";
// ------------------ End Error Messages ----------------

// -------------------  typedef ----------------
typedef struct {
  int depth_curly; // {}
  int depth_round; // ()
  int depth_comment;
  unsigned int line_number;
  bool in_block_comment;
  bool in_line_comment;
  bool in_string;
  bool in_char;
  bool depth_curly_was_negative;
  bool depth_round_was_negative;
  bool depth_comment_was_negative;
  bool esc;
} State;

typedef enum {
  SKIP_LINE = -1,
  SKIP_NONE = 0,
  SKIP_1_CHAR = 1,
  SKIP_2_CHAR = 2,
} skipResult;

typedef enum {
  CHR_NONE = 0,
  CHR_BLOCK_COMMENT_START,
  CHR_BLOCK_COMMENT_END,
  CHR_LINE_COMMENT,
  CHR_STRING,
  CHR_CHAR,
  CHR_ESC,
  CHR_CURLEY_OPEN,
  CHR_CURLEY_CLOSE,
  CHR_ROUND_OPEN,
  CHR_ROUND_CLOSE,
} CharType;
// ------------------- End typedef ----------------

// ------------------- static Functions ----------------

// checks if the current state allows skipping characters
// and when in such a state, returns how many characters to skip
// Also checks for exiting such states
static skipResult checkIfCanSkip(State *state, char chr, char next_char) {
  assert(state != NULL);

  if (state->in_line_comment) {
    DEBUG_LOG("In line comment, skipping rest of line");
    return SKIP_LINE;
  }

  if (state->in_block_comment) {
    if (chr == '*' && next_char == '/') {
      DEBUG_LOG("Exiting block comment");
      state->in_block_comment = false;
      state->depth_comment--;
      return SKIP_2_CHAR;
    }
    return SKIP_1_CHAR;
  }

  if (state->in_string) {
    if (chr == '"' && !state->esc) {
      DEBUG_LOG("Exiting string literal");
      state->in_string = false;
      return SKIP_1_CHAR;
    }
    state->esc = (chr == '\\' && !state->esc);
    return SKIP_1_CHAR;
  }

  if (state->in_char) {
    if (chr == '\'' && !state->esc) {
      DEBUG_LOG("Exiting char literal");
      state->in_char = false;
      return SKIP_1_CHAR;
    }
    state->esc = (chr == '\\' && !state->esc);
    return SKIP_1_CHAR;
  }

  return SKIP_NONE;
}

// process a single line and update the state accordingly
static void process_line(char const *const line, State *state) {
  if (line == NULL || state == NULL) {
    return;
  }

  size_t i = 0;
  // while loop so it is clear that increamenting i is done manually
  while (line[i] != '\0') {
    char chr = line[i];
    char next_c = line[i + 1]; // never undefined, as \0 must exist after chr

    // check if we can skip characters due to current state
    // also handles exiting states
    skipResult skip = checkIfCanSkip(state, chr, next_c);
    if (skip == SKIP_LINE) {
      break;
    }
    if (skip == SKIP_2_CHAR) {
      i += 2;
      continue;
    }
    if (skip == SKIP_1_CHAR) {
      i += 1;
      continue;
    }

    // check for entering states or updating depths
    switch (chr) {
    case '/':
      if (next_c == '*') {
        DEBUG_LOG("Entering block comment at index %zu", i);
        state->in_block_comment = true;
        state->depth_comment++;
        i += 2;
        continue;
      }
      if (next_c == '/') {
        DEBUG_LOG("Entering line comment at index %zu", i);
        state->in_line_comment = true;
        // Dont need to increas i since line stops
      }
      break;
    case '*':
      // This state can only be reached if not in block comment and */ comes
      // This Implies an error
      if (next_c == '/') {
        DEBUG_LOG("Exiting block comment at index %zu", i);
        state->in_block_comment = false;
        state->depth_comment--;
        i += 2;
        continue;
      }
      break;
    case '"':
      DEBUG_LOG("Entering string literal at index %zu", i);
      state->in_string = true;
      state->esc = false;
      break;
    case '\'':
      DEBUG_LOG("Entering char literal at index %zu", i);
      state->in_char = true;
      break;
    case '{':
      DEBUG_LOG("Entering '{' at index %zu", i);
      state->depth_curly++;
      break;
    case '}':
      DEBUG_LOG("Exiting '}' at index %zu", i);
      state->depth_curly--;
      break;
    case '(':
      DEBUG_LOG("Entering '(' at index %zu", i);
      state->depth_round++;
      break;
    case ')':
      DEBUG_LOG("Found ')' at index %zu", i);
      state->depth_round--;
      break;
    default:
      break;
    }

    i += 1;

    // check here so negative depths are recorded even if multiple in one line
    if (state->depth_curly < 0) {
      state->depth_curly_was_negative = true;
    }
    if (state->depth_round < 0) {
      state->depth_round_was_negative = true;
    }
    if (state->depth_comment < 0) {
      state->depth_comment_was_negative = true;
    }
  }
}

// prints the end state of the parser
static void print_EndState(State const *const state) {
  if (state == NULL) {
    return;
  }
  printf("---------------------------------\n");
  if (state->depth_curly != 0) {
    printf("    - Geschweifte Klammern { } nicht ausgeglichen\n");
  }
  if (state->depth_round != 0) {
    printf("    - Runde Klammern ( ) nicht ausgeglichen\n");
  }
  if (state->depth_comment != 0) {
    printf("    - Blockkommentare /* */ nicht ausgeglichen\n");
  }
  if (state->depth_curly_was_negative) {
    printf("    - Warning: Geschweifte Klammern waren einmal negative\n");
  }
  if (state->depth_round_was_negative) {
    printf("    - Warning: Runde Klammern waren einmal negative\n");
  }
  if (state->depth_comment_was_negative) {
    printf("    - Warning: Blockkommentare waren einmal negative\n");
  }

  if (state->depth_curly == 0 && state->depth_round == 0 &&
      state->depth_comment == 0 && !state->depth_curly_was_negative &&
      !state->depth_round_was_negative && !state->depth_comment_was_negative) {
    printf("    - Alles ausgeglichen!\n");
  }
}

// ------------------- End static Functions ----------------

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf(cErrUsage);
    DEBUG_LOG("Invalid number of arguments: %d", argc - 1);
    return EXIT_FAILURE;
  }
  DEBUG_LOG("Given File %s", argv[1]);

  FILE *inputfile = fopen(argv[1], "r");
  if (inputfile == NULL) {
    perror(cErrOpen);
    DEBUG_LOG("Could not open file %s", argv[1]);
    return EXIT_FAILURE;
  }

  char line[BUFFER_SIZE];
  State state = {0};
  while (fgets(line, sizeof(line), inputfile) != NULL) {
    char *truncated = strchr(line, '\n');

    // when it was not possible to load the complete line into the buffer
    // cancle program since general line length should not be over 512
    // In a programm
    // Correctly handling line feeds would require handling a lot of edge cases
    // like /<bufferend overflow>/ or similar
    if (truncated == NULL) {
      perror(cErrLineBufferOverflow);
      DEBUG_LOG("Line buffer overflow detected");
      fclose(inputfile); // dont check in error state
      return EXIT_FAILURE;
    }

    // update state based on line content
    process_line(line, &state);

    state.in_line_comment = false; // reset line comment at EOL
    // Remove newline character for cleaner output
    *truncated = '\0';
    state.line_number++;

    printf("%5d: {%2d} (%2d) /*%2d*/ | %s\n", state.line_number,
           state.depth_curly, state.depth_round, state.depth_comment, line);
  }
  // check if reading ended because of eof or error
  if (!feof(inputfile) && ferror(inputfile)) {
    perror(cErrRead);
    DEBUG_LOG("Error occurred while reading file %s", argv[1]);
    fclose(inputfile); // check for fclose error
  }

  // print final state
  print_EndState(&state);

  // close file and check for error
  if (fclose(inputfile) != 0) {
    perror(cErrClose);
    DEBUG_LOG("Could not close file %s", argv[1]);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
