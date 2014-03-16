include ../../../environment

all: MainLan MainSeq

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: newGA.req.o newGA.pro.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: newGA.req.o newGA.pro.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq: newGA.req.o newGA.pro.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq newGA.cfg ../../../ProblemInstances/SPHERE-instances/SPH10.txt res/sp10.newga.seq.txt
