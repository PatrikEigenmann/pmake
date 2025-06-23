#!/bin/sh

set -e

echo "Building pmake..."
mkdir -p bin
gcc -Wall -Wextra -std=c99 -Iinclude src/*.c -o bin/pmake

echo "Installing to /usr/local/bin/pmake (requires sudo)..."
sudo cp bin/pmake /usr/local/bin/

echo "Done. Type 'pmake' to begin."