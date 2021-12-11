CXXFLAGS = -ggdb -Wall -Wno-sign-compare -std=c++14

all: serverC clientA clientB serverT serverS serverP

network.o: network.h network.cpp
	g++ $(CXXFLAGS) -c network.cpp

serverC: central.cpp network.o
	g++ $(CXXFLAGS)  central.cpp network.o -o serverC

clientA: clientA.cpp network.o
	g++ $(CXXFLAGS)  clientA.cpp network.o -o clientA

clientB: clientB.cpp network.o
	g++ $(CXXFLAGS)  clientB.cpp network.o -o clientB

serverT: serverT.cpp network.o
	g++ $(CXXFLAGS)  serverT.cpp network.o -o serverT

serverS: serverS.cpp network.o
	g++ $(CXXFLAGS)  serverS.cpp network.o -o serverS

serverP: serverP.cpp network.o
	g++ $(CXXFLAGS)  serverP.cpp network.o -o serverP
clean:
	rm -f network.o serverC clientA clientB serverT serverS serverP