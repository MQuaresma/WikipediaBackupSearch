
//5 devolve o nome do autor com um determinado identificador. Caso não exista
//o autor a interrogação retorna o valor NULL.
public String contributor_name(long contributor_id) {

  if(QueryEngineImpl.contributors.containsKey(contributor_id))
		return QueryEngineImpl.contributors.get(contributor_id).getName();
    
  else return null;
}







//​devolve o título do artigo com um determinado identificador. Caso não exista
//o artigo a interrogação retorna o valor NULL. No caso de um determinado artigo ter várias
//versões (revisões) deve ser considerado o título da revisão mais recente.
public String article_title(long article_id) {
      
    if(QueryEngineImpl.articles.containsKey(article_id))
      return QueryEngineImpl.articles.get(article_id).getTitle();
                                        
    else return null;
      
}







//devolve o timestamp para uma certa revisão de um artigo. Caso não exista
//a revisão daquele artigo a interrogação retorna o valor NULL.
    public String article_timestamp(long article_id, long revision_id) {
         if(QueryEngineImpl.articles.containsKey(article_id) && QueryEngineImpl.articles.get(article_id).getRevisions().containsKey(revision_id))
            return QueryEngineImpl.articles.get(revision_id).getTimeStamp();

         else return null;
    }