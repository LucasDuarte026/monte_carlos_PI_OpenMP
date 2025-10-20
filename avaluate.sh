#!/bin/bash

set -uo pipefail

output_file="reference.txt"
declare -a values=(10 100 1000 10000 100000 1000000 10000000)

: > "$output_file"

for N in "${values[@]}"; do
    {
        echo "*****************************************************"
        echo "test: N=$N"
    } >> "$output_file"

    if ./bin/reference "$N" >> "$output_file" 2>&1; then
        echo "result: ok" >> "$output_file"
    else
        exit_code=$?
        echo "result: error (exit code $exit_code)" >> "$output_file"
    fi
done
