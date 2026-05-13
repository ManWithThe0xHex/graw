#!/usr/bin/bash
set -xe

CC=cc
CFLAGS="-Wall -Wextra"

$CC $CFLAGS -o graw main.c -l curl
