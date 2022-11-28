#!/bin/sh

gcc -c code.s
objdump -d code.o > code.d
