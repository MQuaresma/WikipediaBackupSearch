TAD_istruct load(TAD_istruct qs, int nsnaps, char *snaps_paths[]){

    xmlDocPtr doc;
    xmlNodePtr cur;

    while(--nsnaps){
        doc = xmlParseFile(*snaps_paths);

        if(!doc) fprintf(stderr, "Couldn't parse file\n");

        else{
            cur = xmlDocGetRootElement(doc);
            if(!cur) fprintf(stderr, "Empty xml document\n");
            else{
                cur = cur->xmlChildrenNode;
                qs = processPages(qs, cur, doc);                  
            }
            xmlFreeDoc(doc);
        } 
        ++snaps_paths;
    }
}

TAD_istruct processPages(TAD_istruct qs, xmlNodePtr t, xmlDocPtr doc){

	while(cur){												//percorre o doc xml na totalidade
		if(!xmlStrcmp(cur->name, (const xmlChar*)"page")){	//encontrou uma pagina
			qs->artTot ++;
			if(hashAdd(qs->articCollect, cur, doc)) qs->artUn ++; 
        }
        cur = cur->next;
	}
	return qs;
}

void addAVL(contribTree **cT, long id, xmlChar *nome){

    

    while(*cT && id != *cT->id)
        cT = ((*cT->id > id) ? &(*cT->left) : &(*cT->left)); 
    if(!(*cT)){



    }

}

long all_articles(TAD_istruct qs){

    return qs->artTot;

}

long unique_articles(TAD_istruct qs){

    return qs->artUn;

}

long *top_10_contributors(TAD_istruct qs){

    return qs->top10Contr;

}