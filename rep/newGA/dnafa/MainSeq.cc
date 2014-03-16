#include "newGA.hh"
#include "ContigBuilder.h"

extern int Global_cutoff;

int main (int argc, char** argv)
{
	using skeleton newGA;

	system("clear");

	if(argc < 4)
		show_message(1);

	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	ifstream f2(argv[2]);
	if (!f2) show_message(12);

	Problem pbm;
	f2 >> pbm;
	Global_cutoff = pbm.cutoff;
	
	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;

	
	Solver_Seq solver(pbm,cfg);
	solver.run();

	if (solver.pid()==0)
	{
		solver.show_state();
		Solution s(solver.global_best_solution());
		cout << "Solution: " << s
		     << " Fitness: " << s.fitness() << endl;

/*		ContigBuilder * builder = new ContigBuilder(s.fragments(), pbm.detector->getScoreTable(),&pbm);
		builder->buildContigs();

		builder->mergeContigs();*/
//		builder->buildConsensus();
	
		cout << "\n\n :( ---------------------- THE END --------------- :) ";

		ofstream fexit(argv[3]);
		if(!fexit) show_message(13);
		fexit << solver.userstatistics();
	}

	return(0);
}
