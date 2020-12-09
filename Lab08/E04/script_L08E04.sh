#!/usr/local/bin/bash

# Checking arguments
if [ $# -eq 0 ]; then
    read filename
elif [ $# -eq 1 -a ! -f $1 ]; then
    echo ">>> [ERROR] $1 should be a filename"
    exit 1
elif [ $# -ge 2 ]; then
    echo ">>> [ERROR] Usage $0 [filename]"
    exit 1
else 
    filename=$1
fi

# VARIABLES
# Use an associative array to store word counts
declare -A words_to_freq

for word in $(cat $filename); do
    freq=${words_to_freq[$word]}
    words_to_freq[$word]=$((freq+1))
done

# DISPLAYING ALL VALUES
# array[*]  means: get all array values (i.e., element values)
# !array[*] means: get all array index values (i.e., index values)
for word in ${!words_to_freq[*]}; do
  echo "$word ${words_to_freq[$word]}"
done

# Delete the array
unset array

exit 0

