#include <iostream.h>
#include <fstream.h>
#include "CLS.hh"
#include "Mallba/States.hh"
#include "Mallba/random.hh"



using skeleton CLS; 


int main (int argc, char** argv)
{

 if (argc!=4) 
 	show_message(1);

 ifstream f1(argv[1]);
 if (!f1) 
 	show_message(11);

 CLS::SetUpParams cfg;
 f1 >> cfg;
 // cout << cfg;
 // continue_question();

 BaseProblem pbm;
 ifstream f2(argv[2]);
 if (!f2) 
 	show_message(12);
 f2 >> pbm;
 // cout << pbm;
 // continue_question();

 BaseSolution sol(pbm);
 char data_stored[sol.size()];
 unsigned long n, l;
 double fit;
 CLS::Solver_Seq solver(pbm,cfg);
 
 solver.run();

 solver.show_state();
 cout << "Best Solution : " << solver.global_best_solution()
      << " Fitness: " << solver.global_best_solution().fitness() << endl;
 cout << "\n\n :( ---------------------- THE END --------------- :) ";

 ofstream fexit(argv[3]);
 if(!fexit) show_message(13);
 fexit << solver.statistics();
 return(0);
}
