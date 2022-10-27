#!/usr/local/bin/bash

# Checking arguments
if [ $# -ne 2 ]; then
    echo ">>> [ERROR] Usage: $0 process_pid time_interval"
    exit 1
fi

# VARIABLES
STATUS_DEFINED="Z"
process_pid=$1
time_interval=$2
counter=0

# TRUE LOOP
while [ 1 -eq 1 ]; do
    # Retrieving process information
    process_information=$(ps -el | tr -s " " | cut -d " " -f 3,12 | grep $process_pid)
    if [ $? -eq 0 ]; then
        # Checking if the process is a STATUS_DEFINED process and how long it is remaining a STATUS_DEFINED process
        status=$(echo $process_information | cut -d " " -f 2)
        echo "... checking status ($status) ..."
        if [ "$status" = "$STATUS_DEFINED" ]; then
            let counter=counter+1
            if [ $counter -ge 5 ]; then
                kill $process_pid
                echo "... Process $process_pid has been successfully killed ..."
                exit 0
            fi
        else
            let counter=0
        fi

        sleep $time_interval        
    fi
done

exit 0

