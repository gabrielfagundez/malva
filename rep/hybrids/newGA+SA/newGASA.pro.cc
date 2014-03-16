#ifndef PRO_newGASA
#define PRO_newGASA

#include "newGASA.hh"
#include "newGASAaux.hh"
#include "SA.hh"
#include "newGA.hh"

skeleton newGASA
{

//  Improve: Intra_operator -------------------------------------------------------------

	Improve::Improve():Intra_Operator(2)
	{
		 my_sa=NULL;
		 probability = new float[1];
		 _evaluations = 0;
	}


	void Improve::do_improve(Solution& sol) const
	{
		 if (my_sa!=NULL)
		 {
		  	my_sa->run(sol,my_sa->setup().max_evaluations());
			sol=my_sa->current_best_solution();
			_evaluations += my_sa->current_iteration();
		 }
	}

	void Improve::set_sa(SA::Solver* sa)
	{
		 my_sa=sa;
	}

	SA::Solver* Improve::get_sa() const
	{
		 return my_sa;
	}

	void Improve::execute(Rarray<Solution*>& sols) const
	{
		 for (int i=0;i<sols.size();i++)
			 if(rand01()<=probability[0])
				do_improve(*sols[i]);
	}

	ostream& operator<< (ostream& os, const Improve&  improve)
	{
		 os << "Improve." << " Probability: " << improve.probability[0];
		 return os;
	}

	void Improve::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	void Improve::RefreshState(const StateCenter& _sc) const
	{
		unsigned long nbytes, length, evals;
		_sc.get_contents_state_variable("_current_evaluations",(char *)&evals,nbytes,length);
		evals += _evaluations;
		_evaluations = 0;
		_sc.set_contents_state_variable("_current_evaluations",(char *)&evals,1,sizeof(unsigned long));
		_sc.set_contents_state_variable("_user_op_probability0",(char *)&probability,1,sizeof(float));
	}

	void Improve::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_user_op_probability0",(char *)&probability,nbytes,length);
	}


	Improve::~Improve()
	{
		 delete my_sa;
		 delete [] probability;
	}

// SetUpParams -------------------------------------------------------------------------------
		SetUpParams::SetUpParams()
		:_select_to_SA(0),
		 _parameter_select_to_SA(0),
		 _number_solutions_SA(0),
		 _nGA_Cfg(NULL),
		 _SA_Cfg(NULL),
		 _pbm(NULL),
		 _res(NULL)
		{
			_path = new char[1];
			_path[0] = '\0';
		}

		SetUpParams::SetUpParams(char *path)
		:_select_to_SA(0),
		 _parameter_select_to_SA(0),
		 _number_solutions_SA(0),
		 _nGA_Cfg(NULL),
		 _SA_Cfg(NULL),
		 _pbm(NULL),
		 _res(NULL)
		{
				_path = new char[strlen(path)+1];
				strcpy(_path,path);
		}
			 
		ostream& operator<< (ostream& os, const SetUpParams& setup)
		{
			os << endl << " nGA+SA CONFIGURATION: " << endl
			           << "=======================" << endl
				   << "Selection SA: " << setup.select_to_SA() << endl
				   << "Paramater Select SA: " << setup.parameter_select_to_SA() << endl
				   << "Number of solution passes to SA: " << setup.number_solutions_SA() << endl
				   << "nGA Configuration: " << setup.nGA_Cfg() << endl
				   << "SA Configuration: " << setup.SA_Cfg() << endl
				   << "Problem File: " << setup.pbm() << endl
				   << "Resultate File: " << setup.res() << endl;
			return os;
		}

		istream& operator>> (istream& is, SetUpParams& setup)
		{
			char buffer[MAX_BUFFER]; // current line in the setup file
			char command[100];
			int parameter;
			short int nb_section = 0;
			short int nb_param=0;
			short int nb_cfg = 0;
			short int nb_problem = 0;
			char *aux;

			while (is.getline(buffer,MAX_BUFFER,'\n'))
			{
				sscanf(buffer," %s ",command);
				if (!(strcmp(command,"Parameters")))
				{
					nb_section=0;
					continue;
				}
				if (!(strcmp(command,"Configuration-Files")))
				{
					nb_section=1;
					continue;
				}
				if (!(strcmp(command,"Problem-Files")))
				{
					nb_section=2;
					continue;
				}

				switch (nb_section)
				{
					case 0: sscanf(buffer," %d ",&parameter);
						switch (nb_param)
						{
							case 0: setup.select_to_SA(parameter); break;
					 	 	case 1: setup.parameter_select_to_SA(parameter); break;
							case 2: setup.number_solutions_SA(parameter); break;
						}
						nb_param++;
						break;
					case 1: aux = new char[strlen(buffer)+strlen(setup._path)+1];
						strcpy(aux,setup._path);
						strcat(aux,buffer);
						switch (nb_cfg)
						{
							case 0: setup.nGA_Cfg(aux); break;
					 	 	case 1: setup.SA_Cfg(aux); break;
							default : delete [] aux;
						}
						nb_cfg++;
						break;
					case 2: aux = new char[strlen(buffer)+strlen(setup._path)+1];
						strcpy(aux,setup._path);
						strcat(aux,buffer);
						switch (nb_problem)
						{
							case 0: setup.pbm(aux); break;
					 	 	case 1: setup.res(aux); break;
							default : delete [] aux;
						}
						nb_problem++;
						break;
				}
			}

			return is;
		}

		const unsigned int    SetUpParams::select_to_SA() const
		{
			return _select_to_SA;
		}

		const unsigned int    SetUpParams::parameter_select_to_SA() const
		{
			return _parameter_select_to_SA;
		}

		const unsigned int    SetUpParams::number_solutions_SA() const
		{
			return _number_solutions_SA;
		}

		const char*	      SetUpParams::nGA_Cfg() const
		{
			return _nGA_Cfg;
		}

		const char*	      SetUpParams::SA_Cfg() const
		{
			return _SA_Cfg;
		}

		const char*	      SetUpParams::pbm() const
		{
			return _pbm;
		}

		const char*	      SetUpParams::res() const
		{
			return _res;
		}

		const char*	      SetUpParams::path() const
		{
			return _path;
		}

		void SetUpParams::select_to_SA(const unsigned int val)
		{
			_select_to_SA = val;
		}

		void SetUpParams::parameter_select_to_SA(const unsigned int val)
		{
			_parameter_select_to_SA = val;
		}

		void SetUpParams::number_solutions_SA(const unsigned int val)
		{
			_number_solutions_SA = val;
		}

		void SetUpParams::nGA_Cfg(char *val)
		{
			_nGA_Cfg = val;
		}

		void SetUpParams::SA_Cfg(char *val)
		{
			_SA_Cfg = val;
		}

		void SetUpParams::pbm(char *val)
		{
			_pbm = val;
		}

		void SetUpParams::res(char *val)
		{
			_res = val;
		}

		void SetUpParams::path(char *path)
		{
			_path = path;
		}

		SetUpParams::~SetUpParams()
		{
			if(_nGA_Cfg != NULL) delete [] _nGA_Cfg;
			if(_SA_Cfg != NULL) delete [] _SA_Cfg;
			if(_pbm != NULL) delete [] _pbm;
			if(_res != NULL) delete [] _res;
			if(_path != NULL) delete [] _path;
		}


// Solver -----------------------------------------------------------------------

	Solver::Solver()
	{
	}

	Solver::~Solver()
	{
	}


// Solver_Seq_v1 -----------------------------------------------------------------------

	Solver_Seq_v1::Solver_Seq_v1()
	{
	}

	void Solver_Seq_v1::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Seq solver(pbm,GAcfg);
		solver.run();
		cout << endl << "FINAL"  << endl;
		solver.show_state();
		cout << solver.global_best_solution();
	 	cout << endl << "Total Spent time: " << solver.current_time_spent() << endl;
	 	fres << endl << solver.userstatistics()  << endl << solver.global_best_solution();

	}

	Solver_Seq_v1::~Solver_Seq_v1()
	{
	}

// Solver_Lan_v1 -----------------------------------------------------------------------

	Solver_Lan_v1::Solver_Lan_v1()
	{
	}


	void Solver_Lan_v1::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Lan solver(pbm,GAcfg,argc,argv);
		solver.run();

		if (solver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			solver.show_state();
			cout << solver.global_best_solution();
		 	cout << endl << "Total Spent time: " << solver.current_time_spent() << endl;
		 	fres << endl << solver.userstatistics() << endl << solver.global_best_solution();
		}
	}


	Solver_Lan_v1::~Solver_Lan_v1()
	{
	}

// Solver_Wan_v1 -----------------------------------------------------------------------

	Solver_Wan_v1::Solver_Wan_v1()
	{
	}


	void Solver_Wan_v1::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Wan solver(pbm,GAcfg,argc,argv);
		solver.run();

		if (solver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			solver.show_state();
			cout << solver.global_best_solution();
		 	cout << endl << "Total Spent time: " << solver.current_time_spent() << endl;
		 	fres << endl << solver.userstatistics() << endl << solver.global_best_solution();
		}
	}


	Solver_Wan_v1::~Solver_Wan_v1()
	{
	}

// Solver_Seq_v2 -----------------------------------------------------------------------

	Solver_Seq_v2::Solver_Seq_v2()
	{
	}

	void Solver_Seq_v2::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Seq GAsolver(pbm,GAcfg);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
			GAsolver.run(GAcfg.nb_evolution_steps());

			// Aplico SA
			pool.selector(params.select_to_SA()).prepare(GAsolver.population().fitness_values(),false);
			for (int i = 0; i < params.number_solutions_SA(); i++)
			{
				Solution& selected_solution= *(GAsolver.population().parents()[pool.selector(params.select_to_SA()).select_one(GAsolver.population().parents(),GAsolver.population().offsprings(),GAsolver.population().fitness_values(),params.parameter_select_to_SA(),false).index]);
				SAsolver.run(selected_solution,SAcfg.max_evaluations());
				selected_solution=SAsolver.current_best_solution();
				GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial() + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());

				GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
				GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());
			}
			GAsolver.UpdateFromState();
			GAsolver.RefreshCfgState();
			GAsolver.statistics().update(GAsolver);
			GAsolver.userstatistics().update(GAsolver);
		}

		cout << endl << "FINAL"  << endl;
		GAsolver.show_state();
		cout << GAsolver.global_best_solution();
	 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
	 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
	}


	Solver_Seq_v2::~Solver_Seq_v2()
	{
	}

// Solver_Lan_v2 -----------------------------------------------------------------------

	Solver_Lan_v2::Solver_Lan_v2()
	{
	}

	void Solver_Lan_v2::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));


		newGA::Solver_Lan GAsolver(pbm,GAcfg,argc,argv);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
			GAsolver.run(GAcfg.nb_evolution_steps());
			// Aplico SA

		 	if (GAsolver.pid()!=0)
			{
				pool.selector(params.select_to_SA()).prepare(GAsolver.population().fitness_values(),false);

				for (int i=0;i < params.number_solutions_SA();i++)
				{
					Solution& selected_solution= *(GAsolver.population().parents()[pool.selector(params.select_to_SA()).select_one(GAsolver.population().parents(),GAsolver.population().offsprings(),GAsolver.population().fitness_values(),params.parameter_select_to_SA(),false).index]);
					SAsolver.run(selected_solution,SAcfg.max_evaluations());
					selected_solution=SAsolver.current_best_solution();
					GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial() + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());
					GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
					GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());

				}

				GAsolver.UpdateFromState();
				GAsolver.RefreshCfgState();
				GAsolver.send_local_state_to(-1);
			}
			else
			{
				GAsolver.end_trial(false);
				GAsolver.check_for_refresh_global_state();
			}
		}

		if (GAsolver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			GAsolver.show_state();
			cout << GAsolver.global_best_solution();
		 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
		 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
		}
	}

	Solver_Lan_v2::~Solver_Lan_v2()
	{
	}

// Solver_Wan_v2 -----------------------------------------------------------------------

	Solver_Wan_v2::Solver_Wan_v2()
	{
	}

	void Solver_Wan_v2::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));


		newGA::Solver_Wan GAsolver(pbm,GAcfg,argc,argv);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		
		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
			GAsolver.run(GAcfg.nb_evolution_steps());
			// Aplico SA

		 	if (GAsolver.pid()!=0)
			{
				pool.selector(params.select_to_SA()).prepare(GAsolver.population().fitness_values(),false);

				for (int i=0;i < params.number_solutions_SA();i++)
				{
					Solution& selected_solution= *(GAsolver.population().parents()[pool.selector(params.select_to_SA()).select_one(GAsolver.population().parents(),GAsolver.population().offsprings(),GAsolver.population().fitness_values(),params.parameter_select_to_SA(),false).index]);
					SAsolver.run(selected_solution,SAcfg.max_evaluations());
					selected_solution=SAsolver.current_best_solution();
					GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial() + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());
					GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
					GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());

				}

				GAsolver.UpdateFromState();
				GAsolver.RefreshCfgState();
				GAsolver.send_local_state_to(-1);
			}
			else
			{
				GAsolver.end_trial(false);
				GAsolver.check_for_refresh_global_state();
			}
		}

		if (GAsolver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			GAsolver.show_state();
			cout << GAsolver.global_best_solution();
		 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
		 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
		}
	}

	Solver_Wan_v2::~Solver_Wan_v2()
	{
	}

// Solver_Seq_v3 -----------------------------------------------------------------------

	Solver_Seq_v3::Solver_Seq_v3()
	{
	}


	void Solver_Seq_v3::run(SetUpParams& params,int argc,char** argv)
	{
		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Seq GAsolver(pbm,GAcfg);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
	        	GAsolver.run(GAcfg.nb_evolution_steps());

			for (int i=0;i<GAcfg.population_size();i++)
			{
				if (rand01() < ((double)params.number_solutions_SA()/GAcfg.population_size()))
				{
					Solution& selected_solution= *((GAsolver.population().parents())[i]);
					SAsolver.run(selected_solution,SAcfg.max_evaluations());
					selected_solution=SAsolver.current_best_solution();
					GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial()  + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());
					GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
					GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());

				}
			}
			GAsolver.UpdateFromState();
				GAsolver.RefreshCfgState();
			GAsolver.statistics().update(GAsolver);
			GAsolver.userstatistics().update(GAsolver);
		}

		cout << endl << "FINAL"  << endl;
		GAsolver.show_state();
		cout << GAsolver.global_best_solution();
	 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
	 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
	}


	Solver_Seq_v3::~Solver_Seq_v3()
	{
	}

// Solver_Lan_v3 -----------------------------------------------------------------------

	Solver_Lan_v3::Solver_Lan_v3()
	{
	}

	void Solver_Lan_v3::run(SetUpParams& params,int argc,char** argv)
	{

		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Lan GAsolver(pbm,GAcfg,argc,argv);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
 			GAsolver.run(GAcfg.nb_evolution_steps());

			if (GAsolver.pid()!=0)
			{
				for (int i=0;i<GAcfg.population_size();i++)
				{
					if (rand01() < ((double)params.number_solutions_SA()/GAcfg.population_size()))
					{
						Solution& selected_solution= *((GAsolver.population().parents())[i]);
						SAsolver.run(selected_solution,SAcfg.max_evaluations());
						selected_solution=SAsolver.current_best_solution();
						GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial()  + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());
						GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
						GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());
					}
				 }

				 GAsolver.UpdateFromState();
				GAsolver.RefreshCfgState();
				 GAsolver.send_local_state_to(-1);
			}
			else
			{
				 GAsolver.end_trial(false);
				 GAsolver.check_for_refresh_global_state();
			}
		}

		if (GAsolver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			GAsolver.show_state();
			cout << GAsolver.global_best_solution();
		 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
		 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
		}
	}

	Solver_Lan_v3::~Solver_Lan_v3()
	{
	}

// Solver_Wan_v3 -----------------------------------------------------------------------

	Solver_Wan_v3::Solver_Wan_v3()
	{
	}

	void Solver_Wan_v3::run(SetUpParams& params,int argc,char** argv)
	{

		ifstream fpbm(params.pbm());
		ofstream fres(params.res());
		ifstream fnGA(params.nGA_Cfg());
		ifstream fSA(params.SA_Cfg());
		// If a file isn't found, it shows a message in the screen and stops the execution
		if (!fpbm) show_message(12);
		if (!fres) show_message(13);
		if (!fnGA || !fSA) show_message(11);

		Problem pbm;
		fpbm >> pbm;
		newGA::Operator_Pool pool(pbm);

		newGA::SetUpParams GAcfg(pool);
		SA::SetUpParams SAcfg;

		fnGA >> GAcfg;
		fSA >> SAcfg;

		for(int i = 0; i < pool.intra_operators().size() ; i ++)
			if(pool.intra_operator(i).number_operator() == 2)
				((Improve&)pool.intra_operator(i)).set_sa(new SA::Solver_Seq(pbm,SAcfg));

		newGA::Solver_Wan GAsolver(pbm,GAcfg,argc,argv);
		SA::Solver_Seq SAsolver(pbm,SAcfg);

		for(int i = 0; i < GAcfg.independent_runs(); i++)
		{
 			GAsolver.run(GAcfg.nb_evolution_steps());

			if (GAsolver.pid()!=0)
			{
				for (int i=0;i<GAcfg.population_size();i++)
				{
					if (rand01() < ((double)params.number_solutions_SA()/GAcfg.population_size()))
					{
						Solution& selected_solution= *((GAsolver.population().parents())[i]);
						SAsolver.run(selected_solution,SAcfg.max_evaluations());
						selected_solution=SAsolver.current_best_solution();
						GAsolver.KeepHistory(SAsolver.current_best_solution(),SAsolver.current_best_cost(),SAsolver.current_best_cost(),GAsolver.time_spent_trial()  + SAsolver.time_best_found_trial(),GAsolver.current_time_spent() + SAsolver.time_best_found_trial());
						GAsolver.time_spent_trial(GAsolver.time_spent_trial() + SAsolver.time_spent_trial());
						GAsolver.current_time_spent(GAsolver.current_time_spent() + SAsolver.time_spent_trial());
					}
				 }

				 GAsolver.UpdateFromState();
				GAsolver.RefreshCfgState();
				 GAsolver.send_local_state_to(-1);
			}
			else
			{
				 GAsolver.end_trial(false);
				 GAsolver.check_for_refresh_global_state();
			}
		}

		if (GAsolver.pid()==0)
		{
			cout << endl << "FINAL"  << endl;
			GAsolver.show_state();
			cout << GAsolver.global_best_solution();
		 	cout << endl << "Total Spent time: " << GAsolver.current_time_spent() << endl;
		 	fres << endl << GAsolver.userstatistics() << endl << GAsolver.global_best_solution();
		}
	}

	Solver_Wan_v3::~Solver_Wan_v3()
	{
	}
}

#endif
