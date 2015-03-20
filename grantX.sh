#!/bin/bash

echo
echo [Grant X to current user...]
if [ $# -ne 1 ]; then
  echo "[invalid parameter;;;]"
  echo "[usage: $0 <filename>]"
  echo ""
  exit 1
else
  FILENAME=$1
fi

if [ -e $FILENAME ]; then
  echo chmod u+x $1
  $(chmod u+x $1)
  echo [granted.]
  echo
else
  echo "[$FILENAME doesn't exist...]"
fi

echo ""
