#include "everything.h"

#define ACTIONS 6
c pkgPrfx[ACTIONS-1] = "@+%=-";
c*verbs[ACTIONS] = {
	NULL,
	"reinstalling",
	"installing",
	"updating",
	"purging",
	"removing"
};
LinkedArr*actions[ACTIONS] = {NULL};

size_t pkgArgType(c*inp) {
	size_t count = 0;
	size_t len = sizeof(pkgPrfx);
	while ((count<len)&&(inp[1]!=0)) {
		if (inp[0]==pkgPrfx[count]) {
			c*a = inp+1;
			while (isalnum(*a)) {
				a++;
				if (*a==0) return count+1;
			}
		}
		count++;
	}
	return 0;
}

bool isPkgArg(c*inp) {return pkgArgType(inp)>0;}

bool isPkg() {
	if (argc<2) return false;
	size_t okay = 1;
	size_t count = 1;
	while (count<argc) {
		okay += isPkgArg(argv[count]);
		count++;
	}
	if (okay==argc) return true;
	size_t mistyped = argc-okay;
	if (okay>1) fprintf(stderr,"note: you may have mistyped %zu arg%s\n",mistyped,(mistyped>1)?"s":"");
	return false;
}

LinkedArr*filterArgvByType(size_t type) {
	LinkedArr*res = NULL;
	size_t count = 1;
	while (count<argc) {
		if (pkgArgType(argv[count])==type) res=arrAppend(res,argv[count]+1); // wow so convenient!
		count++;
	}
	return res;
}

void freeActions() {
	size_t count = 0;
	while (count<ACTIONS) arrFree(actions[count++]);
}

int pkgMain() {
	if (!(boolCfg("wget"))) fprintf(stderr,"note: aria2c is currently unsupported\n");
	LinkedArr*all = NULL;
	bool dup = false;
	bool prant = false;
	size_t count = 0;
	while (count<ACTIONS) {
		LinkedArr*root = filterArgvByType(count);
		actions[count] = root;
		size_t len = sizeofA(actions[count]);
		if (len>0) {
			if (!(dup)) {
				while (root!=NULL) {
					LinkedArr*allIter = all;
					while (allIter!=NULL) {
						if (strcmp(allIter->val,root->val)==0) {
							dup = true;
							break;
						}
						allIter = allIter->next;
					}
					if (dup) break;
					all = arrAppend(all,root->val);
					root = root->next;
				}
			}
			printf("%s%s %zu package%s",prant?", ":"",verbs[count],len,(len>1)?"s":"");
			prant = true;
		}
		count++;
	}
	if (prant) puts("...");
	arrFree(all);
	if (dup) {
		fprintf(stderr,"found conflicting actions for one (or more) package name(s)\n");
		freeActions();
		return 1;
	}
	freeActions();
	return 0;
}