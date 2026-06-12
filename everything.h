#ifndef EVERYTHING_H
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#ifndef ETC
#define ETC "/etc/scavenge/"
#endif
#define CFGTXT ETC "cfg.txt"
#define REALLOC_ADD 50

typedef unsigned char c;
typedef struct LinkedArr {
	void*val;
	struct LinkedArr*next;
} LinkedArr;

extern size_t argc;
extern c**argv;

LinkedArr*arrNew(void*);
LinkedArr*arrAppend(LinkedArr*,void*);
void arrFree(LinkedArr*);
size_t sizeofA(LinkedArr*);

bool readCfg();
void freeCfg();
c*cfg(c*);
bool boolCfg(c*);

bool isPkgArg(c*);
bool isPkg();
int pkgMain();
#endif