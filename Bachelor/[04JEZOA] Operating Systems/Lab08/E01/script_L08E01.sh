#!/bin/bash

search () {

    # Chekcing the number of arguments
    if [ $# -ne 3 ]; then
        echo ">>> [ERROR] Usage: $0 dir fun out"
        exit 1
    fi

    # Checking the arguments
    if [ ! -d $1 ]; then
        echo ">>> [ERROR] $1 should be an existing directory."
        exit 1
    fi

    # VARIABLES
    local dir_SRC=$1
    local function_TARGET=$2
    local file_DST=$3
    local dir_SUB

    for target in $(ls $dir_SRC); do
        dir_SUB="$dir_SRC/$target"
        echo $dir_SUB
        if [ -d $dir_SUB ]; then
            search $dir_SUB $function_TARGET $file_DST
        elif [ -f $dir_SUB ]; then
            check_occurrence $dir_SUB $function_TARGET $file_DST
        fi
    done

    return 0
}

check_occurrence () {
    local target=$1
    local function=$2
    local output=$3
    local n=1 # Counting the number of lines

    while read line; do
        if grep -q "$function" <<< "$line"; then
            local file=$(basename $target)
            echo "[file: $file line: $n] $line" >> $output
        fi
        let n=n+1
    done < $target
}

# Creating the file
touch $3

# Running script
search $1 $2 $3

exit 0