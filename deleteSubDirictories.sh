#!/bin/bash

# Dlelete sub direcories only if the sub directory is empty.

# get the names of the sub directories.
SUBDIRs=`ls -l | egrep '^d' | awk '{print $9}'`

# convert the string of sub dir names into array
DIRarr=($SUBDIRs)

# Declare the result variable
RESULT=0

echo ""
echo "Deleting empty sub directoris..."
echo "This directory has [${#DIRarr[@]}] sub directoris."
echo ""

if [ ${#DIRarr[@]} -eq 0 ]; then
  echo "Nothing to delete. Abort."
  echo ""
  exit 1
fi


#echo "$SUBDIRs"
for DIR in $SUBDIRs
do
  echo "--> Deleting [${DIR}]..."
  if [ "$(ls -A $DIR)" ]; then #check the directory is empty or not.
    echo "    $DIR is not an empty directory... Pass."
  else
    echo "    $DIR is an empty directory..."
    (rmdir ./$DIR)
    let RESULT=RESULT+1
    echo "    $DIR has deleted."
  fi
  echo ""
done

echo "Total deleted directories: ${RESULT}"
echo "done."
