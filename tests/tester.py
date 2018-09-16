#!/usr/bin/python

import sys

move = sys.stdin.readline().strip() #reading game params
filename="../YINSH_AI_bot/tests/foo.txt"
if (len(sys.argv)>1):
	filename=sys.argv[1]

f=open(filename,'r')
l=f.readlines()
player_id=-1

if player_id==1:
	move = sys.stdin.readline().strip()
for i in l:
	sys.stdout.write(i+'\n')
	move = sys.stdin.readline().strip()