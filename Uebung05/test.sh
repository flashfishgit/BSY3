#!/usr/bin/env bash

gcc -o append append.c -Wall -Wextra

echo "Run with no arguments:"

./append

echo "########################################"
echo "Run with one argument:"

./append test1.txt

echo "########################################"
echo "Run with three argument:"

./append test1.txt test2.txt test3.txt

echo "########################################"
echo "Run with non existent input file:"

./append nonExist.txt output.txt

echo "########################################"
echo "Run with a file without content (Input: test1.txt | Output: empty.txt):"
rm test1.txt
touch test1.txt #create empty input file
rm empty.txt
touch empty.txt

./append test1.txt empty.txt

echo "########################################"
echo "Run with indentical Input and Output file (Input: test1.txt | Output: test1.txt):"
# test1.txt already exists from previous step

./append test1.txt test1.txt

echo "########################################"
echo "Run with existent output file (Input: test2.txt | Output: output2.txt):"
rm test2.txt
echo -e "Line 1\nLine 2\nLine 3" >test2.txt
rm output2.txt
touch output2.txt #create empty output file

./append test2.txt output2.txt

echo "########################################"
echo "Run with existent output file and appent (Input: test3.txt | Output: output3.txt):"
rm test3.txt
echo -e "First Line\nSecond Line\nThird Line" >test3.txt
rm output3.txt
echo "Already inside" >output3.txt

./append test3.txt output3.txt

echo "########################################"
echo "Run with nonexistent output file (Input: test4.txt | Output: output4.txt):"
rm test4.txt
echo -e "Alpha\nBeta\nGamma" >test4.txt
rm output4.txt #ensure output file does not exist

./append test4.txt output4.txt

echo "########################################"
echo "Run with large output file (Input: test5.txt | Output: output5.txt):"
rm test5.txt

# print 1000 lines with 10 characters each to test5.txt
for ((i = 0; i < 1000; i++)); do
  echo "0123456789" >>test5.txt # print 11 characters per line
done

rm output5.txt

./append test5.txt output5.txt
