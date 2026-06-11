#include "everything.h"
c pkgPrfx[] = "&+.-";

void readCfg() {} // temporarily here

size_t pkgArgType(c*inp) {
	size_t count = 1;
	size_t len = sizeof(pkgPrfx);
	while ((count<len)&&(inp[1]!=0)) {
		if (inp[0]==pkgPrfx[count-1]) {
			c*a = inp+1;
			while (isalnum(*a)) {
				a++;
				if (*a==0) return count;
			}
		}
		count++;
	}
	return 0;
}

bool isPkgArg(c*inp) {return pkgArgType(inp)>0;}

bool isPkg() {
	if (argc<2) return false;
	return isPkgArg(argv[1]);
}

int pkgMain() {
	puts("pkg mode activated");
	return 0;
}