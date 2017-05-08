#include "structManager.h"
#include <string.h>

long* devolveArrayOrd(int, int, TAD_istruct);
long* runTree(contribTreeP tree);
contribTreeP *myRealloc(contribTreeP *oStack, int size, int nSize);
void quickSort(char **arr, int len);

/**
Devolve o nro total de artigos encontrados
@param qs Estrutura com a informaçao processada
@return Nro total de artigos
 */
long all_articles(TAD_istruct qs){

    return qs->artTot;

}

/**
Devolve o nro total de artigos unicos encontrados
@param qs Estrutura com a informaçao processada
@return Nro total de artigos unicos
 */
long unique_articles(TAD_istruct qs){

    return qs->artUn;

}

/**
Calcula os 10 contribuidores com maior nro de revisoes efetuadas
@param qs Estrutura com a informaçao processada
@return Apontador para um array ordenado, por ordem decrescente, para os id's dos contribuidores
 */
long *top_10_contributors(TAD_istruct qs){

    return runTree(qs->contribuitors);

}

/**
Percorre a árvore binária dos contribuidores e calcula os dez mais ativos
@param tree Árvore binária dos contribuidores
@return Apontador para um array ordenado, por ordem decrescente, para os id's dos contribuidores
 */
long* runTree(contribTreeP tree){

    long *idTop, idTemp, sp = 0, size = 100;
    int *valTop, revTemp, i, in=0;
    idTop = (long*)calloc(10, sizeof(long));
    valTop = (int*)calloc(10, sizeof(int));
    contribTreeP *stack = (contribTreeP *)calloc(1000, sizeof(void*));
    contribTreeP *aux = NULL;
             
    if(idTop && valTop && stack){
        while(sp >= 0L){
            idTemp = tree->id; 
            revTemp = tree->nRev;
            for(i = 0; i < in && (revTemp < valTop[i] || (idTop[i] && revTemp == valTop[i] && idTemp > idTop[i])); i ++);
            if(i < 10){
                for(int j = in-1; j > i; j --){
                    idTop[j] = idTop[j-1];
                    valTop[j] = valTop[j-1];
                }
                idTop[i] = idTemp;
                valTop[i] = revTemp;
                if(in < 10) in ++;
            }             
            if(tree->left || tree->right){
                if(sp >= size){
                    aux = realloc(stack, (int)(size*1.5));
                    if(aux && aux != stack){ 
                        stack = aux;
                        size = (int)(size*1.5);
                    }    
                }
                if(tree->left){
                    if(tree->right) stack[sp++] = tree->right;
                    tree = tree->left;
                }
                else if(tree->right) tree = tree->right;
            }else if(--sp >= 0L) tree = stack[sp];
        }
        free(valTop);
        free(stack);
    }

    return idTop;

}


/**
 *Função que devolve um array com os ids dos 20 artigos maiores
@param qs Estrutura com os dados dos artigos
@return Array, ordenado por ordem decrescente do tamanho, com os id's dos artigos
 */
long* top_20_largest_articles(TAD_istruct qs){
	return devolveArrayOrd(20,0,qs);
}

/**
Função que devolve um array com os ids dos N artigos com mais palavras
@param n Tamanho do array a devolver, ou seja N artigos com mais palavras
@param qs Estrutura com os dados dos artigos
@return array com os id's dos artigos ordenados por ordem decrescente do numero de palavras
 */
long* top_N_articles_with_more_words(int n, TAD_istruct qs){
	return devolveArrayOrd(n,1,qs);
}

/**
 *Devolve o timestamp de uma determinada revisao de um artigo
@param article_id id do artigo no qual pertence a revisão
@param revision_id Id da revisão
@param qs Estrutura com os dados dos artigos
@return String com o timestamp ou null caso nao exista a revisão/artigo pedido.
 */
char* article_timestamp(long article_id, long revision_id, TAD_istruct qs){
	long hashV = hash(article_id,(qs->articCollect)->size);
	struct articleInfo *aux = NULL; 
	struct revDict *auxRev = NULL;
	char *ret = NULL;
	int f = 0,i;
	
	for(aux=(qs->articCollect)->table[hashV]; aux && !f; aux=aux->next){
		if(aux->id==article_id){
			auxRev = aux->revs;
			for(i=0; i<(aux->nRev) && !f; i++){
				if((auxRev[i].id)==revision_id){
					ret=(char*)(auxRev[i].timeStamp);
					f = 1;
				}
			}
		}
	}
	return ret;
}


/**
 *Funçao que devolve um array de ids de artigo
@param n Tamanho do array a devolver
@param b b=0 ordenado por tamanho, b=1 ordenado por palavras
@param qs Estrutura com os dados dos artigos
@return Array com os id's dos artigos ordenados segundo o criterio dado
 */
long* devolveArrayOrd(int n, int b, TAD_istruct qs){
        int i,j,w;
        long x;
        struct articleInfo *aux = NULL;
        long *top =(long *)calloc(n, sizeof(long));
        long *val =(long *)calloc(n, sizeof(long));

        for(i=0; i<(qs->articCollect)->size; i++){
            for(aux=(qs->articCollect)->table[i]; aux; aux = aux->next){
                j=0;
                if(b==0) x = aux->len;
                else if(b==1) x = aux -> words;
                else return NULL;

                while(j<n && x<val[j]) j++;
                if(j!=n){
                    while(x==val[j]){
                        if(aux->id>top[j] && top[j] != 0 && j < n) j++;
                        else break;
                    }
                    for(w=n-2;w>=j;w--) {
                        top[w+1]=top[w];
                        val[w+1]=val[w];
                    }
                    if(j < n){
                        val[j]=x;
                        top[j]=aux->id;
                    }
                }
            }
        }
        if(val)free(val);
        return top;
}



/**
Nro total de revisoes presentes nos snapshots
@param qs Estrutura com os dados dos artigos
@return Nro de revisoes encontradas nos snapshots
 */
long all_revisions(TAD_istruct qs){
    articleInfoP i;
    long k,soma=0;
    
    for (k=0; k<qs->articCollect->size;k++){
       i = qs->articCollect->table[k];
       while (i!=NULL){
         soma = soma + i->nRev;
         i=i->next;
       }
    }
    return soma;
}



/**
Devolve o nome do contribuidor identificado pelo id dado
@param id Id a procurar 
@param qs Estrutura com os dados dos artigos
@return Apontador para o nome do contribuidor caso tenha sido encontrado, NULL caso contrario
 */
char* contributor_name(long contributor_id, TAD_istruct qs){

  contribTreeP nodo;

  nodo = qs->contribuitors;
  while(nodo && nodo->id != contributor_id){
      if(nodo->id > contributor_id) nodo = nodo->left;
      else nodo = nodo->right;
  }

  if(nodo) return (char*)nodo->nome;
  else return NULL;

} 




/**
Devolve o nome de um artigo, através do id dado
@param id Id a procurar 
@param qs Estrutura com os dados dos artigos
@return Apontador para o nome do artigo caso tenha sido encontrado, NULL caso contrario
 */
char* article_title(long article_id, TAD_istruct qs){
  articleInfoP i;
  long k, flag=0;
  
  for (k=0; k<qs->articCollect->size && !flag; k++){
   i=qs->articCollect->table[k];
   while (i != NULL && i->id != article_id){
     i=i->next;
   }
   if (i!=NULL) flag=1;
  }
  if (i!= NULL)
  return (char*)i->title;
  else return NULL;
}


/** 
Devolve uma lista de endereços, ordenados por ordem alfabetica, cujo o título tem o prefixo dado.
@param prefix Prefixo a procurar
@param qs Estrutura com os dados dos artigos
@return Array de apontadores (ordenado) para os titulos com o prefixo dado
 */
char** titles_with_prefix(char* prefix, TAD_istruct qs){

    int n=10, len = strlen(prefix), full = 0;
    long k, j=0;
    articleInfoP i;
    char** guarda = (char**)calloc(n,sizeof (char*));
    char **aux = NULL;

    if(guarda){
        for (k=0; k<qs->articCollect->size && !full;k++)
            for(i = qs->articCollect->table[k]; !full && i ; i=i->next)
                if (xmlStrstr(i->title,(xmlChar*)prefix) == i->title){
                    if (j>=n) {
                        n *= 2;
                        aux = (char**)realloc(guarda,n);
                        if(aux && aux != guarda) guarda = aux;
                        else full = 1;
                    }
                    guarda[j++] = (char*)i->title;
                }

        if (j>=n) guarda = (char**)realloc(guarda,n+1);
        guarda[j-full] = NULL;
    }

    quickSort(guarda, len);  

    return guarda;
}

/**
Ordena um array de strings por ordem alfabetica
@param arr Array de apontadores para arrays de caracteres
@param len Tamanho do prefixo, i.e. ponto a partir do qual e efetuada a comparaçao
 */
void quickSort(char **arr, int len){

    char *piv, *aux;
    int i, j;

    if(*arr && arr[1]){
        piv = *arr;
        j = 1;
        for(i = 1; arr[i]; i ++){
            if(strcmp(piv+len, arr[i]+len) > 0){
                aux = arr[i];
                arr[i] = arr[j];
                arr[j++] = aux;
            }
        }
        arr[0] = arr[j-1];
        arr[j-1] = NULL;
        quickSort(arr, len);
        quickSort(arr+j, len);
        arr[j-1] = piv;
    }

}
