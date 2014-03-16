#include "newGASA.hh"

int main (int argc, char** argv)
{
	using skeleton newGASA;

	char path[MAX_BUFFER];
	char path2[MAX_BUFFER];

	system("clear");

	get_path(argv[0],path);
	strcpy(path2,path);

	strcat(path,"newGASA.cfg");
	ifstream f(path);
	if(!f) show_message(10);

	SetUpParams cfg(path2);
	f >> cfg;

	Solver_Seq_v2 solver;
	solver.run(cfg,argc,argv);

	return(0);
}
