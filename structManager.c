#include <interface.h>
#include <stdlib.h>

#define HASHSIZE 100013

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

typedef struct articTable{
	int racio;
	long size;
	struct articleInfo **table;
}*articTableP;

struct contribTree{
	long id;
	int nRev;
	struct contribTree *left;
	struct contribTree *right;
};

struct TCD_istruct {
	long artUn, artTot;
	articTableP articCollect;
	struct contribTree *contribuitors;
	long top10Contr[10];
};

long hash(long id, long size){
	return id % size; 
}

TAD_istruct init(){

    TAD_istruct res = (TAD_istruct)malloc(sizeof(struct TCD_istruct));

    if(res){
        res->artUn = res->artTot = 0L;
        res->contribuitors = NULL;
        res->articCollect = (articTableP)malloc(sizeof(struct articTable));
        res->articCollect->size = HASHSIZE;
        res->articCollect->racio = 0;
        res->articCollect->table = (struct articleInfo**)calloc(HASHSIZE, sizeof(void*));
    }
    
    return res;
}
