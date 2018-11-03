#!/usr/bin/python

import os, time, random
SERVER_LOG = "server.log"

def read_scores():
    f=open(SERVER_LOG)
    s = f.readline()
    f.close()
    s = s.strip().split(':')
    if (s[0].find("Player 1")!=-1):
        return [float(s[1]),float(s[3])]
    else:
        return [float(s[3]),float(s[1])]

def param_string(l):
    s=""
    for i in l:
        s+=str(i)+" "
    return s
def update_params(a_params,b_params):
    f = open("runa.sh",'w')
    f.write("#!/bin/bash\n")
    f.write("../YINSH_AI_bot/runner "+param_string(a_params)+"\n")
    f.close()

    f = open("runb.sh",'w')
    f.write("#!/bin/bash\n")
    f.write("../YINSH_AI_bot/runner "+param_string(b_params)+"\n")
    f.close()

a_params = [2,5,0.5,3.5,2]
b_params = [2,5,0.5,3.5,2]
feature = random.randint(0,10000)%(len(a_params))
modif = 0.9 + 0.2*random.random()
a_params[feature] = a_params[feature]*modif

epochs = 30
while epochs>0:
    epochs-=1
    a_score=0
    b_score=0
    update_params(a_params,b_params)
    for i in xrange(2):
        os.system("python server.py 10000 -n 5 -s 5  -NC 2 -TL 150 -LOG server.log > server.data &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./runa.sh -mode GUI> Player1.data &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./runb.sh > Player2.data ")
        l = read_scores()
        a_score+=l[0]
        b_score+=l[1]
        print "GAME",2*i, l[0], l[1]

        os.system("python server.py 10000 -n 5 -s 5  -NC 2 -TL 150 -LOG server.log > server.data &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./runb.sh -mode GUI> Player1.data &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./runa.sh > Player2.data ")
        l = read_scores()
        a_score+=l[1]
        b_score+=l[0]
        print "GAME",2*i+1 , l[1], l[0]
        
    # tweak a and b params depending on a_score and b_score
    print "EPOCH ",epochs
    print "A_PARAMS ",a_params,a_score
    print "B_PARAMS ",b_params,b_score

    if ((a_score-b_score)>0):
        for i in xrange(len(b_params)):
            b_params[i]=a_params[i]
        # if a wins then use 90% of previous modif on same feature
        # modif = (modif - 0.9) * 0.8 + 0.9
        a_params[feature] = a_params[feature]*modif
    else:
        for i in xrange(len(b_params)):
            a_params[i]=b_params[i]
        feature = random.randint(0,10000)%(len(a_params))
        modif = 0.9 + 0.2*random.random()
        a_params[feature] = a_params[feature]*modif


# a_params = [2,5,0.5,3.5,2]
# update_params(a_params,[])
# os.system("python server.py 10000 -n 5 -s 5  -NC 2 -TL 150 -LOG server.log >server.data &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./shubham.sh -mode GUI >Player1.data &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./runa.sh > Player2.data")
# l = read_scores()
# print l
# os.system("python server.py 10000 -n 5 -s 5  -NC 2 -TL 150 -LOG server.log >server.data &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./runa.sh -mode GUI >Player1.data &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./shubham.sh > Player2.data")
# time.sleep(10)
# l = read_scores()
# print l