#include "everything.h"

size_t argc;
c**argv;

typedef int Cmd(void);

struct {
	c*name;
	Cmd*handler;
	c*summ;
} cmds[] = {
	{"list", listCmd, "lists all currently installed packages ig"}
};

int main(size_t a, c**b) {
	argc=a;
	argv=b;
	if (!(readCfg())) return 1;
	Cmd*handler = pkgMain;
	if (!(isPkg())) {
		size_t cLen = sizeof(cmds)/sizeof(cmds[0]);
		c*cmd = getCmd();
		if (cmd==NULL) {
			freeCfg();
			size_t count = 1;
			while (count<argc) {
				if ((strcmp(argv[count],"--help")==0)||(strcmp(argv[count],"-h")==0)) {
					printf(
						"scavenge package manager\n"
						"\n"
						"usage: %s <command> [...args]\n"
						" ....  %s <packageModeArgs...>\n"
						" ....  %s [--help] [--version]\n"
						"\n"
						"commands:\n"
						,argv[0],argv[0],argv[0]
					);
					size_t longest = 0;
					count = 0;
					while (count<cLen) {
						size_t nLen = strlen(cmds[count].name);
						if (nLen>longest) longest=nLen;
						count++;
					}
					c*fmt1 = " .......  %%%ds -- %%s\n";
					c*fmt2 = calloc(snprintf(NULL,0,fmt1,longest)+1,sizeof(c));
					sprintf(fmt2,fmt1,longest);
					count = 0;
					while (count<cLen) {
						printf(fmt2, cmds[count].name, cmds[count].summ);
						count++;
					}
					free(fmt2);
					puts(
						"\n"
						"package mode args help:\n"
						"\n"
						"there are 5 total actions. here's what they do:\n"
						"@pkg -- reinstalls a package. useful if you deleted a file from some package\n"
						"+pkg -- installs a package\n"
						"%pkg -- updates a package to the latest version\n"
						"=pkg -- purges a package, removing the package and it's config files"
						"-pkg -- removes a package, but keeps the config\n"
						"\n"
						"for each argument, you first type in the action prefix, then the package name"
					);
					break;
				}
				count++;
			}
			return 0;
		}
		bool foundCmd = false;
		size_t count = 0;
		while (count<cLen) {
			if (strcmp(cmds[count].name,cmd)==0) {
				foundCmd = true;
				handler = cmds[count].handler;
				break;
			}
			count++;
		}
		if (!(foundCmd)) {
			fprintf(stderr,"command '%s' not found",cmd);
			freeCfg();
			return 1;
		}
	}
	int code = handler();
	freeCfg();
	return code;
}