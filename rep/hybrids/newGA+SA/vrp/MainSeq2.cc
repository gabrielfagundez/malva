#include "newGASA.hh"

int main (int argc, char** argv)
{
	using skeleton newGASA;

	char path[MAX_BUFFER];
	int len;
	int longitud;

	system("clear");

	get_path(argv[0],path);
	len = strlen(path) + 1;
	longitud = MAX_BUFFER - len;

	sprintf(path,"%s/newGASA.cfg",path);
	ifstream f(path);
	if(!f) show_message(10);

	SetUpParams cfg;
	f >> cfg;

/*	cout << cfg;
*/
	Solver_Seq_v2 solver;
	solver.run(cfg,argc,argv);

	return(0);
}
