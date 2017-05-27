package engine;

import java.util.Map;
import java.util.HashMap;
import java.util.Comparator;
import java.util.Arrays;
import java.util.stream.Stream;

public class Article{

	//Instance variavels
	private long id;
	private String title;
	private Map<Long,String> revisions;
	private long nRev;
	private long len;
	private long words;

	//constructors
	public Article(){
		this.id = 0;
		this.title = null;
	    this.revisions = new HashMap<Long,String>();
        this.nRev = 0;
        this.len = 0;
        this.words = 0;
	}

    public Article(long id, String title, HashMap<Long,String> rev, long nRev, long len, long words){
        this.id = id;
        this.title = title;
        
        this.revisions = new HashMap<Long,String>();
        for(Map.Entry<Long,String> e: rev.entrySet())
            this.revisions.put(e.getKey(),e.getValue());

        this.nRev = nRev;
        this.len = len;
        this.words = words;
    }

    public Article(Article a){
        this.id = a.getId();
        this.title = a.getTitle();
        this.revisions = a.getRevisions();
        this.nRev = a.getNRev();
        this.len = a.getLen();
        this.words = a.getWords();
    }
    
    //gets & sets
    public long getId(){
        return this.id;
    }

    public String getTitle(){
        return this.title;
    }

    public Map<Long,String> getRevisions(){
        HashMap<Long,String> r = new HashMap<Long,String>();
        for(Map.Entry<Long,String> e: this.revisions.entrySet())
            r.put(e.getKey(),e.getValue());
        return r;
    }

    public Map<Long,String> getRevisionsP(){
        return this.revisions;
    }

    public long getNRev(){
        return this.nRev;
    }

    public long getLen(){
        return this.len;
    }

    public long getWords(){
        return this.words;
    }

    public void setId(long id){
        this.id = id;
    }

    public void setTitle(String title){
        this.title = title;
    }

    public void setRevisions(HashMap<Long,String> rev){
        this.revisions = new HashMap<Long,String>();
        for(Map.Entry<Long,String> e: rev.entrySet())
            this.revisions.put(e.getKey(),e.getValue());
    }

    public void setNRev(long nRev){
        this.nRev = nRev;
    }

    public void setLen(long len){
        this.len = len;
    }

    public void setWords(long words){
        this.words = words;
    }

    public void setNewLenghtWords(String text){
        long size = text.length(), tWords=0L;
        
        for(int i = 0; i < size;){
            while(i < size && (text.charAt(i) == ' ' || text.charAt(i) == '\t'  || text.charAt(i) == '\n')) i ++;
            if(i < size) tWords++; 
            while(i < size && text.charAt(i) != ' ' && text.charAt(i) != '\t'  && text.charAt(i) != '\n') i ++;
        }

        if(size > this.len) this.len = size;
        if(tWords > this.words) this.words = tWords;
    }

    public void incNRev(){
        this.nRev ++;
    }
    
    public boolean equals(Object o){
        if(this == o) return true;
        if((o==null) || (o.getClass()!=this.getClass())) return false;
        Article a = (Article)o;
        return (this.id==a.getId() && this.title.equals(a.getTitle()) && this.revisions.equals(a.getRevisions()) 
                && this.nRev==a.getNRev() && this.len==a.getLen() && this.words==a.getWords());
    }
    
    public Article clone(){
        return new Article(this);
    }

    public String toString(){
        StringBuilder r = new StringBuilder();
        r.append("Id: ").append(this.id);
        r.append(" Title: ").append(this.title);
        r.append(" Revisions: ").append(this.revisions.toString());
        r.append(" nRev: ").append(this.nRev);
        r.append(" Len: ").append(this.len);
        r.append(" Words: ").append(this.words);
        return r.toString();
    }

    public int hashCode(){
        int r = 7;

        r = r*23 + (int)(this.id ^ (this.id >>> 32));
        r = r*23 + this.title.hashCode();
        r = r*23 + this.revisions.hashCode();
        r = r*23 + (int)(this.nRev ^ (this.nRev >>> 32));
        r = r*23 + (int)(this.len ^ (this.len >>> 32));
        r = r*23 + (int)(this.words ^ (this.words >>> 32));
        return r;
    }

    public int compareTo(Article a){
        long aux = a.getId();
        int res;
        if(this.id == aux) res = 0;
        else if(this.id<aux) res = -1;
             else res = 1;
        return res;
    }
}
