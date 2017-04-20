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


// soma de todas as revisões
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



//Devolve 

char* contribuitor_name(long contributor_id, TAD_istruct qs){
  contribTreeP *nodo;

  nodo = qs->contributors;
  while (nodo != NULL && nodo->id != contributor_id){
     
  if (nodo->id > contributor_id)
     nodo = nodo->left;
  else
     nodo = nodo->right;
  }
  if (nodo!= NULL)
  return (char*)nodo->nome;
  else return NULL;
} 




//Devolve o nome de um artigo, através do id dado.

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





// Devolve uma lista de endereços cujo o título tem o prefixo dado.

char** titles_with_prefix(char* prefix, TAD_istruct qs){
  articleInfoP i;
  long k, j=0;
  char** guarda;
  int n;
  
  guarda =(char**) calloc (n,sizeof (char*));
  
  for (k=0; k<qs->articCollect->size;k++){
    i = qs->articCollect->table[k];
    for(i=0; i!=NULL;i=i->next){
        if (xmlStrstr (i->title,(xmlChar*)prefix)==(i->title)){
          if (j>=n) {
            n = 2*n;
            guarda = (char**) realloc (guarda,n);
          }
          *guarda[j] = *(i->title); //corrigir
          j++;
        }

    }
    
  }
  if (j>=n) guarda = (char**) realloc (guarda,n+1);
  guarda[j] = NULL;
  return guarda;
}

