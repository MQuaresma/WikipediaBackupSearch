#include "structManager.h"
#include <string.h>

TAD_istruct processPages(TAD_istruct qs, xmlNodePtr cur, xmlDocPtr doc){

	while(cur){												//percorre o doc xml na totalidade
		if(!xmlStrcmp(cur->name, (const xmlChar*)"page")){	//encontrou uma pagina
			qs->artTot ++;
			if(hashAdd(qs->articCollect, cur, doc)) qs->artUn++; 
        }
        cur = cur->next;
	}
	return qs;
}


 /*
  * Adiciona um artico à hashTable
  * @param articCollect apontador para hashTable
  * @param nodo apontador de nós do xml só de um artico
  * @return 0 se não adicionei, 1 se adicionei
  */
  int hashAdd(TAD_istruct st, xmlNodePtr nodo, xmlDocPtr doc){
 	    int success=0, found=0;
	    long ind, id;
	    xmlChar * title = NULL;
	    xmlNodePtr nodoRev = NULL;
            struct articleInfo *newArtic = NULL, *aux = NULL, *prev = NULL;
  
            nodo = nodo->xmlChildrenNode;
            if(nodo){
                  while(nodo){    
                          if(!xmlStrcmp(nodo->name,(const xmlChar*)"title")){ 
                                  title = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
                          }else if(!xmlStrcmp(nodo->name,(const xmlChar*)"id")){
				  xmlChar * name = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
				  if(name) sscanf(name,"%ld",&(id));
				  xmlFree(name);
			  }else if(!xmlStrcmp(nodo->name,(const xmlChar*)"revision")){
				  nodoRev = nodo;
                          }
                          nodo=nodo->next;
		  }

                  ind = hash(id,st->articCollect->size);
  		  for(aux=(st->articCollect)->table[ind]; aux && !found; aux=aux->next){
			  if(id==aux->id) found=1;
			  prev = aux;
		  } 
			
		  if(found){
			  if(title) aux->title = strdup(title);
			  aux->nRev += addRev(&(aux->revs),&(st->contribuitors),nodoRev,doc,&(aux->len),&(aux->words), aux->nRev);
		  }else{
			  newArtic = (struct articleInfo*)malloc(sizeof(struct articleInfo));
                          if(title) newArtic->title = strdup(title);
			  newArtic->id=id;
			  newArtic->nRev=0;
			  newArtic->nRev += addRev(&(newArtic->revs),&(st->contribuitors),nodoRev,doc,&(newArtic->len),&(newArtic->words), newArtic->nRev);
			  prev->next=newArtic;
		  	  success = 1; 
		  }
		  xmlFree(title);
            }
            return success;
    }

/*
 * Redimensiona a hashTable de articleInfo *
 * @param articCollect apontador para a hashTable
 */
 void resize(articTableP * articCollect){
          long elem = 0, i=0, hashVal;
          long newSize = 2*(*articCollect)->size;
          struct articleInfo **newP = (struct articleInfo **)calloc(newSize,sizeof(void *));
          struct articleInfo *aux = NULL, *auxI=NULL;
  
          for(i=0; i<(*articCollect)->size; i++){
                  for(aux=(*articCollect)->table[i]; aux; aux=aux->next){
  
                          hashVal = hash(aux->id,newSize);
                          for(auxI=newP[hashVal]; auxI && auxI->next; auxI=auxI->next);
  
                          if(auxI) auxI->next = aux;
                          else auxI = aux;
  
                          elem++;
                  }
          }
          free((*articCollect)->table);
          (*articCollect)->table = newP;
          (*articCollect)->size = newSize;
          (*articCollect)->racio = elem/newSize;
}


/*
 * Regista um novo contribuidor e revisao
 *
*/
int addRev(revDictP *dict, contribTreeP *tree, xmlNodePtr cur, xmlDocPtr doc, long *len, long *words, long nRev){
    
    xmlChar *temp, *userN;
    xmlNodePtr aux;
    int new = 1, grown=1;
    long i, idT; 

    cur = cur->xmlChildrenNode;
    while(cur && new){
        if(!xmlStrcmp(cur->name, (xmlChar *)"id")){                     //id found
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);
            if(temp){                                                   //non empty node
                sscanf(temp, "%ld", &idT);
                if((new = newEntry(*dict, nRev, idT))){
                    *dict = (struct revDict *)realloc(*dict, nRev+1);    
                    (*dict)[nRev].id = idT;
                }
                xmlFree(temp);
            }    
        }else if(!xmlStrcmp(cur->name, (xmlChar *)"timestamp")){        //add revision timestamp
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode,1);
            if(temp){                                                   //non empty node
                (*dict)[nRev].timeStamp = (xmlChar *)strdup(temp);    
                xmlFree(temp);
            }
        }else if(!xmlStrcmp(cur->name, (xmlChar*)"contributor")){
            aux = cur->xmlChildrenNode;
            while(aux){
                if(!xmlStrcmp(aux->name, (xmlChar *)"username")) userN = xmlNodeListGetString(doc, aux->xmlChildrenNode,1);    //found user
                else if(!xmlStrcmp(aux->name, (xmlChar *)"id")){                                                            //found user id
                    temp = xmlNodeListGetString(doc, aux->xmlChildrenNode,1);
                    if(temp){
                        sscanf(temp, "%ld", &idT);
                        xmlFree(temp);                     
                    }
                    addAVL(tree, idT, userN, &grown); 
                    xmlFree(userN);
                }    
                else if(!xmlStrcmp(aux->name, (xmlChar *)"text")){
                    temp = xmlNodeListGetString(doc, aux->xmlChildrenNode,1);
                    contarWL((char*)temp, len, words); 
                    xmlFree(temp);
                }
                aux = aux->next;
            }
            xmlFree(aux);
        }
        cur = cur->next;
    }

    return new;
}

void addAVL(contribTreeP *tree, long id, xmlChar *nome, int *new){

    if(*tree){
        if(*tree->id == id){ 
            (*tree)->nRev ++; 
            *new = 0;
        }
        else if(*tree->id > id) addAVLLeft(tree, id , nome, new);
        else addAVLRight(tree, id, nome, new);
    }else{
        *tree =(contribTreeP)malloc(sizeof(struct contribTreeP));
        *tree->id = id;
        *tree->nome = xmlStrdup(nome);
        *tree->nRev = 1;
        *tree->bal = EH;
        *tree->left = *tree->right = NULL;
        *new = 1;
    }
}


void addAVLRight(contribTreeP *tree, long id, xmlChar *nome, int *new){

    addAVL(&(*tree->right), id, nome, new);

    if(*new){
        switch(*tree->bal){
            case EH:
                *new = 1;
                *tree->bal = RH;
                break;
            case LH:
                *new = 0;
                *tree->bal = EH;
                break;
            case RH:
                balanceRight(tree);
                break;
        }
    }

}

void addAVLLeft(contribTreeP *tree, long id, xmlChar *nome, int *new){

    addAVL(&(*tree->left), id, nome, new);

    if(*new){
        switch(*tree->bal){
            case EH:
                *new = 1;
                *tree->bal = RH;
                break;
            case RH:
                *new = 0;
                *tree->bal = EH;
                break;
            case LH:
                balanceLeft(tree);
                break;
        }
    }

}

void balanceRight(contribTreeP *tree){

    contribTreeP aux;  
    
    if(*tree->right->bl == RH){
        aux = *tree;
        *tree = *tree->right;
        aux->right = *tree->left;
        *tree->left = aux;
        *tree->bl = EH;
        *tree->left->bl = EH;
    }else{
        aux = *tree->right;
        *tree->right = aux->left;
        aux->left = *tree->right->right;
        *tree->right->right = aux;
        aux =  = *tree;
        *tree = *tree->right;
        aux->right = *tree->left;
        *tree->left = aux;
        switch(*tree->bl){
            case LH: 
                *tree->left->bl = EH;                   
                *tree->right->bl = RH;                  
                break;
            case RH:
                *tree->left->bl = LH;                   
                *tree->right->bl = EH;                   
                break;
            case EH:
                *tree->left->bl = EH;                 
                *tree->right->bl = EH;                 
                break;
        }
        *tree->bl = EH;
    }

}

void balanceLeft(contribTreeP *tree){

    contribTreeP aux;  
    
    if(*tree->left->bl == LH){
        aux = *tree;
        *tree = *tree->left;
        aux->left = *tree->right;
        *tree->right = aux;
        *tree->bl = EH;
        *tree->left->bl = EH;
    }else{
        aux = *tree->left;
        *tree->left = aux->right;
        aux->right = *tree->left->left;
        *tree->left->left = aux;
        aux = *tree;
        *tree = aux->left;
        aux->left = *tree->right;
        *tree->right = aux;
        switch(*tree->bl){
            case LH: 
                *tree->left->bl = EH;                   
                *tree->right->bl = RH;                  
                break;
            case RH:
                *tree->left->bl = LH;                   
                *tree->right->bl = EH;                   
                break;
            case EH:
                *tree->left->bl = EH;                 
                *tree->right->bl = EH;                 
                break;
        }
        *tree->bl = EH;
    }

}

long hash(long id, long size){
	return id % size; 
}
