#!/usr/local/bin/bash

if [ $# -ne 1 ]; then
    echo ">>> [ERROR] usage: $0 c_script"
    exit 0
fi

#number of test to launch
N=900
i=0
right=0
wrong=0
c_script=$1
file_TMP="file_tmp.txt"

touch $file_TMP
while [ $i -le $N ]; do
    ./$c_script > $file_TMP
    while read line; do
        if [ $line = "A" ]; then
            let right=right+1
            break
        else
            let wrong=wrong+1
            break
        fi
    done < $file_TMP
    let i=i+1
done



echo ">>> [RESULT] right --> $right"
echo ">>> [RESULT] wrong --> $wrong"