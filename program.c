#include "interface.h"
#include <stdio.h>


int main(int argc, char *argv[]){

    char *paths[] = {"snapshot_dec16", "snapshot_jan17", "snapshot_fev17"};
    long *top;

    TAD_istruct qs = init();
    qs = load(qs, 3, paths);

    /*printf("All articles: %ld\nUnique articles: %ld\nAll revisions: %ld\n", all_articles(qs), unique_articles(qs), all_revisions(qs));
    top = top_10_contributors(qs);
    printf("Top 10 contributors: ");
    for(int i = 0; i < 10; i ++) printf("%ld, ",  top[i]);
    printf("\n");
    printf("%s\n", contributor_name(28903366, qs));
    printf("%s\n", contributor_name(194203, qs));
    printf("%s\n", contributor_name(1000, qs));
    top = top_20_largest_articles(qs);
    printf("Top 20 largest articles: ");
    for(int i = 0; i < 20; i ++) printf("%ld, ",  top[i]);
    printf("\n");
    printf("%s\n", article_title(15910, qs));
    top = top_N_articles_with_more_words(30, qs);
    printf("Top 30 with more words: ");
    for(int i = 0; i < 30; i ++) printf("%ld, ", top[i]);
    printf("\n");
    printf("%s\n", article_title(25507, qs));
    printf("%s\n", article_title(1111, qs));
    printf("%s\n", article_timestamp(12,763082287, qs));
    printf("%s\n", article_timestamp(12,755779730, qs));
    printf("%s\n", article_timestamp(12,4479730, qs));

    qs = clean(qs);*/
    
}
