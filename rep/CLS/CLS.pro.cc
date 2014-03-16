/************************************************ 
***				  	      *** 
***  Cooperative Local Search Skeleton  v1.0  *** 
***  Provided classes and methods             ***
***  Developed by: Carlos Cotta Porras        *** 
***                                           *** 
************************************************/


#include <iostream.h>
#include <fstream.h>
#include "CLS.hh"
#include "Mallba/random.hh"
#include "Mallba/time.hh"

skeleton CLS {


  // SetUpParams -----------------------------------------------------------


	SetUpParams::SetUpParams ():
	 _independent_runs(0),
	 _max_evaluations(0),
	 _number_of_solvers(0),
	 _execution_granularity(0)
	{
	}


	istream& operator>> (istream& is, SetUpParams& setup)
	{
		 char buffer[MAX_BUFFER]; // current line in the setup file
		 char filename[MAX_BUFFER]; // name of the file with the base parameters
		 int op;
		 short int nb_param=0;

		 while ((nb_param<4) && (is.getline(buffer,MAX_BUFFER,'\n'))) {
		    	op=-1;
			sscanf(buffer," %d ",&op);
		    
			if (op>=0) {
				 switch (nb_param) {
					  case 0: setup.independent_runs(op);
						  break;
					  case 1: setup.max_evaluations(op);
						  break;
 					  case 2: setup.number_of_solvers(op);
						  break;
					  case 3: setup.execution_granularity(op);
						  break;
				 }
				 nb_param++;
			}
		 }
		is.getline(buffer, MAX_BUFFER, '\n');
		sscanf(buffer, "%s", filename);
		ifstream ifs(filename);
		ifs >> setup._baseSetUp;

		return is;
	}


	ostream& operator<< (ostream& os, const SetUpParams& setup)
	{
		 os << "CONFIGURATION -------------------------------------------" << endl << endl;
		 os << "\t" << "Independent runs :     " << setup.independent_runs()    << endl
		    << "\t" << "Evaluation steps:      " << setup.max_evaluations() << endl
		    << "\t" << "Number of solvers:     " << setup.number_of_solvers() << endl
		    << "\t" << "Execution granularity: " << setup.execution_granularity() << endl << endl
		    << "BASE " << setup.baseSetUp() << endl;
		 os << endl << endl << "END CONFIGURATION -------------------------------------------" << endl << endl;
		 return os;
	}


	/*
	friend NetStream& operator << (NetStream& ns, const SetUpParams& setup)
		{
		 return ns;
		}

	friend NetStream& operator >> (NetStream& ns, SetUpParams& setup)
		{
		 return ns;
		}
	*/

	const unsigned int SetUpParams::independent_runs() const
	{
		 return _independent_runs;
	}

	const unsigned int SetUpParams::max_evaluations() const
	{
		 return _max_evaluations;
	}

	const unsigned int SetUpParams::number_of_solvers() const
	{
		 return _number_of_solvers;
	}

	const unsigned int SetUpParams::execution_granularity() const
	{
		 return _execution_granularity;
	}

	const BaseSetUpParams SetUpParams::baseSetUp() const
	{
		return _baseSetUp;
	}
	

	void SetUpParams::independent_runs(const unsigned int val)
	{
		_independent_runs = val;
	}
	void SetUpParams::max_evaluations(const unsigned int val)
	{
		_max_evaluations= val;
	}
	void SetUpParams::number_of_solvers(const unsigned int val)
	{
		_number_of_solvers= val;
	}
	void SetUpParams::execution_granularity(const unsigned int val)
	{
		_execution_granularity= val;
	}
	void SetUpParams::baseSetUp(const BaseSetUpParams& val)
	{
		_baseSetUp = val;
	}
	
	SetUpParams::~SetUpParams()
	{
	}



  // Statistics ------------------------------------------------------


	Statistics::Statistics()
	{}


	ostream& operator<< (ostream& os, const Statistics& stats)
		{
		 int j;
		 os << "\n---------------------------------------------------------------" << endl;
		 os << "                   STATISTICS OF CURRENT TRIAL                   " << endl;
		 os << "------------------------------------------------------------------" << endl;
		 for (int i=0;i< stats.stats_data.size();i++)
			{
			 os << endl
			    << " Evaluations: " << stats.stats_data[i].nb_evaluations
			    << " Best:        " << stats.stats_data[i].best_cost
			    << " Current:     " << stats.stats_data[i].current_cost;
			}
		 os << endl << "------------------------------------------------------------------" << endl;
		 return os;
		}


	/*
	 friend NetStream& operator << (NetStream& ns, const Statistics& stats )
		{
		 return ns;
		}

	 friend NetStream& operator >> (NetStream& ns, Statistics& stats )
		{
		 return ns;
		}
	*/

	Statistics& Statistics::operator= (const Statistics& stats)
	{
		 stats_data = stats.stats_data;
		 return *this;
	}


	void Statistics::update(const Solver_Seq& solver)
	{
		 struct stat *new_stat=(struct stat *)malloc(sizeof(struct stat));
		 new_stat->trial         = solver.current_trial(); 
		 new_stat->nb_evaluations= solver.current_iteration();
		 new_stat->best_cost     = solver.current_best_cost();
		 new_stat->current_cost  = solver.current_cost(); 
		 stats_data.append(*new_stat);
	}


	Statistics::~Statistics()
	{ 
		stats_data.remove(); 		
	}
	
	void Statistics::clear()
	{
		stats_data.remove();
	}



	// Solver (superclasse)---------------------------------------------------


    Solver::Solver (const BaseProblem& pbm, const SetUpParams& setup)
	: problem(pbm),
	  params(setup),
	  solvers(),
	 _stat(),
	 _userstat(),
	 _sc(),
	 _direction(pbm.direction()),
	 _current_trial("_current_trial",_sc),
	 _current_iteration("_current_iteration",_sc),
	 _current_best_solution("_current_best_solution",_sc),
	 _current_best_cost("_current_best_cost",_sc),
	 _current_solution("_current_solution",_sc), 
	 _current_cost("_current_cost",_sc), 
	 _global_best_solution("_global_best_solution",_sc),
	 _global_best_cost("_global_best_cost",_sc),
	 _display_state("_display_state", _sc)
    {

	current_trial(0);
	current_iteration(0);
	display_state(1);
	current_best_cost(problem.infinity()); 
	current_cost(problem.infinity()); 
	global_best_cost(problem.infinity()); 


	int num = params.number_of_solvers();
	solvers = (BaseSolver**) malloc (num*sizeof(BaseSolver*));
	BaseSetUpParams cfg = params.baseSetUp();
	for (int i=0; i<num; i++)  
		solvers[i] = new BaseSolver(problem, cfg);
	
    }


	unsigned int Solver::current_trial() const
	{
		 unsigned int value=0;
		 unsigned long nitems,length;
		 _sc.get_contents_state_variable("_current_trial",(char *)&value, nitems, length);
		 return value;
	}

	unsigned int Solver::current_iteration() const
	{
		 unsigned int value=0;
		 unsigned long nitems,length;
		 _sc.get_contents_state_variable("_current_iteration",(char *)&value, nitems, length);
		 return value;
	}

	Solution Solver::current_best_solution() const
	{
        BaseSolution sol(problem);
        unsigned long nitems,length;
        char data_stored[sol.size()];
        _sc.get_contents_state_variable("_current_best_solution", data_stored, nitems, length);
        sol.to_Solution(data_stored);
        return sol;
	}

	double Solver::current_best_cost() const
	{
		 double value=0.0;
		 unsigned long nitems,length;
		 _sc.get_contents_state_variable("_current_best_cost",(char *)&value, nitems, length);
		 return value;
	}

	Solution Solver::current_solution() const 
	{ 
        BaseSolution sol(problem);
        unsigned long nitems,length;
        char data_stored[sol.size()];
        _sc.get_contents_state_variable("_current_solution", data_stored, nitems, length);
        sol.to_Solution(data_stored);
        return sol;
	} 
 
	double Solver::current_cost() const 
	{ 
		 double value=0.0; 
		 unsigned long nitems,length; 
		 _sc.get_contents_state_variable("_current_cost",(char *)&value, nitems, length); 
		 return value; 
	} 


	Solution Solver::global_best_solution() const
	{
        BaseSolution sol(problem);
        unsigned long nitems,length;
        char data_stored[sol.size()];
        _sc.get_contents_state_variable("_global_best_solution", data_stored, nitems, length);
        sol.to_Solution(data_stored);
        return sol;
	}

	double Solver::global_best_cost() const
	{
		 double value=0.0;
		 unsigned long nitems,length;
		 _sc.get_contents_state_variable("_global_best_cost",(char *)&value, nitems, length);
		 return value;
	}


	int Solver::display_state() const
	{
		 int value=0;
		 unsigned long nitems,length;
		 _sc.get_contents_state_variable("_display_state",(char *)&value, nitems, length);
		 return value;
	}




	void Solver::current_trial(const unsigned int value)
	{
		 _sc.set_contents_state_variable("_current_trial",(char *)&value,1,sizeof(int));
	}

	void Solver::current_iteration(const unsigned int value)
	{
		 _sc.set_contents_state_variable("_current_iteration",(char *)&value,1,sizeof(int));
	}

	void Solver::current_best_solution(const BaseSolution& sol)
	{
		 _sc.set_contents_state_variable("_current_best_solution",sol.to_String(),1,sol.size());
	}

	void Solver::current_best_cost(const double value)
	{
		_sc.set_contents_state_variable("_current_best_cost",(char *)&value,1,sizeof(double));
	}

	void Solver::current_solution(const BaseSolution& sol) 
	{ 
		 _sc.set_contents_state_variable("_current_solution",sol.to_String(),1,sol.size()); 
	} 
 
	void Solver::current_cost(const double value) 
	{ 
		_sc.set_contents_state_variable("_current_cost",(char *)&value,1,sizeof(double)); 
	} 

	void Solver::global_best_solution(const BaseSolution& sol)
	{
		_sc.set_contents_state_variable("_global_best_solution",sol.to_String(),1,sol.size());
	}

	void Solver::global_best_cost(const double value)
	{
		_sc.set_contents_state_variable("_global_best_cost",(char *)&value,1,sizeof(double));
	}


	void Solver::display_state(const int value)
	{
		_sc.set_contents_state_variable("_display_state",(char *)&value,1,sizeof(int));
	}




	const Statistics& Solver::statistics() const
	{
		 return _stat;
	}

	const UserStatistics& Solver::userstatistics() const
	{
		 return _userstat;
	}


	void Solver::KeepHistory(const BaseSolution& sol, const double curfit)
	{

		  bool betterG=false;
		  bool betterT=false;

		  switch (_direction)
			{
			case minimize:
				     {
				       betterG = (curfit < global_best_cost());
				       betterT = (curfit < current_best_cost());
				       break;
				      }
			case maximize:
				      {
				       betterG = (curfit > global_best_cost());
				       betterT = (curfit > current_best_cost());
				       break;
				      }
			}


		  if (betterT)	{
			 current_best_solution(current_solution());
			 current_best_cost(current_cost());
			 if (betterG) {
				 global_best_solution(current_best_solution());
				 global_best_cost(current_best_cost());
			}
		  }

	}

	
	StateCenter* Solver::GetState()
	{
		return &_sc;
	}

	void Solver::RefreshState() 
	{
		int num = params.number_of_solvers();
		int pos;
		double fit, bestfit;
		unsigned long n, l;
		BaseSolver* bsolv;
		StateCenter*  bstat;

		bestfit = problem.infinity();
		
		for (int i=0; i<num; i++) {
			bsolv = solvers[i];
			bstat = bsolv->GetState();
			bstat->get("_current_best_cost").get_contents((char*)&fit,n,l);
			switch (_direction) {
				case maximize: if (fit > bestfit) {
						pos = i;
						bestfit = fit;
					  }
					  break;

				case minimize: if (fit < bestfit) {
						pos = i;
						bestfit = fit;
					  }
					  break;
			}
		}

		BaseSolution sol(problem);
		char data[sol.size()];
		bsolv = solvers[pos];
		bstat = bsolv->GetState();
		bstat->get("_current_best_solution").get_contents(data,n,l);
		sol.to_Solution(data);

		current_cost(bestfit);
		current_solution(sol);

		KeepHistory(sol,bestfit);

		if (display_state()) 
			show_state(); 
 
	}



	void Solver::UpdateFromState() 
	{
		// Not yet defined.
	}



	void Solver::show_state() const
	{
		 cout << endl << "Current trial:     " << current_trial();
		 cout << endl << "Current iteration: " << current_iteration();
		 cout << endl << "Current cost:      " << current_cost(); 
		 cout << endl << "Trial best cost:   " << current_best_cost();
		 cout << endl << "Global best cost:  " << global_best_cost();
		 cout << endl << "Current solution:  " << current_solution(); 
		 cout << endl << "Trial best solution: " << current_best_solution();
		 cout << endl << "Global solution:     " << global_best_solution() << endl;
	}


	Solver::~Solver()
	{
		_sc.removeAll();

	}




	// Solver sequencial -----------------------------------------------------


	Solver_Seq::Solver_Seq (const Problem& pbm, const SetUpParams& setup)
	: Solver(pbm,setup)
	{
	}


	Solver_Seq::~Solver_Seq ()
	{
		int num = params.number_of_solvers();
				
		for (int i=0; i<num; i++) {
			BaseSolver* bsolv = solvers[i];
			delete bsolv;
		}
		free(solvers);

	}
	
	

	void Solver_Seq::StartUp()
	{
		BaseSolver* bsolv;
		int num = params.number_of_solvers();

		for (int i=0; i<num; i++) {
			bsolv = solvers[i];
			bsolv->StartUp();
		}
 
		current_trial(current_trial()+1); 
		current_iteration(0); 

 		RefreshState(); 

		_stat.update(*this); 
		_userstat.update(*this); 

		continue_question();
	}


	void Solver_Seq::StartUp(const BaseSolution& sol)
	{

		int num = params.number_of_solvers();
				
		for (int i=0; i<num; i++) {
			BaseSolver* bsolv = solvers[i];
			bsolv->StartUp(sol);
		}

		current_trial(current_trial()+1); 
		current_iteration(0); 

		RefreshState();

		_stat.update(*this); 
		_userstat.update(*this); 

		  
	}
	
	void Solver_Seq::DoStep()
	{
		int num = params.number_of_solvers();
		BaseSolver* bsolv;
				
		current_iteration(current_iteration()+1); 

		for (int i=0; i<num; i++) {
			bsolv = solvers[i];
			for (int j=0; j<params.execution_granularity(); j++)
				bsolv->DoStep();
		}

		RefreshState();

		_stat.update(*this); 
		_userstat.update(*this); 

		BaseSolution sol(problem);
		char data[sol.size()];
		double fit;
		unsigned long n, l;
		_sc.get("_current_best_cost").get_contents((char*)&fit,n,l);
		_sc.get("_current_best_solution").get_contents(data,n,l);
		sol.to_Solution(data);
		
		for (int i=0; i<num; i++) {
			bsolv = solvers[i];
			StateCenter* bstat = bsolv->GetState();
			bstat->get("_current_best_cost").set_contents((char*)&fit,1,sizeof(double));
			bstat->get("_current_best_solution").set_contents(sol.to_String(),1,sol.size());
			bsolv->UpdateFromState();
		}
	}


	void Solver_Seq::run (unsigned long int max_evaluations)
	{
		StartUp();

		while (current_iteration()<max_evaluations) 
			DoStep();		
	}

	void Solver_Seq::run (const BaseSolution& sol, unsigned long int max_evaluations)
	{
		StartUp(sol);

		while (current_iteration()<max_evaluations)
 			DoStep();		
	}


	void Solver_Seq::run ()
	{
		  while (current_trial() < params.independent_runs())
			  run(params.max_evaluations());
	}



	
	// Solver LAN ------------------------------------------------------------

	Solver_Lan::Solver_Lan (const BaseProblem& pbm, const SetUpParams& setup):
		    Solver(pbm,setup) {}

	Solver_Lan::~Solver_Lan () {}

	void Solver_Lan::run () {}



	// Solver WAN ------------------------------------------------------------

	Solver_Wan::Solver_Wan (const BaseProblem& pbm, const SetUpParams& setup):
		    Solver(pbm,setup) {}

	Solver_Wan::~Solver_Wan () {}

	void Solver_Wan::run (){}



}


