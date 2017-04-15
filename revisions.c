#define EH 0
#define LH 1
#define RH 2


/*
 * Regista um novo contribuidor e revisao
 *
*/
int addRev(revDictP *dict, contribTreeP *tree, xmlNodePtr cur, xmlDocPtr doc, long *len, long *words, long nRev){
    
    xmlChar *temp, *userN;
    xmlNodePtr aux;
    int new = 1;
    long i, idT; 

    cur = cur->xmlChildrenNode;
    while(cur && new){
        if(!xmlStrcmp(cur->name, (xmlChar *)"id")){                     //id found
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode);
            if(temp){                                                   //non empty node
                sscanf(temp, "%ld", &idT);
                if((new = newEntry(*dict, nRev, idT))){
                    *dict = (struct revDict *)realloc(*dict, nRev+1);    
                    (*dict)[nRev].id = idT;
                }
                xmlFree(temp);
            }    
        }else if(!xmlStrcmp(cur->name, (xmlChar *)"timestamp")){        //add revision timestamp
            temp = xmlNodeListGetString(doc, cur->xmlChildrenNode);
            if(temp){                                                   //non empty node
                (*dict)[nRev].timestamp = (xmlChar *)strdup(temp);    
                xmlFree(temp);
            }
        }else if(!xmlStrcmp(cur->name, (xmlChar*)"contributor")){
            aux = cur->xmlChildreNode;
            while(aux){
                if(!xmlStrcmp(aux->name, (xmlChar *)"username")) userN = xmlNodeListGetString(doc, aux->xmlChildreNode);    //found user
                else if(!xmlStrcmp(aux->name, (xmlChar *)"id")){                                                            //found user id
                    temp = xmlNodeListGetString(doc, aux->xmlChildrenNode);
                    if(temp){
                        sscanf(temp, "%ld", &idT);
                        xmlFree(temp);                     
                    }
                    addAVL(tree, idT, userN, &new); 
                    xmlFree(userN);
                }    
                else if(!xmlStrcmp(aux->name, (xmlChar *)"text")){
                    temp = xmlNodeListGetString(doc, aux->xmlChildrenNode);
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

