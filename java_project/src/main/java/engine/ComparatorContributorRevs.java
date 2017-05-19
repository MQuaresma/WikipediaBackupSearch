/*
    ordena por número de revisoes(maiores primeiro) e caso o número de revisoes seja igual em primeiro os de menor número
*/
package engine;

import java.util.Comparator;

public class ComparatorContributorRevs implements Comparator<Contributor>{
    public int compare(Contributor c1, Contributor c2){
        int revs1 = c1.getNRev();
        int revs2 = c2.getNRev();
        long id1 = c1.getId();
        long id2 = c2.getId();
        int res;

        if(revs1==revs2){
            if(id1<id2) res=-1;
            else res=1;
        }else if(revs1<revs2) res=1;
              else res=-1;
        return res;
    }
}
