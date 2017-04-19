#include "structManager.h"

long* devolveArrayOrd(int, int, TAD_istruct);
long* runTree(contribTreeP tree);
contribTreeP *myRealloc(contribTreeP *oStack, int size, int nSize);

long all_articles(TAD_istruct qs){

    return qs->artTot;

}

long unique_articles(TAD_istruct qs){

    return qs->artUn;

}

long *top_10_contributors(TAD_istruct qs){

    return runTree(qs->contribuitors);

}

long* runTree(contribTreeP tree){

    long *idTop, idTemp, sp = 0, size = 100;
    int *valTop, revTemp, i, in=0;
    idTop = (long*)calloc(10, sizeof(long));
    valTop = (int*)calloc(10, sizeof(int));
    contribTreeP *stack = (contribTreeP *)calloc(1000, sizeof(void*));
             
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
                    size *= 2;
                    stack = myRealloc(stack, size/2, size);
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

contribTreeP *myRealloc(contribTreeP *oStack, int size, int nSize){

    contribTreeP *nStack = (contribTreeP *)calloc(nSize, sizeof(void*));

    for(int i = 0; i < size; i ++) nStack[i] = oStack[i];
    
    free(oStack);
    
    return nStack;

}

long* top_20_largest_articles(TAD_istruct qs){
	return devolveArrayOrd(20,0,qs);
}

long* top_N_articles_with_more_words(int n, TAD_istruct qs){
	return devolveArrayOrd(n,1,qs);
}

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


/*
 *Funçao que devolve um array de ids de artigo: 
 *    caso b=0: ordenado do maior para o menor artigo em termos de tamanho
 *    caso b=1: ordenado do maior para o menor artigo em termos de palavras
 *@param n tamanho do array a devolver
 *@param b b=0 ordenado por tamanho, b=1 ordenado por palavras
 *@param qs estrutura com os dados dos artigos,etc
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