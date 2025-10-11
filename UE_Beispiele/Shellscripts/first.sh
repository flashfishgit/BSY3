#!/bin/bash

# Ein string und eine Zahl
name="Max"
number=4711

echo $name $number

echo "username:"
read user

if [ "$user" = "Max" ]; then
  echo "hello $user"
else
  echo "your are not $name"
fi

#c aehnliche for schleife
for ((i = 1; i <= 3; ++i)); do
  echo "number: $i"
done

#while schleife
count=1
while [ $count -le 3 ]; do
  echo "count: $count"
  count=$((count + 1))
done

#Funktionen
func_foo() {
  echo "param1: $0"
  echo "I am $1"
  echo "I am $2"
}

compare() {
  if [ "$1" = "$user" ]; then
    echo "equal"
  else
    echo "$1 and $user are not equal"
  fi
}

maniAString() {
  if [ -z "$1" ]; then
    echo empty
  else
    echo "$1 not empty"
    echo "${1:0:3}" # Ausgabe der ersten 3 Zeichen
  fi
}

func_foo "Anna" "hansi"
func_foo

compare "$name"

maniAString "$user"
maniAString
