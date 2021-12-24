import pdb

import matplotlib.pyplot as plt
import numpy as np

l=10
r=20
ps=[
    [32, 50, 12, 60,],
    [30, 60, 8, 70,],
    [25, 70, 0, 80,],
    [15, 30, 28, 40,],
    [5, 20, 14, 25,],
]
topo_sorted=[
    [4, 5],
    [1],
    [2],
    [3],
]

plt.subplot(1,2,1)

for i,p in enumerate(ps):
    plt.plot([p[0],p[2]],[p[1],p[3]],linestyle="solid",color="black")
    plt.text(p[0],p[1],str(i+1))
plt.plot([l,r],[0,0],linestyle="solid",color="green")

plt.subplot(1,2,2)
plt.plot([l,r],[0,0],linestyle="solid",color="green")
for i,arr in enumerate(topo_sorted):
    for j in arr:
        x1=ps[j-1][0]
        x2=ps[j-1][2]
        plt.arrow(x2,i+1,x1-x2,0,head_width=0.1,head_length=1.5,length_includes_head=True)
        plt.text(x2,i+1,str(j))


plt.show()


