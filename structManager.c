#include "structManager.h"


TAD_istruct init(){

    TAD_istruct res = (TAD_istruct)malloc(sizeof(struct TCD_istruct));

    if(res){
        res->artUn = res->artTot = 0L;
        res->contribuitors = NULL;
        res->articCollect = (articTableP)malloc(sizeof(struct articTable));
        if(res->articCollect){
            res->articCollect->size = HASHSIZE;
            res->articCollect->nArt = 0.L;
            res->articCollect->table = (struct articleInfo**)calloc(HASHSIZE, sizeof(void*));
        }
    }
    
    return res;
}


TAD_istruct load(TAD_istruct qs, int nsnaps, char *snaps_paths[]){

    xmlDocPtr doc;
    xmlNodePtr cur;

    while(nsnaps--){
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

    return qs;
}

void cleanContribTree(contribTreeP a){
        xmlFree(a->nome);
        if(a->left) cleanContribTree(a->left);
        if(a->right) cleanContribTree(a->right);
        free(a);
}

TAD_istruct clean(TAD_istruct qs){
    long i=0,j=0;
          articleInfoP aux = NULL, next = NULL;

          for(i=0; i<(qs->articCollect)->size; i++){
                  for(aux=(qs->articCollect)->table[i]; aux; aux=next){
                        next = aux->next;
                        xmlFree(aux->title);
                        for(j=0;j<aux->nRev;j++) xmlFree((aux->revs[j]).timeStamp);
                        free(aux->revs);
                        free(aux);
                  }
          }
          free((qs->articCollect)->table);
          free(qs->articCollect);

          cleanContribTree(qs->contribuitors);
          free(qs);
          qs=NULL;
          return qs;
}
