#ifndef EVERYTHING_H
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

typedef unsigned char c;
extern size_t argc;
extern c**argv;

void readCfg();

bool isPkgArg(c*);
bool isPkg();
int pkgMain();
#endif