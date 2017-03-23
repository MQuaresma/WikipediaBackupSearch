#include <interface.h>

#define hashSize 100013

struct revDict{
	long id;
	char *timeStamp;
};

struct articleInfo {
	long id;
	char *title;
	struct revDict *revs;
	long nRev;
	long len;
	long words;
};

struct contribTree{
	long id;
	int nRev;
	struct contribTree *left;
	struct contribTree *right;
};

struct articTable{
	int racio;
	long size;
	struct articleInfo **table;
};

struct TCD_istruct {
	long artUn, artTot;
	struct articTable *articCollect;
	struct contribTree *contribuitors;
	long top10Contr[10];
};

long hashFunc(long id, long size){
	return id % size; 
}
