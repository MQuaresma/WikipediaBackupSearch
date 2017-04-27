#include "structManager.h"
#include <string.h>

/**
Verifica se a chave(id) dada já se encontra no dicionário
@param dict Dicionário com par id, timestamp da revisao
@param nRev nro de entradas presentes no dicionário
@param idT chave a procurar no dicionário
@return 1 se nao se encontrar, 0 caso contrario
 */
int newEntry(revDictP dict, long nRev, long idT){

    int i;

    for(i = 0; i < nRev && dict[i].id != idT; i ++);

    return (i == nRev);

}

/** 
Conta o número de palavras total e o número de caracteres total do array de caracteres dado
@param s string a percorrer
@param car apontador para o long que guarda o nro de caracteres
@param pal apontador para o long que guarda o nro de palavras
 */
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

/**
Percorre o ficheiro xml e processa as paginas encontradas (usando a tag page)
@param qs estrutura que guarda a informação ja recolhida relativa aos snapshots
@param cur apontador para os nodos que se encontram aos niveis das paginas
@param doc Apontador para o ficheiro xml a ser procesado
@return Devolve a estrutura que resulta de adicionar a informaçao presente no snapshot atual
 */
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


/**
Adiciona um artigo à hashTable
@param articCollect Apontador para hashTable
@param nodo Apontador de nós do xml só de um artico 
@return 0 se não foi adicionado um novo artigo, 1 caso contrário
 */
int hashAdd(TAD_istruct st, xmlNodePtr nodo, xmlDocPtr doc){

    int success=0, found=0;
    long ind, id;
    xmlChar *title = NULL;
    xmlNodePtr nodoRev = NULL;
    articleInfoP newArtic = NULL, aux = NULL, prev = NULL;

    nodo = nodo->xmlChildrenNode;
    if(nodo){
        while(nodo){    
            if(!xmlStrcmp(nodo->name,(const xmlChar*)"title")) title = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
            else if(!xmlStrcmp(nodo->name,(const xmlChar*)"id")){
                xmlChar *name = xmlNodeListGetString(doc, nodo->xmlChildrenNode, 1);
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

/**
Redimensiona a hashTable
@param articCollect Apontador para a hashTable
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


/**
Regista uma nova contrubuição e revisao
@param dict Dicionário com par id, timestamp da revisao
@param tree Árvore binária com os contribuidores ja encontrados
@param nodo Apontador para a tag do artigo(page)
@param doc Apontador para o ficheiro xml a ser procesado
@param len Apontador para o (maior) tamanho do artigo atual
@param words Apontador para o (maior) nro de palavras do artigo atual
@param nRev Nro de revisoes, ja conhecidas, do artigo atual
@return 1 caso se trate de uma nova revisao, 0 caso contrario
 */
int addRev(revDictP *dict, contribTreeP *tree, xmlNodePtr cur, xmlDocPtr doc, long *len, long *words, long nRev){
    
    xmlChar *temp, *userN;
    xmlNodePtr aux = NULL;
    int new = 1;
    long idT, lenAux = 0L, wordsAux=0L; 
    revDictP auxDict = NULL;
    
    cur = cur->xmlChildrenNode;
    while(cur && new){                                                  //para caso a revisao ja tenha sido adicionada
        if(!xmlStrcmp(cur->name, (xmlChar *)"id")){                     //id encontrado
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(temp){                                                 
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
        }else if(!xmlStrcmp(cur->name, (xmlChar *)"timestamp") && new){        
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
            if(temp){                                                   
                ((*dict)[nRev]).timeStamp = xmlStrdup(temp);    
                xmlFree(temp);
            }
        }else if(!xmlStrcmp(cur->name, (xmlChar*)"contributor")){
            aux = cur->xmlChildrenNode;
            while(aux){
                if(!xmlStrcmp(aux->name, (xmlChar *)"username")) userN = xmlNodeListGetString(doc, aux->xmlChildrenNode, 1);    //user encontrado
                else if(!xmlStrcmp(aux->name, (xmlChar *)"id")){                                                                //id user encontrado
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

/**
Adiciona um contribuidor à árvore binária de procura
@param tree Apontador para a árvore binária
@param id Id do contribuidor a adicionar
@param nome Nome do contribuidor a adicionar
 */
void addBTree(contribTreeP *tree, long id, xmlChar *nome){

    while(*tree && (*tree)->id != id){
        if((*tree)->id > id) tree = &((*tree)->left);
        else tree = &((*tree)->right);
    }
    if((*tree) && (*tree)->id == id) (*tree)->nRev ++;  //ja existe logo adiciona nova revisao/contribuiçao
    else{
        *tree =(contribTreeP)malloc(sizeof(struct contribTree));
        (*tree)->id = id;
        (*tree)->nome = xmlStrdup(nome);
        (*tree)->nRev = 1;
        (*tree)->left = (*tree)->right = NULL;
    }
}

/**
Funçao de hash dos id's de artigos
@param id Valor a ser usado para o calculo do hash
@param size Tamanho da tabela de hash
@return output da funçao de hash
 */
long hash(long id, long size){
	return id % size; 
}
