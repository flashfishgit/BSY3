#!/bin/bash
# Skript um das Programm ./write_file filename zu testen

# loeschen der generierten Dateien
# rm -f -> force...ignoiere Warnungen beim Loeschen
rm -f write_file hugo maxi

# erzeugen des Programms 
gcc -o write_file write_file.c -Wall


# teste das Programm
echo "create an error in the command line (no arguments) -> ./write_file" 
./write_file 

echo
echo "create an error in the command line (too many arguments) -> ./write_file hugo maxi"
./write_file hugo maxi

echo
echo "create file hugo and maxi:"
./write_file hugo
./write_file maxi

# liste die Ergebnisse auf
echo
echo "list all generated files:"
ls -l hugo maxi

# erzeuge die bestehenden Dateien erneut
echo
echo "create again existing files (-> error open file is expected):"
./write_file hugo
./write_file maxi

echo
echo "file content from hugo:"
cat hugo

echo
echo "file content from maxi:"
cat maxi

