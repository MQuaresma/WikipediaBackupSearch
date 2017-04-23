#include "structManager.h"
#include <string.h>

int newEntry(revDictP dict, long nRev, long idT){

    int i;

    for(i = 0; i < nRev && dict[i].id != idT; i ++);

    return (i == nRev);

}

// Conta o número de palavras total e o número de caracteres total de uma String.
void contarWL(char s[], long *car, long *pal){

    int i=0, p=0;

    while(s[i]){
        while(s[i] && (s[i] == ' ' || s[i] == '\t'  || s[i] == '\n')) i ++;
        if(s[i]) p++; 
        while(s[i] && s[i] != ' ' && s[i] != '\t'  && s[i] != '\n') i ++;
    }

    *pal = p;
    *car = i;

}


TAD_istruct processPages(TAD_istruct qs, xmlNodePtr cur, xmlDocPtr doc){

	while(cur){												//percorre o doc xml na totalidade
		if(!xmlStrcmp(cur->name, (const xmlChar*)"page")){	//encontrou uma pagina
			qs->artTot ++;
			if(hashAdd(qs, cur, doc)) qs->artUn++; 
        }
        cur = cur->next;
	}
	return qs;
}


/*
* Adiciona um artico à hashTabale
* @param articCollect apontador para hashTabale
* @param nodo apontador de nós do xml só de um artico * @return 0 se não adicionei, 1 se adicionei
*/
int hashAdd(TAD_istruct st, xmlNodePtr nodo, xmlDocPtr doc){

    int success=0, found=0;
    long ind, id;
    xmlChar * title = NULL;
    xmlNodePtr nodoRev = NULL;
    articleInfoP newArtic = NULL, aux = NULL, prev = NULL;

    nodo = nodo->xmlChildrenNode;
    if(nodo){
        while(nodo){    
            if(!xmlStrcmp(nodo->name,(const xmlChar*)"title")) title = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
            else if(!xmlStrcmp(nodo->name,(const xmlChar*)"id")){
                xmlChar * name = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
                sscanf((char*)name,"%ld",&(id));
                if(name)xmlFree(name);
            }else if(!xmlStrcmp(nodo->name,(const xmlChar*)"revision")) nodoRev = nodo;

            nodo=nodo->next;
        }

        ind = hash(id,st->articCollect->size);
        for(aux=(st->articCollect)->table[ind]; aux && !(found=(id==aux->id)); prev = aux, aux=aux->next);

        if(found){
            if(title) aux->title = xmlStrdup(title);
            aux->nRev += addRev(&(aux->revs),&(st->contribuitors),nodoRev,doc,&(aux->len),&(aux->words), aux->nRev);
        }else{
            newArtic = (articleInfoP)malloc(sizeof(struct articleInfo));
            if(newArtic){
                if(title) newArtic->title = xmlStrdup(title);
                newArtic->id=id;
                newArtic->nRev=0;
                newArtic->len = newArtic->words = 0L;
                newArtic->revs = NULL;
                newArtic->nRev += addRev(&(newArtic->revs),&(st->contribuitors),nodoRev,doc,&(newArtic->len),&(newArtic->words), newArtic->nRev);
                newArtic->next = NULL;
                if(prev)prev->next=newArtic;
                else st->articCollect->table[ind] = newArtic;
                (st->articCollect->nArt) ++;
                if((st->articCollect->nArt/(float)st->articCollect->size) >= 0.8f) resize(st->articCollect); //redimensionar tabela quando ocupaçao exceder 0.8
                success = 1; 
            }    
        }
        if(title)xmlFree(title);
    }
    return success;
}

/*
 * Redimensiona a hashTabale de articleInfo *
 * @param articCollect apontador para a hashTabale
 */
void resize(articTableP articCollect){
          long elem = 0, i=0, hashVal;
          long newSize = 2*articCollect->size;
          articleInfoP *newP = (articleInfoP *)calloc(newSize,sizeof(void *));
          articleInfoP aux = NULL, auxI=NULL;
  
          for(i=0; i<articCollect->size; i++){
                  for(aux=articCollect->table[i]; aux; aux=auxI){
                          hashVal = hash(aux->id,newSize);
                          auxI = aux->next;
                          aux->next = newP[hashVal];
                          newP[hashVal] = aux;
                          elem++;
                  }
          }
          free(articCollect->table);
          articCollect->table = newP;
          articCollect->size = newSize;
          articCollect->nArt = elem;
}


/*
 * Regista um novo contribuidor e revisao
 *
*/
int addRev(revDictP *dict, contribTreeP *tree, xmlNodePtr cur, xmlDocPtr doc, long *len, long *words, long nRev){
    
    xmlChar *temp, *userN;
    xmlNodePtr aux = NULL;
    int new = 1;
    long idT, lenAux = 0L, wordsAux=0L; 
    revDictP auxDict = NULL;
    
    cur = cur->xmlChildrenNode;
    while(cur && new){
        if(!xmlStrcmp(cur->name, (xmlChar *)"id")){                     //id found
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(temp){                                                   //non empty node
                sscanf((char*)temp, "%ld", &idT);
                xmlFree(temp);
                if((new = newEntry(*dict, nRev, idT))){
                    auxDict = (revDictP)calloc(nRev+1, sizeof(struct revDict));
                    if(auxDict){
                        for(int i = 0; i < nRev; i ++) auxDict[i] = (*dict)[i];
                        if(*dict) free(*dict);
                        *dict = auxDict;    
                        ((*dict)[nRev]).id = idT;
                    }
                }
            }    
        }else if(!xmlStrcmp(cur->name, (xmlChar *)"timestamp") && new){        //add revision timestamp
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(temp){                                                   //non empty node
                ((*dict)[nRev]).timeStamp = xmlStrdup(temp);    
                xmlFree(temp);
            }
        }else if(!xmlStrcmp(cur->name, (xmlChar*)"contributor")){
            aux = cur->xmlChildrenNode;
            while(aux){
                if(!xmlStrcmp(aux->name, (xmlChar *)"username")) userN = xmlNodeListGetString(doc, aux->xmlChildrenNode, 1);    //found user
                else if(!xmlStrcmp(aux->name, (xmlChar *)"id")){                                                            //found user id
                    temp = xmlNodeListGetString(doc, aux->xmlChildrenNode, 1);
                    if(temp){
                        sscanf((char*)temp, "%ld", &idT);
                        xmlFree(temp);                     
                    }
                    addBTree(tree, idT, userN); 
                    if(userN)xmlFree(userN);
                }    
                aux = aux->next;
            }    
            if(aux) xmlFree(aux);            
        }else if(!xmlStrcmp(cur->name, (xmlChar *)"text")){
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(temp){
                contarWL((char*)temp, &lenAux, &wordsAux); 
                xmlFree(temp);
                if(lenAux > *len) *len = lenAux;
                if(wordsAux > *words) *words = wordsAux;
            }
        }
        cur = cur->next;
    }

    return new;
}

void addBTree(contribTreeP *tree, long id, xmlChar *nome){

    while(*tree && (*tree)->id != id){
        if((*tree)->id > id) tree = &((*tree)->left);
        else tree = &((*tree)->right);
    }
    if((*tree) && (*tree)->id == id) (*tree)->nRev ++; 
    else{
        *tree =(contribTreeP)malloc(sizeof(struct contribTree));
        (*tree)->id = id;
        (*tree)->nome = xmlStrdup(nome);
        (*tree)->nRev = 1;
        (*tree)->left = (*tree)->right = NULL;
    }
}

long hash(long id, long size){
	return id % size; 
}
