import java.util.*;

public class Main {
    static ArrayList<HashMap<Integer, Integer>> graph=new ArrayList<>();
    static ArrayList<Integer> allSpeed=new ArrayList<>();
    static int n,c;

    static class ret{
        int i;
        ArrayList<Integer> vi;
    }

    static ret dfs(int node,int parent){
        int signCost=graph.get(node).size()>1?c*graph.get(node).size():0;
        ArrayList<Integer> costs=new ArrayList<>(Collections.nCopies(allSpeed.size(),0));
        int maxSpeed=0,parS=node!=1?graph.get(node).get(parent):0;
//        try {
//            maxSpeed = 0;
//            parS =
//        }catch(Exception e){
//            System.out.println(e.getMessage());
//        }
        for(Map.Entry<Integer, Integer> e:graph.get(node).entrySet()){
            int ch=e.getKey(), s=e.getValue();
            maxSpeed=Math.max(maxSpeed,s);
            if(ch==parent)continue;
            var r=dfs(ch,node);
            signCost+=r.i;
            for(int i=0;i<allSpeed.size();++i)
                costs.set(i,costs.get(i)+r.vi.get(i));
        }
        int bestCost=signCost;
        for(int i=0;i<allSpeed.size();++i){
            if(allSpeed.get(i)<maxSpeed)
                costs.set(i,signCost+(parent!=0?allSpeed.get(i)-parS:0));
            else {
                costs.set(i, costs.get(i) + (parent != 0 ? allSpeed.get(i) - parS : 0));
                bestCost = Math.min(bestCost, costs.get(i));
                costs.set(i,Math.min(costs.get(i),signCost+(parent!=0?allSpeed.get(i)-parS:0)));
            }
        }
        ret rrr=new ret();
        rrr.i=bestCost;
        rrr.vi=costs;
        return rrr;
    }

    static public void main(String ars[]){
        Scanner sc= new Scanner(System.in);
        n=sc.nextInt();
        c=sc.nextInt();
        Set<Integer> uniqueSpeeds=new HashSet<>();
        for(int i=0;i<n+1;i++){
            graph.add(new HashMap<>());
        }
        for(int i=0;i<n-1;++i){
            int u,v,s;
            u=sc.nextInt();
            v=sc.nextInt();
            s=sc.nextInt();
//            if(graph.get(u)==null)graph.set(u,new HashMap<>());
            graph.get(u).put(v,s);
//            if(graph.get(u)==null)graph.set(u,new HashMap<>());
            graph.get(v).put(u,s);
            uniqueSpeeds.add(s);
        }
        allSpeed.addAll(uniqueSpeeds);
        //graph.get(1).put(0,0);
        ret r=dfs(1,0);
        System.out.println(r.i);
    }
}
