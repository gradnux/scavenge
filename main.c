#include "everything.h"

size_t argc;
c**argv;

int main(size_t a, c**b) {
	argc=a;
	argv=b;
	if (!(readCfg())) return 1;
	if (!(isPkg())) {
		c*cmd = argv[1];
		
		freeCfg();
		return 0;
	}
	int code = pkgMain();
	freeCfg();
	return code;
}