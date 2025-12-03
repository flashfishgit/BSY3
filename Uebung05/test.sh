#!/usr/bin/env bash

gcc -o ccheck ccheck.c -Wall -Wextra

echo "Run with no arguments:"

./ccheck

echo "########################################"
echo "Run with two argument:"

./ccheck test1.txt test2.txt

echo "########################################"
echo "Run with no existing file:"

./ccheck noexist.txt

echo "########################################"
echo "Run on test_00_error.txt:"

./ccheck test_00_error.txt >test_00_output.txt

echo "########################################"
echo "Run on test_01_error.txt"

./ccheck test_01_error.txt >test_01_output.txt

echo "########################################"
echo "Run on test_02_ok.txt"

./ccheck test_02_ok.txt >test_02_output.txt

echo "########################################"
echo "Test buffer overflow"
i=0
: >one_line.txt
while [ "$i" -lt 1000 ]; do
  printf 'A' >>one_line.txt
  i=$((i + 1))
done
printf '\n' >>one_line.txt

./ccheck one_line.txt

echo "########################################"
echo "Run on ccheck.c"

./ccheck ccheck.c >test_04_output.txt
