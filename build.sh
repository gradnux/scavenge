#!/bin/sh
# scavenge build script
# compatible with any posix-compliant shell + coreutils/busybox

test -z $CC && CC=tcc
test -z $CFGDIR && CFGDIR="/var/etc/scavenge/"
test -n $CFLAGS && CFLAGS="$CFLAGS "
CFLAGS="${CFLAGS:=""}-Oz"

xxd -i cfg.txt > cfg.h
"$CC" main.c pkg.c -o scavenge -DETC="$CFGDIR" $CFLAGS
rm cfg.h