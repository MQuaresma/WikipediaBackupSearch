package engine;

public class Contribuitor {

		//Instance variavels
		private long id;
		private String name;
		private int nRev;
        
        //constructors
        public Contribuitor(){
            this.id = 0;
            this.name = null;
            this.nRev = 0;
        }

        public Contribuitor(long id, String name, int nRev){
            this.id = id;
            this.name = name;
            this.nRev = nRev;
        }

        public Contribuitor(Contribuitor c){
            this.id = c.getId();
            this.name = c.getName();
            this.nRev = c.getNRev();
        }

        //gets & sets
        public long getId(){
            return this.id;
        }

        public String getName(){
            return this.name;
        }

        public int getNRev(){
            return this.nRev;
        }

        public void setId(long id){
            this.id = id;
        }

        public void setName(String name){
            this.name = name;
        }

        public void setNRev(int nRev){
            this.nRev = nRev;
        }

        public boolean equals(Object o){
            if(this == o) return true;
            if((o == null) || (o.getClass()!=this.getClass())) return false;
            Contribuitor c = (Contribuitor)o;
            return (this.id==c.getId() && this.name.equals(c.getName()) && this.nRev==c.getNRev());
        }

        public Contribuitor clone(){
            return new Contribuitor(this);
        }

        public String toString(){
            StringBuilder r = new StringBuilder();
            r.append("Id: ").append(this.id);
            r.append(" Name: ").append(this.name);
            r.append(" nRev: ").append(this.nRev);
            return r.toString();
        }

        public int hashCode(){
            int r = 7;

            r = r*23 + (int)(this.id ^ (this.id >>> 32));
            r = r*23 + this.name.hashCode();
            r = r*23 + this.nRev;
            return r;
        }

        public int compareTo(Contribuitor c){
            long aux = c.getId();
            int res;
            if(this.id == aux) res = 0;
            else if(this.id < aux) res = -1;
                 else res = 1;
            return res;
        }
}
