package engine;

import li3.Interface;

import java.util.ArrayList;
import javax.xml.stream.XMLStreamReader;
import java.io.FileInputStream;

public class QueryEngineImpl implements Interface {
	
	//Instance variavels
	private static long artUn;
	private static long artTot;
	private static HashMap<Long,Article> artigos;
	private static TreeMap<Long,Contribuitor> contribuitors;	

    public void init() {
        QueryEngineImpl.artUn = 0;
        QueryEngineImpl.artTot = 0;
        QueryEngineImpl.artigos = new HashMap<Long,Article>();
        QueryEngineImpl.contribuitors = new TreeMap<Long,Contribuitor>();
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
    private void processDoc(FileInputStream docStream) throws IllegalStateException, XMLStreamException, FactoryConfigurationError{
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

    private boolean processPage(XMLStreamReader parser) throws XMLStreamException, IllegalStateException{
        int event;
        String title;
        long id;

        for(event = parser; !event.isStartElement() || !parser.getLocalName().equals("revision"); event=parser.next()){ //enquanto nao é a tag revision
            if(event.isStartElement()){
                if(parser.getLocalName().equals("title")) title = parser.getElementText(); 
                else if(parser.getLocalName.equals("id")) id = Long.parseLong(parser.getElementText());
            }
        }

        if(QueryEngineImpl.artigos.containsKey(id)){
            Article aux = QueryEngineImpl.artigos.get(id);
            aux.setTitle(title);
        }else{
            Article newArt = new Article();
            newArt.setId(id);
            newArt.setTitle(title);
            QueryEngineImpl.artigos.put(id,newArt);
        }
        processRevision(parser, id);
    }

    private boolean processRevision(XMLStreamReader parser, long id){

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
