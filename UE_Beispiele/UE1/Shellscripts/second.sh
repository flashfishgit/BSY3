#!/bin/bash

#anderes script einbinden
source ./first.sh

echo "print first param: $0"

#exit status prufen (./first.sh)

if [ $? -eq 0 ]; then
  echo "run of $0 OK"
else
  echo "run of $0 NOK"
fi

#ein einfaches Menue
echo "make a choice:"
select opt in start stop finish; do
  case $opt in
  start) echo "started" ;;
  stop) echo "stopped" ;;
  finish) break ;;
  *) echo "wrong choice" ;;
  esac
done

#teste auf Existenz, Datei oder Verzeichnis
check() {
  if [ -f "$1" ]; then
    echo "$1 is a regular file"
  elif [ -d "$1" ]; then
    echo "$1 is a directory"
  else
    echo "$1 is not a file or directory"
  fi

  if [ ! -f "$1" ]; then
    echo "$1 does not exist"
  fi
}

check "first.sh"
check "/home"
check "/home/ttttt"
check
