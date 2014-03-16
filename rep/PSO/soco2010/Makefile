include ../../../environment

all: clean MainSeq #MainLan 

clean:
	rm -f  MainLan MainSeq MainWan core.* *.o *% *~ *.out
	

MainSeq: MainSeq.o 
	$(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -lm -o $@    

MainLan: MainLan.o
	 $(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -lgcj -o $@  

#MainWan: MainWan.o 
#	 $(CXX) $(LDFLAGS) $^ $(LOADLIBES) $(CPPFLAGS) -o $@  
LAN:
	$(RUN)  -v  -p4pg pgfileLan MainLan
WAN:
	$(RUN)  -v  -p4pg pgfileWan MainWan

##############################################################################################
## execution order, config file, problem dimension, number of SOCO 2010 function, results file
SEQ:    
	./MainSeq PSO.cfg 500 19 results.txt

