#include <stdio.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "interface.h"
#define HASHSIZE 100013
#define EH 0        //AVL equilibrada
#define LH 1        //AVL desequilibrada para a esquerda
#define RH 2        //AVL desequilibrada para a direita

/*
 * Dicionario que guarda uma revisao de determinado artigo
  @param id ID da revisao
  @param timeStamp altura em que a revisao foi efetuada
 */
typedef struct revDict{
	long id;
	xmlChar *timeStamp;
}*revDictP;

/*
 * Info essencial associada a cada artigo
  @param id ID do artigo
  @param title Titulo do artigo
  @param revs Lista com todas as revisoes feitas ao artigo
  @param nRev Nro de revisoes totatis feitas ao artigo
  @param len Tamanho do artigo
  @param words Nro de palavras do artigo
  @param next Apontador para o artigo seguinte
 */
struct articleInfo {
	long id;
	xmlChar *title;
	struct revDict *revs;
	long nRev;
	long len;
	long words;
    struct articleInfo *next;
};

/*
 * Hash table fechada com artigos
 @param racio Percentagem de ocupaçao da hash table
 @param size Nro de posiçoes da tabela
 @param table Lista de apontadores para a estrutura do ficheiro
 */
typedef struct articTable{
	int racio;
	long size;
	struct articleInfo **table;
}*articTableP;

/*
 * AVL para guardar os contribuidores ordenados por id
 @param id ID do contribuidor
 @param nome Nome do contribuidor
 @param nRev Nro de revisoes efetuadas pelo contribuidor
 @param left Apontador para o contribuidor da esquerda(menor id)
 @param bal Fator de balanço
 @param right Apontador para o contribuidor da direita(maior id)
 */
typedef struct contribTree{
	long id;
	xmlChar *nome;
	int nRev;
    int bal;
	struct contribTree *left;
	struct contribTree *right;
}*contribTreeP;

/*
 * Estrutura principal com toda a informaçao necessario ao processamento das queries
 @param artUn Nro de artigos unicos
 @param artTot Nro de total de artigos 
 @param articCollect Artigos contidos nos snapshots processados
 @param contribuitors Contribuidores envolvidos nas snapshots
 @param top10Contr Array com os 10 contribuidores com maior nro de revisoes efetuadas
 */
struct TCD_istruct {
	long artUn, artTot;
	articTableP articCollect;
	struct contribTree *contribuitors;
	long top10Contr[10];
};

TAD_istruct processPages(TAD_istruct qs, xmlNodePtr t, xmlDocPtr doc);
int hashAdd(TAD_istruct st, xmlNodePtr nodo, xmlDocPtr doc);
void resize(articTableP * articCollect);
int addRev(revDictP *dict, contribTreeP *tree, xmlNodePtr cur, xmlDocPtr doc, long *len, long *words, long nRev);
void addAVL(contribTreeP *tree, long id, xmlChar *nome, int *new);
void addAVLRight(contribTreeP *tree, long id, xmlChar *nome, int *new);
void addAVLLeft(contribTreeP *tree, long id, xmlChar *nome, int *new);
void balanceRight(contribTreeP *tree);
void balanceLeft(contribTreeP *tree);
long hash(long id, long size);