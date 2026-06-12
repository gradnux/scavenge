#include "everything.h"

LinkedArr*realApp(LinkedArr*onto,LinkedArr*new) {
	if (onto==NULL) return new;
	onto->next = realApp(new,onto->next);
	return onto;
}

LinkedArr*arrNew(void*val) {
	LinkedArr*new = calloc(1,sizeof(LinkedArr));
	new->val = val;
	return new;
}

LinkedArr*arrAppend(LinkedArr*onto,void*val) {
	return realApp(onto,arrNew(val));
}

void arrFree(LinkedArr*a) {
	if (a==NULL) return;
	arrFree(a->next);
	free(a);
}

size_t realSizeof(size_t acc, LinkedArr*a) {
	if (a==NULL) return acc;
	return realSizeof(acc+1,a->next);
}

size_t sizeofA(LinkedArr*a) {
	return realSizeof(0,a);
}