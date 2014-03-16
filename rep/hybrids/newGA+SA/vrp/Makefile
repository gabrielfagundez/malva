include ../../../../environment

all: MainLan1 MainLan2 MainLan3 MainSeq1 MainSeq2 MainSeq3

clean:
	rm -f MainLan? MainSeq? MainWan? *.o *% *~

MainLan1: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainLan1.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainLan2: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainLan2.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainLan3: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainLan3.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan1: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainWan1.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan2: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainWan2.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainWan3: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainWan3.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq1: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainSeq1.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq2: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainSeq2.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

MainSeq3: SA.req.o SA.pro.o StopCondition.o  newGA.req.o newGA.pro.o newGASA.req.o newGASA.pro.o MainSeq3.o
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

LAN_1:
	$(RUN)  -v  -p4pg pgfileLan1 MainLan1
WAN_1:
	$(RUN)  -v  -p4pg pgfileWan1 MainWan1
SEQ_1:
	./MainSeq1
LAN_2:
	$(RUN)  -v  -p4pg pgfileLan2 MainLan2
WAN_2:
	$(RUN)  -v  -p4pg pgfileWan2 MainWan2
SEQ_2:
	./MainSeq2
LAN_3:
	$(RUN)  -v  -p4pg pgfileLan3 MainLan3
WAN_3:
	$(RUN)  -v  -p4pg pgfileWan3 MainWan3
SEQ_3:
	./MainSeq3
