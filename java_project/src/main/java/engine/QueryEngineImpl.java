package engine;

import li3.Interface;

import java.util.ArrayList;
import javax.xml.stream.XMLStreamReader;
import java.io.FileInputStream;

public class QueryEngineImpl implements Interface {
	
	//Instance variavels
	private static long artUn;
	private static long artTot;
	private static HashMap<Long,Article> artigos
	private static TreeMap<Long,Contribuitor> contribuitors;	

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
    private void processDoc(FileInputStream docStream){
        XMLInputFactory factory = XMLInputFactory.newInstance();  
        XMLStreamReader parser = factory.createXMLStreamReader(docStream);

        for(int event = parser.next(); event != XMLStreamConstants.END_DOCUMENT; event = parser.next())
            if(parser.isStartElement() && parser.getLocalName().equals("page")){
                parser.next(); 
                if(processPage(parser)) QueryEngineImpl.artUn ++;
                QueryEngineImpl.artTot ++;
            }

        parser.close();
    }

    private boolean processPage(XMLStreamReader parser){
        int event;
        
        for(event = parser; !event.isStartElement() || !parser.getLocalName().equals("revision"); event=parser.next()){ //enquanto nao é a tag revision
            String title;

            if(event.isStartElement()){
                if(parser.getLocalName().equals("title")){ //será que devo garantir que é um event CHARACTERS?
                    title = 
                }else if(parser.getLocalName.equals("id")){

                }
            }
        }
        processRevision(parser);
    }

    private boolean processRevision(XMLStreamReader parser){

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
