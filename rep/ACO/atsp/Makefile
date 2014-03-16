include ../../../environment

all: MainLan MainSeq

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: aco.pro.o aco.req.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: aco.pro.o aco.req.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq: aco.pro.o aco.req.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq aco.cfg ../../../ProblemInstances/ATSP-instances/ftv70.atsp res/atsp.aco.seq.txt
