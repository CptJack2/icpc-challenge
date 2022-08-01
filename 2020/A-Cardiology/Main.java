import java.util.*;
import java.util.function.BiFunction;
import java.util.function.Function;

public class Main {
	static long r,c;
	static class pll{
		long r,c;
		pll(long rr,long cc){r=rr;c=cc;}
		pll(pll p2){r=p2.r; c=p2.c;}
		boolean equals(pll p2){return r==p2.r && c==p2.c;}
	}
	public static  void main(String args[]){
		Scanner sc=new Scanner(System.in);
		r=sc.nextLong();
		c=sc.nextLong();
		class ansStru{
			pll rc;
			int iterationTimes;
			ansStru(){rc=new pll(0,0);}
		}
		ArrayList<ansStru> stablePos=new ArrayList<>((int)c);
		for(int i=0;i<c;++i){stablePos.add(new ansStru());}
		for(long p=0;p<c;++p){
			pll start=new pll(0,0), end=new pll(r-1,c-1);
			int iterationTime=0;
			pll pthStart=new pll(r*p/c,r*p%c),
				pthEnd=new pll((r*p+c-1)/c,(r*p+c-1)%c);
			while(true){
				BiFunction<pll, Long,pll> rcAdd=(pll rc, Long v)->{
					pll ret=new pll(rc);
					ret.r+=v/c;
					v%=c;
					ret.c+=v;
					if(ret.c>=c){
						ret.c%=c;
						++ret.r;
					}
					return ret;
				};
				pll oS=new pll(start),
					oE=new pll(end);
				//rcAdd
				start=rcAdd.apply(pthStart,start.r);
				end=rcAdd.apply(pthStart,end.r);
				if(oS.equals(start) && oE.equals(end))
					break;
				++iterationTime;
			}
			stablePos.get((int) p).rc=(start.equals(end)?new pll(start):new pll(-1,-1));
			stablePos.get((int)p).iterationTimes=iterationTime;
		}
		int ans=0;
		long minDist=999999999;
		Function<pll,Long> calDist=(pll rc)->{
			final long mr=r/2, mc=c/2;
			long minr=Math.abs(rc.r-mr),
				minc=Math.abs(rc.c-mc);
			if(r%2==0)
				minr=Math.min(minr,Math.abs(rc.r-mr+1));
			if(c%2==0)
				minc=Math.min(minc,Math.abs(rc.c-mc+1));
			return minr+minc;
		};
		for(int i=0;i<stablePos.size();++i){
			var pi=stablePos.get(i).rc;
			var dist=calDist.apply(pi);
			if((!pi.equals(new pll(-1,-1))) && dist<minDist){
				minDist=dist;
				ans=i;
			}
		}
		System.out.printf("%d %d %d %d",ans+1,stablePos.get(ans).rc.r+1,
			stablePos.get(ans).rc.c+1,stablePos.get(ans).iterationTimes);
	}
}
