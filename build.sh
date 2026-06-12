#!/bin/sh
# scavenge build script
# compatible with any posix-compliant shell + coreutils/busybox

test -z "$CC" && CC=tcc
test -z "$CFGDIR" && CFGDIR="/etc/scavenge/"
test -n "$CFLAGS" && CFLAGS="$CFLAGS "
CFLAGS="${CFLAGS:=""}-Oz -Wformat=2"

xxd -i cfg.txt > cfg.h
"$CC" main.c util.c cfg.c pkg.c -o scavenge -DETC="\"$CFGDIR\"" $CFLAGS
rm cfg.h