import random
import math

L,R=10,20
N=random.randint(50,100)

def multiply(v1, v2):
    """
    计算两个向量的叉积
    """
    return v1.x*v2.y - v2.x*v1.y


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def __sub__(self, other):
        """
        重载减法运算，计算两点之差，实际上返回的表示该点到另一点的向量
        :param other: 另一个点
        :return: 该点到另一点的向量
        """
        return Point(self.x-other.x, self.y-other.y)


class Segment:
    def __init__(self, point1, point2):
        self.point1 = point1
        self.point2 = point2

    def straddle(self, another_segment):
        """
        :param another_segment: 另一条线段
        :return: 如果另一条线段跨立该线段，返回True；否则返回False
        """
        v1 = another_segment.point1 - self.point1
        v2 = another_segment.point2 - self.point1
        vm = self.point2 - self.point1
        if multiply(v1, vm) * multiply(v2, vm) <= 0:
            return True
        else:
            return False

    def is_cross(self, another_segment):
        """
        :param another_segment: 另一条线段
        :return: 如果两条线段相互跨立，则相交；否则不相交
        """
        if self.straddle(another_segment) and another_segment.straddle(self):
            return True
        else:
            return False

    def cross(self, another_segment):
        return self.is_cross(another_segment)

x_set= {L, R}
def gen_line():
    x_ran=(0,300)
    y_ran=(1,500)
    l_ran=(1,30)

    while 1:
        while 1:
            x1=random.randint(*x_ran)
            if not x1 in x_set:break
        y1=random.randint(*y_ran)
        l=random.randint(*l_ran)
        rad=random.random()*math.pi
        x2=math.ceil(x1+l*math.cos(rad))
        y2=math.ceil(y1+l*math.sin(rad))
        if x1==x2 or y1==y2:continue
        else:break

    if y1>y2:
        x1,x2,y1,y2=x2,x1,y2,y1
    return x1,y1,x2,y2

tarp=[gen_line()]

while len(tarp)<N:
    l=gen_line()
    seg_l=Segment(Point(l[0], l[1]), Point(l[2], l[3]))
    ok=True
    for ol in tarp:
        seg_ol=Segment(Point(ol[0], ol[1]), Point(ol[2], ol[3]))
        if seg_ol.cross(seg_l):
            ok=False
            break
    if ok:
        tarp.append(l)
        x_set.add(l[0])
        x_set.add(l[2])

output=f"{L} {R} {N}\n"
for t in tarp:
    output+=f"{t[0]} {t[1]} {t[2]} {t[3]}\n"

f=open("test.txt","w")
f.write(output)
f.close()
