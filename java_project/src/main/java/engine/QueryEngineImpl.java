package engine;

import li3.Interface;

import java.util.ArrayList;
import javax.xml.stream.XMLStreamReader;
import javax.xml.stream.XMLInputFactory;
import javax.xml.stream.XMLStreamConstants;
import java.util.Iterator;
import java.io.FileInputStream;
import java.util.HashMap;
import java.util.TreeMap;
import javax.xml.stream.XMLStreamException;
import java.io.IOException;
import javax.xml.stream.FactoryConfigurationError;
import java.io.FileNotFoundException;
import java.util.stream.Stream;
import java.util.stream.Collectors;

public class QueryEngineImpl implements Interface{
	
	//Instance variavels
	private static long artUn;
	private static long artTot;
	private static HashMap<Long,Article> articles;
	private static TreeMap<Long,Contributor> contributors;	

    public void init() {
        QueryEngineImpl.artUn = 0;
        QueryEngineImpl.artTot = 0;
        QueryEngineImpl.articles = new HashMap<Long,Article>();
        QueryEngineImpl.contributors = new TreeMap<Long,Contributor>();
    }

    public void load(int nsnaps, ArrayList<String> snaps_paths) {
        FileInputStream inp;
        Iterator<String> run = snaps_paths.iterator();

        while(run.hasNext()){
            try{ 
                inp = new FileInputStream(run.next());
                processDoc(inp);
                inp.close();          
            }catch(IOException | XMLStreamException e){
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
        String title="";
        boolean newPage=true;
        long id=0;

        for(parser.next(); !parser.isStartElement() || !parser.getLocalName().equals("revision");){ //enquanto nao é a tag revision
            if(parser.isStartElement()){
                if(parser.getLocalName().equals("title")) title = parser.getElementText(); 
                else if(parser.getLocalName().equals("id")){
                        id = Long.parseLong(parser.getElementText());
                        newPage = !QueryEngineImpl.articles.containsKey(id);
                     }
            }    
        }

        if(!newPage){
            Article aux = QueryEngineImpl.articles.get(id);
            aux.setTitle(title);
        }else{
            Article newArt = new Article();
            newArt.setId(id);
            newArt.setTitle(title);
            QueryEngineImpl.articles.put(id,newArt);
        }
        parser.next();
        processRevision(parser, id);
        return newPage;
    }

    private void processRevision(XMLStreamReader parser, long id) throws XMLStreamException, IllegalStateException{
        Article auxA=null;
        Contributor auxC=null;
        String contribName="", timestamp="";
        long idR=0, idC=0;
        boolean newRev=true;

        for(;newRev && (!parser.isStartElement() || !parser.getLocalName().equals("text")); parser.next()){
            if(parser.isStartElement()){
                    if(parser.getLocalName().equals("id")){ 
                        idR = Long.parseLong(parser.getElementText());
                        auxA = QueryEngineImpl.articles.get(id);
                        newRev = !auxA.getRevisions().containsKey(idR); //verifica se a revisao ja foi adicionada
                    }
                    else if(parser.getLocalName().equals("timestamp")) timestamp  = parser.getElementText();
                    else if(parser.getLocalName().equals("username")){
                        contribName = parser.getElementText();
                        while(!parser.isStartElement() || !parser.getLocalName().equals("id")) parser.next();
                        idC = Long.parseLong(parser.getElementText());
                        if(QueryEngineImpl.contributors.containsKey(idC)) QueryEngineImpl.contributors.get(idC).addNewRev();
                        else{
                            auxC = new Contributor();    
                            auxC.setId(idC);
                            auxC.setName(contribName);
                            auxC.addNewRev();
                            QueryEngineImpl.contributors.put(idC, auxC);
                        }
                    } 
            }
        }

        //revisao encontrada e nova
        if(newRev){
            auxA.getRevisions().put(idR, timestamp);  
            if(parser.getLocalName().equals("text"))
                    auxA.setNewLenghtWords(parser.getElementText()); 
            auxA.incNRev();
        }
        
    }

    public long all_articles() {
        return QueryEngineImpl.artTot;
    }

    public long unique_articles() {
        return QueryEngineImpl.artUn;
    }

    public long all_revisions() {
        return QueryEngineImpl.articles.values()
                                       .stream()
                                       .mapToLong(Article::getNRev)
                                       .sum();
    }

    public ArrayList<Long> top_10_contributors() {
        
        return QueryEngineImpl.contributors.values()
                                           .stream()
                                           .sorted(new ComparatorContributorRevs())
                                           .limit(10)
                                           .map(a -> a.getId())
                                           .collect(Collectors.toCollection(ArrayList::new));
    }
    
    //devolve o nome do autor com um determinado identificador. Caso não exista
    //o autor a interrogação retorna o valor NULL.
    public String contributor_name(long contributor_id) {
        
        if(QueryEngineImpl.contributors.containsKey(contributor_id))
            return QueryEngineImpl.contributors.get(contributor_id).getName();
        else return null;
    }

    public ArrayList<Long> top_20_largest_articles() {
        return QueryEngineImpl.articles.values()
                                       .stream()
                                       .sorted(new ArtCompareText())
                                       .limit(20)
                                       .map(Article::getId)
                                       .collect(Collectors.toCollection(ArrayList::new));
    }
    
    //devolve o título do artigo com um determinado identificador. Caso não exista
    //o artigo a interrogação retorna o valor NULL. No caso de um determinado artigo ter várias
    //versões (revisões) deve ser considerado o título da revisão mais recente.
    public String article_title(long article_id) {
        
        if(QueryEngineImpl.articles.containsKey(article_id))
            return QueryEngineImpl.articles.get(article_id).getTitle();
        else return null;
    }

    public ArrayList<Long> top_N_articles_with_more_words(int n) {
        return QueryEngineImpl.articles.values().stream()
                                                .sorted(new ArtCompareWords())
                                                .limit(n)
                                                .map(Article::getId)
                                                .collect(Collectors.toCollection(ArrayList::new));
    }

    public ArrayList<String> titles_with_prefix(String prefix) {

        return QueryEngineImpl.articles.values()
                                       .stream()
                                       .map(a -> a.getTitle())
                                       .filter(a -> a.startsWith(prefix))
                                       .sorted()
                                       .collect(Collectors.toCollection(ArrayList::new));

    }

    //devolve o timestamp para uma certa revisão de um artigo. Caso não exista
    //a revisão daquele artigo a interrogação retorna o valor NULL.
    public String article_timestamp(long article_id, long revision_id) {
        
        if(QueryEngineImpl.articles.containsKey(article_id) && QueryEngineImpl.articles.get(article_id).getRevisions().containsKey(revision_id))
            return QueryEngineImpl.articles.get(article_id).getRevisions().get(revision_id);
        else return null;
    }

    public void clean() {
       ; 
    }
}
