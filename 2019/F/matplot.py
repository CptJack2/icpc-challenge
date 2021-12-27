import matplotlib.pyplot as plt

from solution import *

L,R,N,data,allX=get_data_from_input()

dependency=get_dep(N,allX,data)
sorted=topo_sort(dependency)

plt.subplot(1,2,1)

data.pop(0)
ceiling=data.pop()
plt.plot([L, R], [0, 0], linestyle="solid", color="green")
plt.plot([L, R], [ceiling[1], ceiling[1]], linestyle="solid", color="green")
for i,p in enumerate(data):
    plt.plot([p[0],p[2]],[p[1],p[3]],linestyle="solid",color="black")
    plt.text(p[0],p[1],str(i+1))

plt.subplot(1,2,2)
sorted.remove(0)
sorted.remove(N+1)
plt.plot([L, R], [0, 0], linestyle="solid", color="green")
plt.plot([L, R], [N+1, N+1], linestyle="solid", color="green")
#[4, 5, 1, 2, 3]
for i, ind in enumerate(sorted):
    x1=data[ind-1][0]
    x2=data[ind-1][2]
    plt.arrow(x2,i+1,x1-x2,0,head_width=0.1,head_length=1.5,length_includes_head=True)
    plt.text(x2,i+1,str(ind))

plt.show()


