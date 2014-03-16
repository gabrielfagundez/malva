#include "aco.hh"
#include <fstream.h>

using namespace std;

int main (int argc, char** argv)
{
	using skeleton ACO;

	//system("clear");
	if(argc < 4)
		show_message(1);

	ifstream f1(argv[1]);
	if (!f1) show_message(11);

	ifstream f2(argv[2]);
	if (!f2) show_message(12);
	Problem pbm;
	f2 >> pbm;
	Operator_Pool pool(pbm);
	SetUpParams cfg(pool);
	f1 >> cfg;

	Solver_Seq solver(pbm,cfg);

	solver.run();

	solver.show_state();
	cout << "Solution : "
	     << solver.global_best_solution()
	     << endl 
	     <<" Fitness: " 
	     << solver.global_best_solution().fitness() 
	     << endl;
	
	ofstream fexit(argv[3]);
	if(!fexit) show_message(13);
	fexit << solver.userstatistics();
	cout << "\n\n :( ---------------------- THE END --------------- :) "<< endl;

	return(0);
}


