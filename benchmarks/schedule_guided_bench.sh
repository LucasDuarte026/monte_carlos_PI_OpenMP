#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
# Treat unset variables as an error.
# The return value of a pipeline is the status of the last command to exit with a non-zero status.
set -uo pipefail

# Define the program type to be compiled and executed
type="schedule_guided"

# Compile the specified program
make ${type}

# Maximum of interations N = 10^max
max=11
if [ -z "$1" ]; then
        echo "Please provide a directory suffix (e.g., exec.sh 1)"
        exit 1
fi

# Create the results directory path based on the argument
results_dir="./benchmarks/results$1"
output_file="$results_dir/${type}_benchmark.txt"

# Create the results directory if it doesn't exist and clear the output file
mkdir -p "$results_dir"
: > "$output_file"

echo "*********************** ${type} Analisys *********************************"

# Loop through exponents from 1 to 10 to generate N values
for ((expo=1; expo<=${max}; expo++)); do
    # Calculate N as 10 to the power of the exponent using arithmetic expansion
    N=$((10**expo))

    # Write the test header to the output file with the new format
    {
        echo "*****************************************************"
        # The print format is updated as requested
        echo "test: 10^${expo} : ${N}"
    } >> "$output_file"
    
    # Execute the program with the current N, redirecting all output to the file
    if ./bin/${type} "$N" >> "$output_file" 2>&1; then
        echo "result: ok" >> "$output_file"
        echo >> "$output_file"
    else
        # If the program fails, record the exit code
        exit_code=$?
        echo "result: error (exit code $exit_code)" >> "$output_file"
    fi
    
    # Print progress to the console
    echo "Exponent 10^${expo} (N=${N}) runned"
    
done