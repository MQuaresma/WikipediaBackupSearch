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
        long *top =(long *)calloc(sizeof(long),n);
        long *val =(long *)calloc(sizeof(long),n);

        for(i=0;i<n;i++) {
                top[i]=0;
                val[i]=0;
        }

        for(i=0; i<(qs->articCollect)->size; i++){
                for(aux=(qs->articCollect)->table[i]; aux; aux = aux->next){
                        j=0;
                        if(b==0) x = aux->len;
			else if(b==1) x = aux -> words;
			     else return NULL;
                        
			while(j<n && x<val[j]) j++;
                        if(j!=n){
                                while(x==val[j]){
                                        if(aux->id>top[j]) j++;
                                        else break;
                                }
                                for(w=n-2;w>=j;w--) {
                                        top[w+1]=top[w];
                                        val[w+1]=val[w];
                                }
                                val[j]=x;
                                top[j]=aux->id;
                        }
                }
        }
        free(val);
        return top;
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
					ret=auxRev[i].timeStamp;
					f = 1;
				}
			}
		}
	}
	return ret;
}
