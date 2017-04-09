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

TAD_istruct processPages(TAD_istruct qs, xmlNodePtr t, xmlDocPtr doc){

	while(cur){												//percorre o doc xml na totalidade
		if(!xmlStrcmp(cur->name, (const xmlChar*)"page")){	//encontrou uma pagina
			qs->artTot ++;
			if(hashAdd(qs->articCollect, cur, doc)) qs->artUn ++; 
        }
        cur = cur->next;
	}
	return qs;
}

void addAVL(contribTree **cT, long id, xmlChar *nome){

    

    while(*cT && id != *cT->id)
        cT = ((*cT->id > id) ? &(*cT->left) : &(*cT->left)); 
    if(!(*cT)){



    }

}

 /*
     * Adiciona um artico à hashTable
     * @param articCollect apontador para hashTable
     * @param nodo apontador de nós do xml só de um artico
     * @return 0 se não adicionei
     * @return 1 se adicionei
    */
int hashAdd(TAD_istruct st, xmlNodePtr nodo, xmlDocPtr doc){
            int success=0;
  
            nodo = nodo->xmlChildrenNode;
            if(nodo){
                  articInfo * newArtic;
                  while(nodo){    
                          if(xmlStrCmp(cur->name,(const xmlChar*))"title"){
                                  xmlChar * title = xmlNodeListGetString(doc,nodo->xmlChildrenNode, 1);
                                  if(title) strdup(newArtic->title,title);
                          }
  
                          if(xmlStrCmp(cur->name,(const xmlChar*))"id"){
                                  sscanf(nodo->xmlChildrenNode,long,&(newArtic->id));
                          }
  
                          if((xmlStrCmp(cur->name,(const xmlChar*))"revision")){
                                  newArtic->nRev++;
                                  addRev(newArtic->revs,st->contribuitors,nodo,doc,&(newArtic->len),&(newArtic->words));
                          }
                          nodo = nodo->next;
                  }
                  long ind = hash(newArtic->id,st->articCollect->size);
		  articInfo * aux = st->articCollect[ind];
  
                  while(aux && aux->next){
                          aux = aux->next;
                  }       
                  if(aux) aux->next = newArtic;
                  else aux = newArtic;
  
                  st->articCollect->size++;
                  st->articCollect->racio = size/HASHSIZE;
                  success = 1;
            }
            return success;
    }

/*
 * Redimensiona a hashTable de articleInfo *
 * @param articCollect apontador para a hashTable
 * @return hashTable redimensionada
 */
void resize(articTableP * articCollect){
          long elem = 0, i=0, hashVal;
          long newSize = 2*articCollect->size;
          struct articleInfo **newP = (struct articleInfo **)calloc(newSize,sizeof(void *));
          struct articleInfo *aux = NULL, *auxI=NULL;

          while(i<articCollect->size){
                  aux = articCollect->table[i];
                  while(aux){
                          hashVal = hash((*aux)->id,newSize);
                          auxI = newP[hashVal];

                          while(auxI && auxI->next)
                                  auxI = auxI->next;

                          if(auxI) auxI->next = aux;
                          else auxI = aux;

                          elem++;
                          aux = aux->next;
                  }
                  i++;
          }
          free(articCollect->table);
          articCollect->table = newP;
          articCollect->size = newSize;
          articCollect->racio = elem/newSize;
}

long all_articles(TAD_istruct qs){

    return qs->artTot;

}

long unique_articles(TAD_istruct qs){

    return qs->artUn;

}

long *top_10_contributors(TAD_istruct qs){

    return qs->top10Contr;

}
