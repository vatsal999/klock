#!/bin/sh

set -xe

gcc clock.c -lm -lncurses -o clock
