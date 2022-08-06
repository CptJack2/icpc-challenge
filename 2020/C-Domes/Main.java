import java.util.*;

public class Main {
    static class Point{
        double x,y;
        Point(double xx,double yy){x=xx;y=yy;}
        Point add(Point b){return new Point(x+b.x,y+b.y);}
        Point minus(Point b){return new Point(x-b.x,y-b.y);}
        boolean equals(Point b){return x==b.x && y==b.y;}
    }

    static int n;
    static double dx,dy;
    static ArrayList<Point> domeLocs;
    static ArrayList<Integer> permutation;

    static double CrossProduction(Point a,Point b){return a.x*b.y-a.y*b.x;}

    public static void main(String[] args){
        Scanner sc=new Scanner(System.in);
        dx=sc.nextDouble();
        dy=sc.nextDouble();
        n=sc.nextInt();
        domeLocs=new ArrayList<>();
        permutation=new ArrayList<>(n);
        for(int i=0;i<n;i++){
            double d1=sc.nextDouble(),d2=sc.nextDouble();
            domeLocs.add(new Point(d1,d2));
        }
        for(int i=0;i<n;i++){int t; t=sc.nextInt(); permutation.add(t);}
        ArrayList<Point> polygon=new ArrayList<>(), polygon2;
        polygon.add(new Point(0,0));
        polygon.add(new Point(0,dy));
        polygon.add(new Point(dx,dy));
        polygon.add(new Point(dx,0));
        for(int i=0;i<=n-2;++i)
        for(int j=i+1;j<=n-1;++j){
            var p1=domeLocs.get(permutation.get(i)-1);
            var p2=domeLocs.get(permutation.get(j)-1);
            polygon2=new ArrayList<>();
            for(int k=0;k<polygon.size();++k){
                var p3=polygon.get(k);
                var p4=k!=polygon.size()-1?polygon.get(k+1):polygon.get(0);
                var cp3=CrossProduction(p2.minus(p1),p3.minus(p1));
                var cp4=CrossProduction(p2.minus(p1),p4.minus(p1));
                if(cp3<=0)
                    polygon2.add(p3);
                if(cp3>0 && cp4<0 || cp3<0 && cp4>0){
                    double x1=p1.x, y1=p1.y;
                    double x2=p2.x, y2=p2.y;
                    double x3=p3.x, y3=p3.y;
                    double x4=p4.x, y4=p4.y;
                    double t=((x2-x1)*(y3-y1)-(x3-x1)*(y2-y1))/((x2-x1)*(y3-y4)-(x3-x4)*(y2-y1));
                    Point pcross=new Point(x3+t*(x4-x3),y3+t*(y4-y3));
                    polygon2.add(pcross);
                }
            }
            polygon=polygon2;
            if(polygon.isEmpty())break;
        }
        double area=0;
        for(int i=0;i<polygon.size();++i){
            int ni=i+1;
            if(ni==polygon.size())
                ni=0;
            var cp=CrossProduction(polygon.get(i),polygon.get(ni));
            area+=cp;
        }
        area/=2;
        System.out.printf("%.12f",Math.abs(area));
    }
}



















