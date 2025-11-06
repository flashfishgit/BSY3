#!/bin/bash
# Skript um das Programm ./readWrite zu testen

# loeschen der generierten Dateien
# rm -f -> force...ignoiere Warnungen beim Loeschen
rm -f readWrite *.txt

echo "generate program in release version:"
gcc -o readWrite ReadAndWrite.c -Wall

echo "generate program in debug version:"
gcc -o readWrite.db ReadAndWrite.c -Wall -ggdb3

# teste die Kommandozeile
echo "test command line and usage:"
./readWrite
./readWrite a b

# teste das Programm
echo "call program readWrite..."
./readWrite pers_data.txt

echo
echo "list generated files"
ls -l readWrite readWrite.db pers_data.txt

echo
echo "output generated file:"
cat pers_data.txt
echo
