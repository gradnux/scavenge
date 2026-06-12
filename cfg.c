#include "everything.h"
#include "cfg.h"

c*cfgKeys[] = {
	"where",
	"wget"
};
c*cfgVals[] = { // i can specify defaults here! neato
	"https://scavenge.onrender.com/repo/",
	"n"
};

bool isWS(c ch) {return((ch==32)||(ch==9));}
bool isNL(c ch) {return(ch==10);}

c*realSlurp(bool ws,c**inp,bool whle,...) {
	size_t len = REALLOC_ADD;
	size_t usedLen = 1;
	c*res = calloc(len,sizeof(c));
	if (res==NULL) {
		fprintf(stderr,"too less memory\n");
		return NULL;
	}
	res[0]=0;
	while (((ws?isWS:isNL)(**inp)==whle)&&(**inp!=0)) {
		if (len<usedLen+1) {
			len+=REALLOC_ADD;
			c*tmp = realloc(res,len);
			if (tmp==NULL) {
				free(res);
				fprintf(stderr,"too less memory\n");
				return NULL;
			}
			res=tmp;
		}
		res[usedLen-1]=**inp;
		res[usedLen++]=0;
		(*inp)++;
	}
	return res;
}

#define slurp(ws,inp,...) realSlurp(ws,inp,##__VA_ARGS__,true)

int name2idx(c*name) {
	size_t idx = 0;
	size_t len = sizeof(cfgKeys)/sizeof(cfgKeys[0]);
	while (true) {
		if (idx>=len) {
			fprintf(stderr,"unknown key '%s'\n",name);
			return -1;
		}
		if (strcmp(cfgKeys[idx],name)==0) return idx;
		idx++;
	}
}

bool parseLn(c*ln) {
	c*ptr = slurp(true,&ln);
	if (!(ptr)) return false;
	free(ptr);
	if (isNL(*ln)) return true;
	c*name = slurp(true,&ln,false);
	if (!(name)) return false;
	int idx = name2idx(name);
	free(name);
	if (idx<0) return true; // bail
	ptr = slurp(true,&ln);
	if (!(ptr)) return false;
	free(ptr);
	c*val = slurp(false,&ln,false);
	if (!(val)) return false;
	free(cfgVals[idx]);
	cfgVals[idx]=val;
	return true;
}

bool readCfg() {
	size_t count = 0;
	size_t len = sizeof(cfgVals)/sizeof(cfgVals[0]);
	while (count<len) {
		c*orig = cfgVals[count];
		c*freeable = calloc(strlen(orig)+1,sizeof(c));
		if (freeable==NULL) {
			fprintf(stderr,"too less memory\n");
			return false;
		}
		memcpy(freeable,orig,strlen(orig)); // last byte is still 0 therefore no need for me to copy the null term
		cfgVals[count] = freeable;
		count++;
	}
	FILE*cfgIO = NULL;
	bool triedCreate = false;
	while (cfgIO==NULL) {
		cfgIO = fopen(CFGTXT, "r");
		if (cfgIO==NULL) {
			switch (errno) {
				case ENOENT:
					if (triedCreate) {
						fprintf(stderr,"failed to create file '"CFGTXT"'\n");
						return false;
					}
					triedCreate=true;
					pid_t pid = fork();
					if (pid<0) {
						fprintf(stderr,"error while creating directory '"ETC"': %s\n",strerror(errno));
						return false;
					}
					if (pid==0) {
						execlp("mkdir","mkdir","-p",ETC,NULL);
						_exit(1);
					}
					int c;
					waitpid(pid, &c, 0);
					if (!((WIFEXITED(c))&&(WEXITSTATUS(c)==0))) {
						fprintf(stderr,"failed to create directory '"ETC"'\n");
						return false;
					}
					int createFd = open(CFGTXT, O_WRONLY | O_CREAT, 420);
					if (createFd==-1) {
						fprintf(stderr,"error while creating file '"CFGTXT"': %s\n",strerror(errno));
						return false;
					}
					int thing = write(createFd,cfg_txt,cfg_txt_len);
					(void)thing;
					close(createFd);
					break;
				default:
					fprintf(stderr,"error while opening file '"CFGTXT"': %s\n",strerror(errno));
					return false;
			}
		}
	}
	size_t usedLen = 1;
	bool comment = false;
	len = REALLOC_ADD;
	c*ln = calloc(len,sizeof(c));
	int ch;
	if (ln==NULL) {
		fprintf(stderr,"too less memory\n");
		return false;
	}
	ln[0]=0;
	while ((ch=fgetc(cfgIO))!=EOF) {
		if (len<usedLen+1) {
			len+=REALLOC_ADD;
			c*tmp = realloc(ln,len*sizeof(c));
			// no memset because i add in null term automatically
			if (tmp==NULL) {
				free(ln);
				fprintf(stderr,"too less memory\n");
				return false;
			}
			ln=tmp;
		}
		comment = ((comment)||(ch==35));
		if (!(comment)) {
			ln[usedLen-1]=ch;
			ln[usedLen]=0;
		}
		usedLen+=(!(comment));
		if (ch==10) {
			if (!(comment)) parseLn(ln);
			comment=false;
			ln[0]=0;
			usedLen=1;
		}
	}
	if (ferror(cfgIO)) {
		fprintf(stderr,"error while reading file '"CFGTXT"': %s\n",strerror(errno));
		return false;
	}
	if ((usedLen>1)&&(!(comment))) parseLn(ln);
	free(ln);
	return true;
}

void freeCfg() {
	size_t count = 0;
	size_t len = sizeof(cfgVals)/sizeof(cfgVals[0]);
	while (count<len) free(cfgVals[count++]);
}

c*cfg(c*name) {
	int idx = name2idx(name);
	if (idx<0) return NULL;
	return cfgVals[idx];
}

bool boolCfg(c*name) {
	c*raw = cfg(name);
	return ((raw[1]==0)&&((raw[0]==121)||(raw[0]==89)));
}