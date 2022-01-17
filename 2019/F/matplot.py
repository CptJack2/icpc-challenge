import matplotlib.pyplot as plt

def get_data_from_input():
    L,R,N=[int(i) for i in input().split()]
    #tarp位置数据
    data=[(L,0,R,0)]
    #allX是一个二维数组，每一个子数组是allX_stru的数组，存放在这个x坐标下端点的信息
    allX=[]

    for i in range(1,N+1):
        x1,y1,x2,y2=[int(i) for i in input().split()]
        data.append((x1,y1,x2,y2))
        if x1>x2:
            x1,x2=x2,x1

    allX.sort(key=lambda st:st.x)

    return L,R,N,data

L,R,N,data=get_data_from_input()

data.pop(0)
plt.plot([L, R], [0, 0], linestyle="solid", color="green")
for i,p in enumerate(data):
    plt.plot([p[0],p[2]],[p[1],p[3]],linestyle="solid",color="black")
    plt.text(p[0],p[1],str(i+1))

plt.show()


