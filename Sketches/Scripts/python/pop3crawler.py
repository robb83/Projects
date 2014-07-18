import poplib
import re
import os

re_subject = re.compile("^Subject:.*Hello World")
re_from = re.compile("^From:.*(username1@gmail\.com|username2@gmail\.com)")

server = poplib.POP3_SSL("pop.gmail.com")
server.user("username1@gmail.com")
server.pass_("password1")

msgs = server.list()
nmsgs = len(msgs[1])
weakup = None

for i in range(nmsgs):
	lines = server.retr(i + 1)
	ms = None
	mf = None
	for line in lines[1]:
		if (not ms):
			ms = re_subject.match(line)
		if (not mf):
			mf = re_from.match(line)
		if (mf and ms):
			break
	if (mf and ms):
		weakup = 1
		break
server.quit()

if (weakup):
	print "do something"
	os.system("/home/poweruser/a.sh")
