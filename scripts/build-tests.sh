#!/bin/bash
# Build and run tests
set -e

mkdir -p build

gcc -Wall -Wextra -Werror -std=c11 -pedantic \
    tests/test_runner.c \
    tests/unity/unity.c \
    -o build/test_runner \
    -lm

./build/test_runner
