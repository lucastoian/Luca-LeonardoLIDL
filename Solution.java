import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;

public class Solution{
    public static void main(String[] args) {
        
        String str = "??J???";

        //CJ paga X
        //JC paga Y
        int X = 2;
        int Y = 5;
       // char[] c = str.toCharArray();
        ArrayList<String> c = new ArrayList<String>(Arrays.asList(str.split("")));

        System.out.println(c);
        
        for(int i = 0; i<c.size(); i++){

            if(c.get(i).equals("?")){

                

                if(i==0){
                    c.set(i, c.get(i+1));  //0 == i+1
                    continue;
                }

                if(i+1 == c.size()){

                    c.set(i, c.get(i-1));    //n = i-1
                    continue;
                
                }

                if(c.get(i-1).equals(c.get(i+1))){       // i = i-1 se  C ? C  
                    c.set(i, c.get(i-1));
                    continue;
                }

                if(X<=Y){          // J?C oppure C?J
                    c.set(i, "J");
                    continue;
                }                                       

                c.set(i, "C");
            }

            
        }

        if(c.contains("?")){
            if(!c.contains("C")){
                for(int i = 0; i<c.size(); i++){
                    if(c.get(i).equals("?")) c.set(i, "J");
                }
            }else{
                for(int i = 0; i<c.size(); i++){
                    if(c.get(i).equals("?")) c.set(i, "C");
                }
            }
        }

        int cost = 0;

        for(int i = 0; i<c.size()-1; i++){
            if(c.get(i).equals("C") && c.get(i+1).equals("J")) cost += X;
            if(c.get(i).equals("J") && c.get(i+1).equals("C")) cost += Y;
        }
   
        
        System.out.println(c + " costo = " + cost);

    }
}