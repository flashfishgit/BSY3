#include <stdio.h>  // include printf, fgets, scanf
#include <stdlib.h> // include malloc, free
#include <string.h> // include strlen, strcpy

#include "Print.h" // include PrintHeader, PrintStrArr, PrintIntArr"

#define MAX 100
#define BUFFER_LEN 1024

// Move Prototypes to top of file
static void PrintLength(const char buf[]);
static void Shift(char v[]);

// Tests Input methode and prints them back to stdout
int TestFormatIO() {
  // change to const where possible
  const int i = 1;
  const int j = 65;
  const char *pCh = 0;
  const double pi = 3.1415;

  char str[BUFFER_LEN] = "";

  // Remove unused variables
  // int Arr[2][3] = {{1, 2, 3}, {4, 5, 6}};
  // int h = 0x10; Remove unused variable

  PrintHeader("Test Format IO");

  // i++; Directly initialize i with 1
  printf("i: %d ", i);
  // Don't know what was expected here, Now print the address of i
  printf("Address: %p\n", (void *)&i);

  printf("Bitte eingeben: ");
  fgets(str, BUFFER_LEN, stdin);
  printf("%s", str);
  printf("\n");

  // Remove unneeded fgets
  // fgets(str, BUFFER_LEN, stdin);

  int val = 0;
  // Add text before scanf
  printf("Geben sie einen Integer ein:");
  scanf("%4d", &val);
  printf("%d\n", val);

  pCh = (char *)&j;
  // Print the Pointer of the char
  printf("%p \n", (void *)pCh);
  printf("%c \n", *pCh);

  printf("PI: %f \n", pi);

  return 0;
}

// Copies an string into a char var. the
// prints the length
// Then shifts an text and prints it
int TestString() {
  char buffer[MAX] = "";
  char text[] = "ABC";

  PrintHeader("Test Strings");

  strcpy(buffer, "Das ist ein C-String");
  PrintLength(buffer);

  printf("Buffer: %s \n", buffer);

  printf("Text vorher : %s \n", text);
  Shift(text);
  printf("Text nachher: %s \n", text);

  return 0;
}

// Prints a string and its length
static void PrintLength(const char buf[]) {
  // strlen return unsigned long, but %d was used
  printf("Length of \"%s\" is %lu chars\n", buf, strlen(buf));
}

// Implement real shifts
// pushes chars to the right
// pushed out element get added on the left side
static void Shift(char v[]) {
  size_t len = strlen(v);
  if (len < 2)
    return; // empty or 1-char strings: nothing to do

  char last = v[len - 1]; // save last character
  for (size_t i = len - 1; i > 0; --i) {
    v[i] = v[i - 1]; // shift right
  }
  v[0] = last;
}

// creates dynamic memory for a string,
// writes something to it and prints it
int TestDynMem() {
  char *Buf = 0;

  PrintHeader("Test dynamic Memory");

  Buf = (char *)malloc(100);
  if (Buf != 0) {
    strcpy(Buf, "Hello World!");
    printf(" -> %s \n", Buf);

    free(Buf);
  } else {
    printf("Speicher konnte nicht reserviert werden!");
    return 1;
  }
  return 0;
}

// defines a struct Person,
// fills two instances and prints them
int TestStruct() {
  struct Person {
    char name[100];
    unsigned weight;
  } max;

  struct Person moritz;

  PrintHeader("Test Structs");

  strcpy(moritz.name, "Moritz Mustermann");
  moritz.weight = 80;

  // reset all bytes of max to 0
  memset(&max, 0, sizeof(struct Person));

  // Add nhull terminator
  memcpy(max.name, "Max Mustermann", 14 + 1);
  max.weight = moritz.weight;

  printf("Person: %s hat %d kg\n", max.name, max.weight);
  printf("Person: %s hat %d kg\n", moritz.name, moritz.weight);
  return 0;
}

// Tests array initialization with memset
// int arr[10] = {0}; also possible
// memset with 1 does not work as intended since it fills every byte with 1
int TestArray() {
  int arr[10];

  memset(arr, 0, sizeof(arr)); // Remove & because arr is already a pointer
  PrintHeader("initialized array with 0:");
  PrintIntArr(arr, sizeof(arr) / sizeof(int)); // Add len parameter

  // Writes 1 in every byte of the array bacause of that output 16843009
  memset(arr, 1, sizeof(arr)); // Remove & because arr is already a pointer
  PrintHeader("initialized array with 1:");
  PrintIntArr(arr, 10); // Remove & because arr is already a pointer

  return 0; // Add 0, but why an return value at all? on the test functions?
}

// reverses an string
static void PrintBackward(const char str[]) {
  int i = 0;
  int maxInx = strlen(str) - 1;
  for (i = maxInx; i >= 0; --i) {
    printf("%c", str[i]);
  }
  printf("\n");
}

static int comp(void const *const str1, void const *const str2) {
  return strcmp(*(char **)str1, *(char **)str2);
}

typedef void (*TFunc)(const char arr[]);

// Executes a function pointer for each element of an array
static void CallFuncPointer(TFunc func, const char *arr[]) {
  unsigned i = 0;
  for (; i < 4; i++) {
    func(arr[i]);
  }
}

// Tests function pointers
int TestFuncPtr() {

  const char *unsorted[] = {(char *)"Hello", (char *)"Martha", (char *)"Anton",
                            (char *)"Berta"};

  void (*func)(const char arr[]) = PrintLength;

  PrintHeader("Test Function Pointers");

  // Sorts length
  // sorts using qsort with the comp function as comparator
  qsort(unsorted, 4, sizeof(char *), comp);
  // Cast so constness is preserved
  PrintStrArr(unsorted, 4);

  CallFuncPointer(PrintBackward, unsorted);

  // give callback as variable
  CallFuncPointer(func, unsorted);

  return 0;
}
