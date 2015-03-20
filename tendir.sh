#!/bin/bash

echo ""
echo "[Generating 10 directories...]"

FILENAME="dirname.txt"

if [ -d $FILENAME ]; then
  echo "-->[dirname.txt] is a directory."
  echo ""
  exit 1
fi

if [ -e $FILENAME ]; then
  while read LINE
  do
    (mkdir $LINE)
    echo "mkdir $LINE"
  done < $FILENAME 
  echo "-->done."
  echo ""
  exit 1
fi

echo "-->file [dirname.txt] does not exist"
echo -->nothing done.
echo ""
