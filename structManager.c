#include "structManager.h"


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


TAD_istruct load(TAD_istruct qs, int nsnaps, char *snaps_paths[]){

    xmlDocPtr doc;
    xmlNodePtr cur;

    while(--nsnaps){
        doc = xmlParseFile(*snaps_paths);

        if(!doc) fprintf(stderr, "Couldn't parse file\n");

        else{
            cur = xmlDocGetRootElement(doc);
            if(!cur) fprintf(stderr, "Empty xml document\n");
            else{
                cur = cur->xmlChildrenNode;
                qs = processPages(qs, cur, doc);                  
            }
            xmlFreeDoc(doc);
        } 
        ++snaps_paths;
    }
}