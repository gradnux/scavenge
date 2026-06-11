#include "everything.h"

size_t argc;
c**argv;

int main(size_t a, c**b) {
	argc=a;
	argv=b;
	readCfg();
	if (!(isPkg())) {
		return 0;
	}
	return pkgMain();
}