package engine;

import li3.Interface;

import java.util.ArrayList;
import javax.xml.stream.XMLStreamReader;
import java.io.FileInputStream;

public class QueryEngineImpl implements Interface {
	
	//Instance variavels
	private static long artUn;
	private static long artTot;
	private static HashMap<long,Article> artigos
	//private TreeMap<long,Contribuitor> contribuitors;	

    public void init() {

    }

    public void load(int nsnaps, ArrayList<String> snaps_paths) {
        FileInputStream inp;
        Iterator<String> run = snaps_paths.iterator();

        while(run.hasNext()){
            try{ 
                inp = new FileInputStream(run.next());
                processDoc(inp);
                inp.close();          
            }catch(FileNotFoundException e){
                System.out.println(e.getMessage());
            }    
        }
    }


    /**
     * Percorre documento XML e processos elementos com a tag page
     * @param docStream stream de input do dataset
     */
    public void processDoc(FileInputStream docStream){
        XMLInputFactory factory = XMLInputFactory.newInstance();  
        XMLStreamReader parser = factory.createXMLStreamReader(docStream);

        for(int event = parser.next(); event != XMLStreamConstants.END_DOCUMENT; event = parser.next())
            if(parser.isStartElement() && parser.getLocalName().equals("page")){
                parser.next(); 
                processPage(parser);
            }

        parser.close();
    }

    public void processPage(XMLStreamReader parser){
        


    }


    public long all_articles() {

        return 0;
    }

    public long unique_articles() {

        return 0;
    }

    public long all_revisions() {

        return 0;
    }

    public ArrayList<Long> top_10_contributors() {

        return new ArrayList<Long>();
    }

    public String contributor_name(long contributor_id) {

        return " ";
    }

    public ArrayList<Long> top_20_largest_articles() {

        return new ArrayList<Long>();
    }

    public String article_title(long article_id) {

        return " ";
    }

    public ArrayList<Long> top_N_articles_with_more_words(int n) {

        return new ArrayList<Long>();
    }

    public ArrayList<String> titles_with_prefix(String prefix) {

        return new ArrayList<String>();
    }

    public String article_timestamp(long article_id, long revision_id) {

        return " ";
    }

    public void clean() {

    }
}
