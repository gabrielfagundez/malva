include ../../../environment

all: MainSeq MainLan

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainSeq: CHC.req.o CHC.pro.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainLan: CHC.req.o CHC.pro.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: CHC.req.o CHC.pro.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan

WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan

SEQ:
	./MainSeq CHC.cfg ../../../ProblemInstances/ONEMAX-instances/onemax10.txt res/om10.chc.seq.txt
