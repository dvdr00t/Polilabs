#!/bin/bash

# CHecking number of arguments
if [ $# -ne 1 ]; then
    echo ">>> [ERROR] Usage: $0 filename"
    exit 1
fi
if [ ! -f $1 ]; then
    echo ">>> [ERROR] Usage: $1 should be a file"
    exit 1
fi

# VARIABLES
filename=$1
longest_size=0
tmp_size=0
file_length=$(wc -l $filename | tr -s " " | cut -d " " -f 2)

# Searching longest line
while read line; do
    tmp_size=$(echo -n $line | wc -c | tr -s " " | cut -d " " -f 2)
    if [ $tmp_size -ge $longest_size ]; then
        longest_size=$tmp_size
        longest_line=$line
    fi
done < $filename

# Printing results
echo "$filename has $file_length lines."
echo "Longest line is $longest_line with $longest_size characters."

exit 0


