package engine;

import java.util.Comparator;

public class ArtCompareText implements Comparator<Article>{
    public int compare(Article a1, Article a2){
        if(a1.getLen() > a2.getLen() || (a1.getLen() == a2.getLen() && a1.getId() < a2.getId())) return -1;
        else return 1;
    }
}
