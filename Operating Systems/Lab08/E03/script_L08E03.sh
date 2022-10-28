#!/bin/bash

# Checking number of arguments
if [ $# -ne 1 ]; then 
    echo ">>> [ERROR] Usage: $0 directory"
    exit 1
fi
if [ ! -d $1 ]; then 
    echo ">>> [ERROR] $1 should be a directory"
    exit 1
fi

# VARIABLES
directory=$1

for file in $(ls $directory); do
    file_tmp=$(echo $file | tr '[:upper:]' '[:lower:]' )
    mv $directory/$file $directory/$file_tmp
done

exit 0