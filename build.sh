#!/bin/sh
# scavenge build script
# compatible with any posix-compliant shell + coreutils/busybox

test -z "$CC" && CC=tcc
test -z "$CFGDIR" && CFGDIR="/etc/scavenge/"
test -n "$CFLAGS" && CFLAGS="$CFLAGS "
CFLAGS="${CFLAGS:=""}-Oz"

xxd -i cfg.txt > cfg.h
"$CC" main.c util.c cfg.c pkg.c cmds.c -o scavenge -DETC="\"$CFGDIR\"" $CFLAGS
rm cfg.h