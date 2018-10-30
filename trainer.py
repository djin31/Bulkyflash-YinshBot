#!/usr/bin/python

import os, time
SERVER_LOG = "server.log"

b_params=[]
a_params=[]

def read_scores():
	f=open(SERVER_LOG)
	s = f.readline()
	f.close()
	s = s.strip().split(':')
	return [float(s[1]),float(s[2])]

def param_string(l):
    s=""
    for i in l:
        s+=str(i)

while True:
	a_score=0
	b_score=0
	for i in xrange(3):
		os.system("python server.py 10000 -n 6 -s 6  -NC 2 -TL 150 -LOG server.log &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./run.sh "+ param_string(b_params)+"-mode GUI &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./run.sh "+ param_string(a_params))
        time.sleep(300)
        l = read_scores()
        a_score+=l[0]
        b_score+=l[1]

        os.system("python server.py 10000 -n 6 -s 6  -NC 2 -TL 150 -LOG server.log &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./run.sh "+ param_string(a_params)+"-mode GUI &")
        time.sleep(0.1)
        os.system("python  client.py 0.0.0.0 10000 ./run.sh "+ param_string(b_params))
        time.sleep(300)
        l = read_scores()
        a_score+=l[1]
        b_score+=l[0]
        
        # tweak a and b params depending on a_score and b_score


# os.system("python server.py 10000 -n 6 -s 6  -NC 2 -TL 150 -LOG server.log &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./run.sh &")
# time.sleep(0.1)
# os.system("python  client.py 0.0.0.0 10000 ./run.sh &")
# time.sleep(300)
# l = read_scores()
# print l