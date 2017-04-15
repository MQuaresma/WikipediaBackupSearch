#include "interface.h"


main(){

    char *paths[] = {"snapshot_dec16", "snapshot_jan17", "snapshot_jan17"};
    TAD_istruct qs = init();
    qs = load(qs, 3, paths);

    //chamar interrogaçoes
    
    qs = clean(qs);

}
