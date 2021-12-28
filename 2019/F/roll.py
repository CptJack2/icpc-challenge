with open('test.txt', 'r') as file:
    data = file.readlines()

splited=data[0].split()
N=int(splited[2])
N+=1
splited[2]=str(N)
data[0]=" ".join(splited)+"\n"
with open('test.txt', 'w') as file:
    file.writelines( data )