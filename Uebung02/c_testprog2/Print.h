#ifndef PRINT_H
#define PRINT_H

void PrintResult(char const *const text, unsigned const errorCode);

void PrintHeader(char const header[]);

void PrintStrArr(char const *const *const arr, unsigned const len);

void PrintIntArr(int const *const arr, int len);

#endif
