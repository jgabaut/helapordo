#!/bin/bash

# Define the source directory
SRC_DIR="src"

# Specify the path to the configuration file
CONFIG_FILE=".astylerc"

# List of source files
hlpd_SOURCES=("$SRC_DIR"/*.c "$SRC_DIR"/*.h)

for src in "${hlpd_SOURCES[@]}"; do
    # Create a temporary copy of the source file
    cp "$src" "$src.tmp"

    # Format the temporary file with astyle
    astyle --options="$CONFIG_FILE" "$src.tmp" >/dev/null

    # Check if the temporary file differs from the original file
    if ! diff -q "$src" "$src.tmp" &> /dev/null; then
        printf "\033[1;31m$src\e[0m needs formatting"
    fi

    # Clean up the temporary files
    rm -f "$src.tmp"
    rm -f "$src.tmp.orig"
done
