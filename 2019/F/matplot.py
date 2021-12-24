import matplotlib.pyplot as plt

l=10
r=20
ps=(
32, 50, 12, 60,
30, 60, 8, 70,
25, 70, 0, 80,
15, 30, 28, 40,
5, 20, 14, 25,
)

for i in range(0,len(ps),4):
    plt.plot([ps[i],ps[i+2]],[ps[i+1],ps[i+3]],linestyle="solid",color="black")
    plt.text(ps[i],ps[i+1],str(int(i/4+1)))
plt.plot([l,r],[0,0],linestyle="solid",color="green")

plt.show()


