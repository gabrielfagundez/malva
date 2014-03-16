include ../../../environment

all: MainLan MainSeq

clean:
	rm -f MainLan MainSeq MainWan *.o *% *~

MainLan: newGA.req.o newGA.pro.o MainLan.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX)  $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan: newGA.req.o newGA.pro.o MainWan.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX)  $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq: newGA.req.o newGA.pro.o MainSeq.o OverlapDetector.o sga.o AdjKeys.o ContigBuilder.o ConsensusBuilder.o LayoutBuilder.o
	$(CXX)  $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan
SEQ:
	./MainSeq newGA.cfg ~/Mallba/ProblemInstances/DNAFA-instances/pbm.txt res/dnafa.newga.seq.txt
