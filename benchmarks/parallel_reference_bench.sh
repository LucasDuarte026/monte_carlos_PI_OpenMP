#!/bin/bash

set -uo pipefail

type="parallel_reference"


results_dir="./benchmarks/results"
output_file="$results_dir/${type}_benchmark.txt"

mkdir -p "$results_dir"
: > "$output_file"
echo "*********************** ${type} Analisys *********************************"

declare -a values=(10 100 1000 10000 100000 1000000) # 10000000 100000000 1000000000 10000000000)

for N in "${values[@]}"; do
    {
        echo "*****************************************************"
        echo "test: N=$N"
    } >> "$output_file"
    if ./bin/${type} "$N" >> "$output_file" 2>&1; then
        echo "result: ok" >> "$output_file"
        echo >> "$output_file"
        echo >> "$output_file"
    else
        exit_code=$?
        echo "result: error (exit code $exit_code)" >> "$output_file"
    fi
    
    echo "$N runned"
    
done
