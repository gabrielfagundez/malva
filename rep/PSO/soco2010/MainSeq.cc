/*****************************************************************
 *
 * First version of PSO for Mallba v1.0
 * José Manuel García Nieto 28/11/2007
 * File : Pso.cpp STANDARD CONTINUOUS PSO
 * Definition of the PSO skeleton for Mallba
 * For the problem instance: cec2005 benchmark functions
 *
 *****************************************************************
 ** ****************************/

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#include "PSO.hh"
#include "PSO.req.cc"
#include "PSO.pro.cc"

using namespace std;

int main (int argc, char** argv)
{
	using skeleton PSO;
	double bias_f = 0;
	int sys = system("clear");

	if(argc < 4)
		show_message(1);

	ifstream funo ( argv[1] );
	if (!funo) show_message(11);
       

	//ifstream fpro ( argv[2] );
	//if (!fpro) show_message(12);

	SetUpParams cfg;
	funo >> cfg;

	Problem pbm;
	//fpro >> pbm;
	pbm.dimension(atoi(argv[2]));
	pbm.nfunction(atoi(argv[3]));
	cout << pbm.dimension() << " " << pbm.nfunction() << endl;

	Solver_Seq solver(pbm,cfg);

        cfg.swarm_size(10);
	cfg.nb_evolution_steps((5000*pbm.dimension())/cfg.swarm_size()); 
        cfg.particle_size(pbm.dimension());
	
	switch(pbm.nfunction())
		{
		case 1 : bias_f=-450.0; cfg.delta_min(-100.0); cfg.delta_max(100.0); break;
		case 2 : bias_f=-450.0; cfg.delta_min(-100.0); cfg.delta_max(100.0); break;
		case 3 : bias_f= 390.0; cfg.delta_min(-100.0); cfg.delta_max(100.0); break;
		case 4 : bias_f=-330.0; cfg.delta_min(-5.0); cfg.delta_max(5.0);     break;
		case 5 : bias_f=-180.0; cfg.delta_min(-600.0); cfg.delta_max(600.0); break;
		case 6 : bias_f=-140.0; cfg.delta_min(-32.0); cfg.delta_max(32.0);   break;
		case 7 : bias_f= 0.0;   cfg.delta_min(-10.0); cfg.delta_max(10.0);  break;
		case 8 : bias_f= 0.0;   cfg.delta_min(-65.536); cfg.delta_max(65.536);  break;
		case 9 : bias_f= 0.0;   cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 10 : bias_f= 0.0;  cfg.delta_min(-15.0); cfg.delta_max(15.0);  break;
		case 11 : bias_f= 0.0;  cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 12 : bias_f= 0.0;  cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 13 : bias_f= 0.0;  cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 14 : bias_f= 0.0;  cfg.delta_min(-5.0); cfg.delta_max(5.0);  break;
		case 15 : bias_f= 0.0;  cfg.delta_min(-10.0); cfg.delta_max(10.0);  break;
		case 16 : bias_f= 0.0;  cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 17 : bias_f= 0.0;  cfg.delta_min(-100.0); cfg.delta_max(100.0);  break;
		case 18 : bias_f= 0.0;  cfg.delta_min(-5.0); cfg.delta_max(5.0);  break;
		case 19 : bias_f= 0.0;  cfg.delta_min(-10.0); cfg.delta_max(10.0);  break;
		default: bias_f= 0.0;

		}

	solver.run(); /* number of evaluations */

	if (solver.pid()==0)
	{
		solver.show_state();
		cout << "Solution: " << solver.global_best_solution() << endl;
		cout   << " -|- Final Fitness: " << solver.global_best_solution().fitness()-bias_f << endl;
		printf(" --Final Fitness: %.2E \n\n",solver.global_best_solution().fitness()-bias_f);
		ofstream fexit(argv[4]);
		if(!fexit) show_message(13);
		fexit << solver.userstatistics();

	}
	return(0);
}
