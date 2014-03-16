include ../../../environment

all: MainLan MainSeq

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: SS.req.o SS.pro.o MainLan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: SS.req.o SS.pro.o MainWan.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq: SS.req.o SS.pro.o MainSeq.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq SS.cfg ../../../ProblemInstances/ONEMAX-instances/onemax10.txt res/om10.ss.seq.txt
