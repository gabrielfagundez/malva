include ../../../environment
BASE    =
APP     = 
CPPFLAGS	= -I$(MALLBA_INC) -O -Wno-deprecated -g -I$(MALLBA_REP)/$(BASE)/$(APP)/

Main.seq: Main.o CLS.pro.o CLS.req.o $(MALLBA_REP)/$(BASE)/$(APP)/$(BASE).pro.o $(MALLBA_REP)/$(BASE)/$(APP)/$(BASE).req.o $(MALLBA_REP)/$(BASE)/$(APP)/StopCondition.o

	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@

SEQ: Main.seq
	 Main.seq CLS.cfg ../../../ProblemInstances/instances/instance.txt  res/res.txt

clean:
	rm -f *.o *% *~ core Main.seq
all: clean Main.seq
