package engine;

import java.util.Comparator;

public class ArtCompareWords implements Comparator<Article>{
    public int compare(Article a1, Article a2){
       if(a1.getWords() > a2.getWords() || (a1.getWords() == a2.getWords() && a1.getId() < a2.getId())) return -1; 
       else return 1;
    }
}
