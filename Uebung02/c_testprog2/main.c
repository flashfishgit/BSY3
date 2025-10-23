
#include "Print.h" // Add the Header for TestFormatIO
#include "Test.h"  // Add the Header for function declarations

int main() {

  // Formatierte Ein- und Ausgabe: scanf, printf
  PrintResult("Format IO Test", TestFormatIO());

  // Stringoperationen
  PrintResult("String Test", TestString());

  // dynamischer Speicher
  PrintResult("Memory Test", TestDynMem());

  // Deklaration und Verwendung von Strukturen
  PrintResult("Struct Test", TestStruct());

  // Deklaration und Verwendung eines Feldes
  PrintResult("Array Test", TestArray());

  // Zeiger auf Funktionen
  PrintResult("FuncPtr Test", TestFuncPtr());

  return 0;
}
