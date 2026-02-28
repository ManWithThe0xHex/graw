#!/usr/bin/bash
set -xe

CC=cc
CFLAGS="-Wall -Wextra"

$CC $CFLAGS -o gitcurl main.c -l curl
