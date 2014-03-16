	/************************************************
***				  	      *** 
***  Particle Swarm      Skeleton v1.0        *** 
***  Provided classes and methods             ***
***  Developed by: José Manuel Garcia Nieto   *** 
***                                           *** 
************************************************/

#include "PSO.hh"

skeleton PSO
{

// StopCondition -------------------------------------------------------------------------------------

	StopCondition::StopCondition()
	{}

	StopCondition::~StopCondition()
	{}

// SetUpParams -----------------------------------------------------------

	SetUpParams::SetUpParams ()
	: _independent_runs(0),
	  _nb_evolution_steps(0),
	  _swarm_size(0),
	  _particle_size(0),
	  _neighborhood_size(0),
	  _delta_min(-1.0),
	  _delta_max(1.0),
	  _individuality_weight(2.0),
	  _ind_min_weight(0.0),
	  _ind_max_weight(1.0),
	  _sociality_weight(2.0),
	  _soc_min_weight(0.0),
	  _soc_max_weight(1.0),
	  _weight_max(0.9),
	  _weight_min(0.4),
	  _refresh_global_state(1),
	  _migration_rate(1),
	  _synchronized(0),
	  _check_asynchronous(1),
	  _display_state(0)
	{}

	

 	istream& operator>> (istream& is, SetUpParams& setup)
	{
		char buffer[MAX_BUFFER]; // current line in the setup file
		char command[50];
		long op;
		short int nb_section=0;
		short int nb_param=0;
		short int nb_w_param=0;
		short int nb_delta_param=0;
		short int nb_m_param=0;
		short int nb_LAN_param=0;

		while (is.getline(buffer,MAX_BUFFER,'\n'))
		{
			sscanf(buffer," %s ",command);
			if (!(strcmp(command,"General"))) nb_section=0;
			if (!(strcmp(command,"Binary-pso-params"))) nb_section=1;
			if (!(strcmp(command,"Weight-factors"))) nb_section=2;
			if (!(strcmp(command,"Migration-params"))) nb_section=3;
			if (!(strcmp(command,"LAN-configuration"))) nb_section=4;

			op=-1;
			sscanf(buffer," %d ",&op);
			if (op<0) continue;
			switch (nb_section)
			{
				case 0: switch (nb_param)
					{
						case 0: setup.independent_runs(op); break;
				 	 	case 1: setup.nb_evolution_steps(op); break;
						case 2: setup.swarm_size(op);break;
						case 3: setup.particle_size(op);break;
						case 4: setup.neighborhood_size(op);break;
						case 5: setup.display_state(op);break;
					}
					nb_param++;
					break;
			    case 1: // Discrete pso params
				      switch (nb_delta_param)
					  {
					  case 0: setup.delta_min(op);break;
					  case 1: setup.delta_max(op);break;
					  }
					nb_delta_param++;
					break;
			       case 2:  // Weight factors
				       switch (nb_w_param)
			                  {
					  case 0: setup.individuality_weight(op); break;
					  case 1: setup.ind_min_weight(op); break;
					  case 2: setup.ind_max_weight(op); break;
					  case 3: setup.sociality_weight(op); break;
					  case 4: setup.soc_min_weight(op); break;
					  case 5: setup.soc_max_weight(op); break;
					  case 6: setup.weight_min(op); break;
					  case 7: setup.weight_max(op);  break;
			                  }
					  nb_w_param++;
					break;
					case 3:  // Migration params
				       switch (nb_m_param)
			                  {
					  case 0: setup.migration_rate(op); break;				
			                  }
					  nb_m_param++;
					break;
				case 4: if (nb_LAN_param>=3) break;
					switch (nb_LAN_param)
					{
						case 0: setup.refresh_global_state(op); break;
						case 1: setup.synchronized(op); break;
						case 2: assert(op>0);
							setup.check_asynchronous(op); break;
					}
					nb_LAN_param++;
					break;
			}
		}

		return is;
	}

	ostream& operator<< (ostream& os, const SetUpParams& setup)
	{
		os << "CONFIGURATION -------------------------------------------" << endl << endl;
		os << "\t" << "Independent runs : " << setup.independent_runs()    << endl
		   << "\t" << "Evolution steps: "   << setup.nb_evolution_steps() << endl
		   << "\t" << "Size of swarm: " <<  setup.swarm_size() << endl
	    	   << "\t" << "Size of particle: " << setup.particle_size() << endl
		   << "\t" << "Size of neighborhood: " << setup.neighborhood_size() << endl;
	
		os << "\t" << "Display State: " << setup.display_state() << endl << endl;
	       
		os << endl << "\t" << "LAN configuration:" << endl
		   << "\t" << "----------------------" << endl << endl
		   << "\t" << "Refresh global state in number of generations: " << setup.refresh_global_state() << endl;

		if (setup.synchronized())
		  os << "\t" << "Running in synchronous mode" << endl;
		else
		  os << "\t" << "Running in asynchronous mode" << endl;
		os << "\t" << "Interval for checking asynchronous receptions: " << setup.check_asynchronous() << endl << endl;

		os << endl << endl << "END CONFIGURATION -------------------------------------------" << endl << endl;
		continue_question();
	        return os;
	}

	const unsigned int SetUpParams::independent_runs() const
	{
		return _independent_runs;
	}

	const unsigned long    SetUpParams::nb_evolution_steps() const
	{
		return _nb_evolution_steps;
	}

	const unsigned int SetUpParams::swarm_size() const
	{
		return _swarm_size;
	}

	const unsigned int SetUpParams::particle_size() const
	{
		return _particle_size;
	}

	const unsigned int SetUpParams::neighborhood_size() const
	{
		return _neighborhood_size;
	}

	const unsigned long SetUpParams::refresh_global_state() const
	{
		return _refresh_global_state;
	}
	
	const unsigned int SetUpParams::migration_rate() const
	{
		return _migration_rate;
	}

	const bool SetUpParams::synchronized() const
	{
	  return _synchronized;
	}

	const unsigned int SetUpParams::check_asynchronous() const
	{
	  return _check_asynchronous;
	}

	const bool  SetUpParams::display_state() const
	{
		return _display_state;
	}

	void SetUpParams::independent_runs(const unsigned int val)
	{
		_independent_runs=val;
	}

	void SetUpParams::nb_evolution_steps(const unsigned long val)
	{
		_nb_evolution_steps=val;
	}

	void SetUpParams::swarm_size(const unsigned int val)
	{
		_swarm_size=val;
	}

	void SetUpParams::particle_size(const unsigned int val)
	{
		_particle_size=val;
	}

	void SetUpParams::neighborhood_size(const unsigned int val)
	{
		_neighborhood_size=val;
	}

	void SetUpParams::display_state(const bool val)
	{
		_display_state=val;
	}

	void SetUpParams::refresh_global_state(const unsigned long val)
	{
		_refresh_global_state=val;
	}
	
	void SetUpParams::migration_rate(const unsigned int val)
	{
		_migration_rate=val;
	}

	void SetUpParams::synchronized(const bool val)
	{
	   _synchronized=val;
	}

	void SetUpParams::check_asynchronous(const unsigned int val)
	{
	  _check_asynchronous=val;
	}

	const double SetUpParams::delta_min() const 
	   {
	     return _delta_min;
	   }

    const double SetUpParams::delta_max() const
	   {
	     return _delta_max;
	   }

	 const double SetUpParams::individuality_weight() const
	   {
	     return _individuality_weight;
	   }
	 
	 const double SetUpParams::ind_min_weight() const
	   {
	     return _ind_min_weight;
	   }

	 const double SetUpParams::ind_max_weight() const
	   {
	     return _ind_max_weight;
	   }

	 const double SetUpParams::sociality_weight() const
	   {
	     return _sociality_weight;
	   }
             

	 const double SetUpParams::soc_min_weight() const
	   {
	     return _soc_min_weight;
	   }

	 const double SetUpParams::soc_max_weight() const
	   {
	     return _soc_max_weight;
	   }

	 const double SetUpParams::weight_min() const
	   {
	     return _weight_min;
	   }

	 
	 const double SetUpParams::weight_max() const
	   {
	     return _weight_max;
	   }
    
	 void SetUpParams::individuality_weight(const double val)
	   {
	     _individuality_weight=val;
	   }

	 void SetUpParams::ind_min_weight(const double val)
	   {
	     _ind_min_weight=val;
	   }

     void SetUpParams::ind_max_weight(const double val)
	   {
	     _ind_max_weight=val;
	   }

     void SetUpParams::sociality_weight(const double val)
	   {
	     _sociality_weight=val;
	   }
         
	 void SetUpParams::soc_min_weight(const double val)
	   {
	     _soc_min_weight=val;
	   }

	 void SetUpParams::soc_max_weight(const double val)
	   {
	     _soc_max_weight=val;
	   }

	 void SetUpParams::weight_min(const double val)
	   {
	     _weight_min=val;
	   }

	 void SetUpParams::weight_max(const double val)
	   {
	     _weight_max=val;
	   }

	 void SetUpParams::delta_min(const double val)
	   {
	     _delta_min=val;
	   }
	 
	 void SetUpParams::delta_max(const double val)
	   {
	     _delta_max=val;
	   }

	void SetUpParams::RefreshState(const StateCenter& _sc) const
        {                 
                 _sc.set_contents_state_variable("_display_state",(char *)&_display_state,1,sizeof(bool));
        }
	
	 void SetUpParams::UpdateFromState(const StateCenter& _sc) const
        {
                unsigned long nbytes,length;               
                _sc.get_contents_state_variable("_display_state",(char *)&_display_state,nbytes,length);
        }
	

        SetUpParams::~SetUpParams()
		{}

// Statistics ------------------------------------------------------

	Statistics::Statistics()
	{}

	ostream& operator<< (ostream& os, const Statistics& stats)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF CURRENT TRIAL                   " << endl;
		os << "------------------------------------------------------------------" << endl;
		for (int i=0;i< stats.stats_data.size();i++)
	     	{
			os << endl
			   << " Trial:	" << stats.stats_data[i].trial
			   << " Generation: " << stats.stats_data[i].nb_generation
			   << " Current best cost: " << stats.stats_data[i].best_cost
			   << " Global best cost: " << stats.stats_data[i].global_best_cost
			   << " Avg: " << stats.stats_data[i].average_cost
			   << " Std. Dev.: " << stats.stats_data[i].standard_deviation_cost;
		}

		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}

 	Statistics& Statistics::operator= (const Statistics& stats)
	{
		stats_data = stats.stats_data;
		return *this;
	}

	void Statistics::update(const Solver& solver)
	{
		struct stat *new_stat=(struct stat *)malloc(sizeof(struct stat));

		new_stat->trial=solver.current_trial();
		new_stat->nb_generation=solver.current_iteration();
	 	new_stat->average_cost=solver.current_average_cost();
		new_stat->standard_deviation_cost=solver.current_standard_deviation();
		new_stat->best_cost=solver.current_best_cost();
		new_stat->global_best_cost=solver.global_best_cost();

		stats_data.append(*new_stat);
	}

	void Statistics::clear()
	{
		stats_data.remove();
	}

        Statistics::~Statistics()
	{}

// Swarm ------------------------------------------------------

 	Swarm::Swarm(const Problem& pbm,const SetUpParams& setup)
	:_particles(setup.swarm_size()),
	 _setup(setup)
	{
		for (int i=0;i<_particles.size();i++){
			_particles[i]=new Solution(pbm);	
		}
	}

	Swarm& Swarm::operator= (const Swarm& swarm)
	{
		for (int i=0;i<_particles.size();i++){
			*_particles[i]=*((swarm.particles())[i]);		
		}
		return (*this);
	}

	istream& operator>> (istream& is, Swarm& swarm)
	{
		return is;
	}

	ostream& operator<< (ostream& os, const Swarm& swarm)
	{
		os << "---------------------------------------------------------------" << endl;
		os << "                           PRESENT SWARM                       " << endl << endl;
		for (int i=0;i<swarm._particles.size();i++)
			os << i <<" "<< *swarm._particles[i] << endl;
		os << endl << "---------------------------------------------------------------" << endl;
		return os;
	}

	const SetUpParams& Swarm::setup() const
	{
		return _setup;
	}

	void Swarm::initialize()
	{
		for (int i=0;i<_particles.size();i++)
			_particles[i]->initialize();
		evaluate();
	}

	void Swarm::evaluate()
	{
		double upper_fitness=(infinity() * (-1));
		double lower_fitness=(infinity());
		double current_fitness;
		double cost=0.0;

		_fitness_values=Rarray<struct particle>(_particles.size()); // array with fitness of individuals in population
		for (int i=0;i<_particles.size();i++)
		{
			_fitness_values[i].index=i;
			current_fitness = _particles[i]->fitness();
			_fitness_values[i].fitness=current_fitness;
			if (current_fitness > upper_fitness )
			{
				_upper_cost=i;
				upper_fitness = current_fitness;
			}

			if (current_fitness < lower_fitness )
			{
				_lower_cost=i;
				lower_fitness = current_fitness;
			}

			cost += current_fitness;
		}
		_average_cost =  cost / _fitness_values.size();
	}


	const Rarray<Solution*>& Swarm::particles() const
	{
		return _particles;
	}

	Rarray<struct particle>& Swarm::fitness_values()
	{
		return _fitness_values;
	}

	unsigned int Swarm::upper_cost() const
	{
	     	return _upper_cost;
	}

 	unsigned int Swarm::lower_cost() const
	{
		return _lower_cost;
	}

	double Swarm::best_cost() const
	{
		if ((*_particles[0]).pbm().direction() == minimize)
			return _fitness_values[_lower_cost].fitness;
		else
			return _fitness_values[_upper_cost].fitness;
	}

	double Swarm::worst_cost() const
	{
		if ((*_particles[0]).pbm().direction() == minimize)
			return _fitness_values[_upper_cost].fitness;
		else
			return _fitness_values[_lower_cost].fitness;
	}

	Solution& Swarm::best_solution() const
	{
		if ((*_particles[0]).pbm().direction() == minimize)
			return *_particles[_lower_cost];
		else
			return *_particles[_upper_cost];
	}

	Solution& Swarm::worst_solution() const
	{
		if ((*_particles[0]).pbm().direction() == minimize)
			return *_particles[_upper_cost];
		else
			return *_particles[_lower_cost];
	}

	Solution& Swarm::solution(const unsigned int index) const
	{
		return *_particles[index];
	}


	/* funcion de proximidad : si numero de genes iguales es mayor o igual que el tamaño */
	/* de vecindario*/
	bool Swarm::neighbor_solution(unsigned int index, unsigned int ns) const
	  {
	    return true;
	  }

	/**
	 * @brief manhattan distance between to particles
	 * @param particle 1 and particle 2
	 * @return the distance: number of differente values of the current state
	 */
	int Swarm::distance(int index, int in) const {
	  int distance = 0;
	  for (int i = 0;i<(*_particles[0]).pbm().dimension();i++){
	    distance = distance + abs((*_particles[index]).current(i) - (* _particles[in]).current(i));
	  }
	  return distance;
	}


	Solution& Swarm::neighbor_with_best_fitness(const unsigned int index) const
	  {
	    double best_neighbor_fitness;
	    unsigned int best_neighbor_position=0;
	    if ((*_particles[0]).pbm().direction() == minimize)
	      best_neighbor_fitness=infinity();
	    else
	      best_neighbor_fitness=(infinity() * (-1));
	    for (int i=0;i<_setup.swarm_size();i++){
	      if (i!=index){
			if (distance(index,i)<=_setup.neighborhood_size()){
			  if ((*_particles[0]).pbm().direction() == minimize)
			    {
			      if (_fitness_values[i].fitness<best_neighbor_fitness){
				best_neighbor_fitness=_fitness_values[i].fitness;
				best_neighbor_position=_fitness_values[i].index;
			      }
			    }
			  else
			    {
			      if (_fitness_values[i].fitness>=best_neighbor_fitness){
				best_neighbor_fitness=_fitness_values[i].fitness;
				best_neighbor_position=_fitness_values[i].index;
			      }
			    }
			}
	      }
	    }
	    return *_particles[best_neighbor_position];
	  }

	double Swarm::fitness(const unsigned int index) const
	{
		return _fitness_values[index].fitness;
	}

	double Swarm::average_cost() const
	{
		return _average_cost;
	}

	double Swarm::standard_deviation() const
	{
		double standard=0.0;
		for (int i=0;i<_fitness_values.size();i++)
			standard += pow ((_fitness_values[i].fitness - _average_cost),2);
		standard=sqrt(standard / (_fitness_values.size()-1));
		return standard;
	}
	
	
	void Swarm::local_search(Solution& sol)
	{
		Solution sol1 = sol;
		Solution sol2 = sol;
		int pos; 
		for (int i=0;i<sol.pbm().dimension();i++){			
			if (sol1.current(i)==0)
				sol1.current(i,1);
			else
				sol1.current(i,0);
			if (sol1.fitness()<sol2.fitness())
				sol2=sol1;	
			sol1=sol;		
		}
		sol=sol2;
		
	}

	/**
	 * @brief constrict the value of data between [0,1]
	 * @return int [0,1]
	 */
	double Swarm::constrict(double v){
	  if (v>setup().delta_max())
	    return setup().delta_max();
	  if (v<setup().delta_min())
	    return setup().delta_min();
	  else
	    return v;
	}

	/**
	 * @brief the sigmoid function to normatize values
	 * @param double The value to normalize
	 * @return double
	 */
	double Swarm::sig(double v){
	  return (1.0/(1.0+exp(-v)));
	}

	double Swarm::weight(int iter,int miter){
  		return setup().weight_max() - ((setup().weight_max()-setup().weight_min()/(double)miter)*(double)iter);
	}

	void Swarm::evolution(int iter){
		
		double iFactor=0.0;
		double sFactor=0.0;
		double delta=0.0;
		// gets current interesting values in the current swarm
		/* here there is the core of the pso algorithm */
		
		for (unsigned int i=0;i<setup().swarm_size();i++){
		  //for (unsigned int j=0;j<params.particle_size();j++){
		  //  _swarm.solution(i).current(j,_swarm.solution(i).next(j));
		  //}/*end for j*/

		  if (solution(i).fitness()<best_cost()){
		    solution(i).best_fitness(solution(i).fitness());
		    for (unsigned int k=0;k<setup().particle_size();k++){
		      solution(i).best(k,solution(i).current(k));		      
		    }
		  }/* end if*/
		  /* compu global statistics of the swarm */
		  //evaluate();
		
		//}/* end for i*/

		/* the pso algorithm */
		//for (unsigned int i=0;i<setup().swarm_size();i++){
		  for (unsigned int j=0;j<setup().particle_size();j++){
		    iFactor = setup().individuality_weight()*(double)rand_int(setup().ind_min_weight(),setup().ind_max_weight());
		    sFactor = setup().sociality_weight()*(double)rand_int(setup().soc_min_weight(),setup().soc_max_weight());
		    delta = (iFactor*(double)(solution(i).best(j) 
					      - solution(i).current(j))
			     +(sFactor*(double)(neighbor_with_best_fitness(i).best(j) 
						- solution(i).current(j))));
		    delta=weight(iter,setup().nb_evolution_steps())*(double)solution(i).velocity(j)+delta;
		    solution(i).velocity(j,(int)constrict(delta));
		    
		    //x<-x+v
		    delta=(double)solution(i).current(j)+solution(i).velocity(j);
		    //x<-((4+x) mod2)
		    solution(i).current(j,(int)(4.0+delta)%2);
		    //v<-(3+v) mod3 -1
		    solution(i).velocity(j,((int)(3.0+(double)solution(i).velocity(j))%3-1));
			
		  }/* end for j*/
		  /* two levels local search */
		  //local_search(solution(i));
		  //local_search(solution(i));
		  //local_search(solution(i));
		}/* end for i*/		
		evaluate();
		
		
	}

	void Swarm::interchange(const unsigned long current_generation, const Migration& migration ,NetStream& channel)
        {
                // apply migration                
                migration.execute((*this),current_generation,channel,_setup.synchronized(),_setup.check_asynchronous());
        }
	


        Swarm::~Swarm()
	{
	}

// Migration --------------------------------------------------------------------------

		Migration::Migration(const Direction dir, const int mig_rate)
			:direction(dir),
			migration_rate(mig_rate)
        		{}
        
        void Migration::execute(Swarm& swarm,const unsigned long current_generation,NetStream& _netstream,const bool synchronized,const unsigned int check_asynchronous) const
        {

                Solution solution_to_send(swarm.best_solution());
                Solution solution_received(swarm.best_solution());
                Solution solution_to_remplace(swarm.best_solution());
                bool need_to_revaluate=false;
                int mypid;
                int migration_size = 1;

                int nb_proc=_netstream.pnumber();       // Get the number of processes running

                mypid=_netstream.my_pid();

                int to   = (mypid + 1) % nb_proc;                    // Source (from) and Target (to) of processes
                int from = (nb_proc + mypid - 1) % nb_proc;

                // process number 0 is only to store the global state
                if (to==0) to=1;
                if (from==0) from=nb_proc - 1;

                _netstream << set_target(to)  << set_source(from)
                           << get_target(&to) << get_source(&from);

                if ( (current_generation % migration_rate) == 0
                  && (current_generation!=swarm.setup().nb_evolution_steps())) // in this generation this operator have to be applied
        		 {                        
                        for (int i=0;i<migration_size;i++)
                        {
                                // select individual to send
                                solution_to_send = swarm.best_solution();                                
                                _netstream << solution_to_send;
                        }

                        if (synchronized)    // synchronous mode: blocked until data are received
                        {                                
                                for (int i=0;i<migration_size;i++)
                                {
                                        // select individual to be remplaced
                                        solution_to_remplace = swarm.worst_solution();

                                        //solution_received=new Solution(solution_to_remplace.pbm());
                                        _netstream << wait(regular);
                                        _netstream >> solution_received;

                                        // remplace policy
                                        if ((solution_received.fitness()<=solution_to_remplace.fitness() && direction==minimize)
                                         || (solution_received.fitness()>=solution_to_remplace.fitness() && direction==maximize))
                                        {
                                                need_to_revaluate=true;
                                                solution_to_remplace=solution_received;
                                        }
                                        //delete(solution_received);
                                }
                        }
                } // end if
        		if (!synchronized && ((current_generation % check_asynchronous) ==0))
                { // asynchronous mode: if there are not data, continue;
                  // but, if there are data, i have to receive it
                        int pending=false;
                        _netstream._probe(regular,pending);
                        if (pending)
                        {                               
                                for (int i=0;i<migration_size;i++)
                                {
                                        pending=false;
                                        _netstream._probe(regular,pending);
                                        if (!pending) break;

                                        // select individual to be remplaced
                                        solution_to_remplace = swarm.worst_solution();                     
                                        //solution_received=new Solution(solution_to_remplace.pbm());
                                        _netstream >> solution_received;

                                        // remplace policy
                                        if ((solution_received.fitness()<=solution_to_remplace.fitness() && direction==minimize)
                                         || (solution_received.fitness()>=solution_to_remplace.fitness() && direction==maximize))
                                        {
                                                need_to_revaluate=true;
                                                solution_to_remplace=solution_received;
                                        }
                                        //delete(solution_received);
                                } // end for
                        } // end if
                }
        		  if (need_to_revaluate) swarm.evaluate();
        }/*end lf execute*/
        		
        
        
      	ostream& operator<< (ostream& os, const Migration& migration)
        {
                os << "Migration."
                   << endl << "\t" <<  " Rate: " << migration.migration_rate << endl;
                return os;
        }

        Migration::~Migration()
        {}
      	



// Solver (superclasse)---------------------------------------------------

	Solver::Solver (const Problem& pbm, const SetUpParams& setup)
	: problem(pbm),
	  params(setup),
	  _swarm(pbm,setup),
	  _stat(),
	  _userstat(),
	  _sc(),
	  _best_cost((-1) * pbm.direction() * infinity()),
	  _worst_cost((-1) * _best_cost),
	  _best_solution(pbm),
	  _average_cost(0.0),
	  _standard_deviation(0.0),
	  _time_spent_in_trial(0.0),
	  _total_time_spent(0.0),
	  _start_trial(0.0),
	  _start_global(0.0),
	  _current_trial("_current_trial",_sc),
 	  _current_iteration("_current_iteration",_sc),
	  _current_best_solution("_current_best_solution",_sc),
	  _current_best_cost("_current_best_cost",_sc),
	  _current_worst_cost("_current_worst_cost",_sc),
	  _current_average_cost("_current_average_cost",_sc),
	  _current_standard_deviation("_current_standard_deviation",_sc),
	  _current_time_spent("_current_time_spent",_sc),
	  _best_solution_trial("_best_sol_trial",_sc),
	  _best_cost_trial("_best_cost_trial",_sc),
	  _worst_cost_trial("_worst_cost_trial",_sc),
	  _iteration_best_found_in_trial("_iteration_best_found_in_trial",_sc),
	  _time_best_found_trial("_time_best_found_trial",_sc),
	  _time_spent_trial("_time_spent_trial",_sc),
	  _trial_best_found("_trial_best_found",_sc),
	  _iteration_best_found("_iteration_best_found",_sc),
	  _global_best_solution("_global_best_solution",_sc),
	  _global_best_cost("_global_best_cost",_sc),
	  _global_worst_cost("_global_worst_cost",_sc),
	  _time_best_found("_time_best_found",_sc),
	  _delta_min("_delta_min",_sc),
	  _delta_max("_delta_max",_sc),
	  _individuality_weight("_individuality_weight",_sc),
	  _ind_min_weight("_ind_min_weight",_sc),
	  _ind_max_weight("_ind_max_weight",_sc),
	  _sociality_weight("_sociality_weight",_sc),
	  _soc_min_weight("_soc_min_weight",_sc),
	  _soc_max_weight("_soc_max_weight",_sc),
	  _weight_max("_weight_max",_sc),
	  _weight_min("_weight_min",_sc),
	  _display_state("_display_state",_sc)
	{
		current_trial(0);
	 	current_iteration(0);
		current_best_solution(_best_solution);
		current_best_cost(_best_cost);
		current_worst_cost(_worst_cost);
		current_average_cost(_average_cost);
		current_standard_deviation(_standard_deviation);
		current_time_spent(_total_time_spent);
		best_solution_trial(_best_solution);
		best_cost_trial(_best_cost);
		worst_cost_trial(_worst_cost);
		iteration_best_found_in_trial(0);
		time_best_found_trial(_time_spent_in_trial);
		time_spent_trial(_time_spent_in_trial);
		trial_best_found(0);
		iteration_best_found(0);
		global_best_solution(_best_solution);
		global_best_cost(_best_cost);
		global_worst_cost(_worst_cost);
		time_best_found(_total_time_spent);
  
		individuality_weight(2.0);
        ind_min_weight(0.0);
        ind_max_weight(0.1);
        sociality_weight(2.0);
        soc_min_weight(0.0);
        soc_max_weight(0.1);
        weight_min(0.4);
        weight_max(0.9);
        delta_min(-1.0);
        delta_max(1.0);

		display_state(setup.display_state());
	}

	int Solver::pid() const
	{
		return 0;
	}

	bool Solver::end_trial() const
	{
		return _end_trial;
	}

	void Solver::end_trial(bool et)
	{
		_end_trial = et;
	}

	unsigned int Solver::current_trial() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_current_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	unsigned long Solver::current_iteration() const
	{
		unsigned long value=0;
		unsigned long nitems,length;
		_current_iteration.get_contents((char *)&value, nitems, length);
		return value;
	}

	Solution Solver::current_best_solution() const
	{
		Solution sol(problem);
		unsigned long nitems,length;
		char data_stored[_current_best_solution.get_nitems() + _current_best_solution.get_length()];
		_current_best_solution.get_contents(data_stored, nitems, length);
		sol.to_Solution(data_stored);
		return sol;
	}

	double Solver::current_best_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_current_best_cost.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::current_worst_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_current_worst_cost.get_contents((char *)&value, nitems, length);
		return value;
	} 

      	double Solver::current_average_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_current_average_cost.get_contents((char *)&value, nitems, length);
		return value;
	}		

	double Solver::current_standard_deviation() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_current_standard_deviation.get_contents((char *)&value, nitems, length);
		return value;
	}

	float  Solver::current_time_spent() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_current_time_spent.get_contents((char *)&value, nitems, length);
		return value;
	}		

 	Solution  Solver::best_solution_trial() const
	{
		Solution sol(problem);
		char data_stored[_best_solution_trial.get_nitems() + _best_solution_trial.get_length()];
		unsigned long nitems,length;
		_best_solution_trial.get_contents(data_stored, nitems, length);
		sol.to_Solution(data_stored);
		return sol;
	}

	double Solver::best_cost_trial() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_best_cost_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::worst_cost_trial() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_worst_cost_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	unsigned int Solver::iteration_best_found_in_trial() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_iteration_best_found_in_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	float Solver::time_best_found_trial() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_best_found_trial.get_contents((char *)&value, nitems, length);
		return value;
	}

	float Solver::time_spent_trial() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_spent_trial.get_contents((char *)&value, nitems, length);
		return value;
	}	

	unsigned int Solver::trial_best_found() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_trial_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	unsigned int Solver::iteration_best_found() const
	{
		unsigned int value=0;
		unsigned long nitems,length;
		_iteration_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	Solution Solver::global_best_solution() const
	{
		Solution sol(problem);
		char data_stored[_global_best_solution.get_nitems() + _global_best_solution.get_length()];
		unsigned long nitems,length;
		_global_best_solution.get_contents(data_stored, nitems, length);
		sol.to_Solution(data_stored);
		return sol;
	}

	double Solver::global_best_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_global_best_cost.get_contents((char *)&value, nitems, length);
		return value;
	}

	double Solver::global_worst_cost() const
	{
		double value=0.0;
		unsigned long nitems,length;
		_global_worst_cost.get_contents((char *)&value, nitems, length);
		return value;
	}

	float Solver::time_best_found() const
	{
		float value=0.0;
		unsigned long nitems,length;
		_time_best_found.get_contents((char *)&value, nitems, length);
		return value;
	}

	int Solver::display_state() const
	{
		int value=0;
		unsigned long nitems,length;
		_display_state.get_contents((char *)&value, nitems, length);
		return value;
	}

	
	 const double Solver::delta_min() const
	   {
	     double value=0.0;
		unsigned long nitems,length;
		_delta_min.get_contents((char *)&value, nitems, length);
		return value;
	   }
                
	 const double Solver::delta_max() const
	   {
	     double value=0.0;
		unsigned long nitems,length;
		_delta_max.get_contents((char *)&value, nitems, length);
		return value;
	   }

    const double Solver::individuality_weight() const
	   {
	     double value=0.0;
		unsigned long nitems,length;
		_individuality_weight.get_contents((char *)&value, nitems, length);
		return value;
	   }

    const double Solver::ind_min_weight() const
	    {
	    double value=0.0;
		unsigned long nitems,length;
		_ind_min_weight.get_contents((char *)&value, nitems, length);
		return value;
	    }

	  const double Solver::ind_max_weight() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_ind_max_weight.get_contents((char *)&value, nitems, length);
		return value;
	    }
                
	  const double Solver::sociality_weight() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_sociality_weight.get_contents((char *)&value, nitems, length);
		return value;
	    }
                
	  const double Solver::soc_min_weight() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_soc_min_weight.get_contents((char *)&value, nitems, length);
		return value;
	    }
                
	  const double Solver::soc_max_weight() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_soc_max_weight.get_contents((char *)&value, nitems, length);
		return value;
	    }
                
	  const double Solver::weight_min() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_weight_min.get_contents((char *)&value, nitems, length);
		return value;
	    }
                
	  const double Solver::weight_max() const
	    {
	      double value=0.0;
		unsigned long nitems,length;
		_weight_max.get_contents((char *)&value, nitems, length);
		return value;
	    }


	void Solver::current_trial(const unsigned int value)
	{
		_current_trial.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::current_iteration(const unsigned long value)
	{
 		_current_iteration.set_contents((char *)&value,1,sizeof(long));
	}

	void Solver::current_best_solution(const Solution& sol)
	{
                _current_best_solution.set_contents(sol.to_String(),1,sol.size());
	}

	void Solver::current_best_cost(const double value)
	{
		_current_best_cost.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::current_worst_cost(const double value)
	{
		_current_worst_cost.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::current_average_cost(const double value)
	{
		_current_average_cost.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::current_standard_deviation(const double value)
	{
		_current_standard_deviation.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::current_time_spent(const float value)
	{
		_current_time_spent.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::best_solution_trial(const Solution& sol)
	{
		_best_solution_trial.set_contents(sol.to_String(),1,sol.size());
	}

	void Solver::best_cost_trial(const double value)
	{
		_best_cost_trial.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::worst_cost_trial(const double value)
	{
		_worst_cost_trial.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::iteration_best_found_in_trial(const unsigned int value)
	{
		_iteration_best_found_in_trial.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::time_best_found_trial(const float value)
	{
		_time_best_found_trial.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::time_spent_trial(const float value)
	{
		_time_spent_trial.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::trial_best_found(const unsigned int value)
	{
		_trial_best_found.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::iteration_best_found(const unsigned int  value)
	{
		_iteration_best_found.set_contents((char *)&value,1,sizeof(int));
	}

	void Solver::global_best_solution(const Solution& sol)
	{
		_global_best_solution.set_contents(sol.to_String(),1,sol.size());
	}

	void Solver::global_best_cost(const double value)
	{
		_global_best_cost.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::global_worst_cost(const double value)
	{
		_global_worst_cost.set_contents((char *)&value,1,sizeof(double));
	}

	void Solver::time_best_found(const float value)
	{
		_time_best_found.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::display_state(const int value)
	{
		_display_state.set_contents((char *)&value,1,sizeof(float));
	}

	void Solver::individuality_weight(const double value)
	  {
	    _individuality_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::ind_min_weight(const double value)
	  {
	    _ind_min_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::ind_max_weight(const double value)
	  {
	    _ind_max_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::sociality_weight(const double value)
	  {
	    _sociality_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::soc_min_weight(const double value)
	  {
	    _soc_min_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::soc_max_weight(const double value)
	  {
	    _soc_max_weight.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::weight_min(const double value)
	  {
	    _weight_min.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::weight_max(const double value)
	  {
	    _weight_min.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::delta_min(const double value)
	  {
	    _delta_min.set_contents((char *)&value,1,sizeof(double));
	  }
                
	void Solver::delta_max(const double value)
	  {
	    _delta_max.set_contents((char *)&value,1,sizeof(double));
	  }

	Statistics& Solver::statistics()
	{
		return _stat;
	}

	UserStatistics& Solver::userstatistics()
	{
			return _userstat;
	}

	Swarm& Solver::swarm() 
	{
			return _swarm;
	}
	
	const SetUpParams& Solver::setup() const
	{
		return params;
	}

	const Problem& Solver::pbm() const
	{
		return problem;
	}

	void Solver::KeepHistory(const Solution& best_sol,const double _best_cost,const double _worst_cost,const float _time_spent_in_trial,const float _total_time_spent)
	{
		bool betterG=false;
		bool worseG=false;
		bool betterT=false;
		bool worseT=false;

	    	switch (problem.direction())
		{
			case minimize: betterG = (_best_cost < global_best_cost() || (_best_cost == global_best_cost() && _time_spent_in_trial < time_best_found()));
				       worseG  = (_worst_cost > global_worst_cost());
				       betterT = (_best_cost < best_cost_trial() || (_best_cost == best_cost_trial() && _time_spent_in_trial < time_best_found_trial()));
				       worseT  = (_worst_cost > worst_cost_trial());
				       break;
			case maximize: betterG = (_best_cost > global_best_cost() || (_best_cost == global_best_cost() && _time_spent_in_trial < time_best_found()));
				       worseG  = (_worst_cost < global_worst_cost());
				       betterT = (_best_cost > best_cost_trial() || (_best_cost == best_cost_trial() && _time_spent_in_trial < time_best_found_trial()));
				       worseT  = (_worst_cost < worst_cost_trial());
				       break;
		}

		if (betterT)
		{
			best_solution_trial(best_sol);
			best_cost_trial(_best_cost);
			time_best_found_trial(_time_spent_in_trial);
			iteration_best_found_in_trial(current_iteration());
			if (betterG)
			{
				global_best_solution(best_sol);
				global_best_cost(_best_cost);
				time_best_found(_time_spent_in_trial);
				trial_best_found(current_trial());
				iteration_best_found(current_iteration());
			}
		}

		if (worseT)
		{
			worst_cost_trial(_worst_cost);
 		  	if (worseG)
				global_worst_cost(_worst_cost);
		}
	}

	StateCenter *Solver::GetState()
	{
		return &_sc;
	}

	void Solver::RefreshState()
	{
	    current_best_solution(_best_solution);
		current_best_cost(_best_cost);
		current_worst_cost(_worst_cost);
		current_average_cost(_average_cost);
		current_standard_deviation(_standard_deviation);
		current_time_spent(_total_time_spent);
		time_spent_trial(_time_spent_in_trial);
		KeepHistory(_best_solution,_best_cost,_worst_cost,_time_spent_in_trial,_total_time_spent);
	}

	void Solver::RefreshCfgState()
	{
		params.RefreshState(_sc);
	}

	void Solver::UpdateFromState()
	{
		_best_solution=current_best_solution();
		_best_cost=current_best_cost();
		_worst_cost=current_worst_cost();
		_average_cost=current_average_cost();
		_standard_deviation=current_standard_deviation();
		_total_time_spent=current_time_spent();
		_time_spent_in_trial=time_spent_trial();
		KeepHistory(_best_solution,_best_cost,_worst_cost,_time_spent_in_trial,_total_time_spent);
	}

	void Solver::UpdateFromCfgState()
	{
		params.UpdateFromState(_sc);
	}

	void Solver::show_state() const
	{
		cout << endl << " Current State ---------------------------------------------" << endl;
		cout << endl << "Current trial: " << current_trial();
		cout << endl << "Current iteration: " << current_iteration();
		cout << endl << "Current best cost: " << current_best_cost();
		cout << endl << "Current worst cost: " << current_worst_cost();
		cout << endl << "Current Average cost: " << current_average_cost();
		cout << endl << "Current Standard Deviation: " << current_standard_deviation();
		cout << endl << endl <<  "Trial: ";
		cout << endl << "Best cost trial: " << best_cost_trial();
		cout << endl << "Worst cost trial: " << worst_cost_trial();
		cout << endl << "Iteration best found in trial: " << iteration_best_found_in_trial();
		cout << endl << "Time best found trial: " << time_best_found_trial();
		cout << endl << "Time spent in trial: " << time_spent_trial();
		cout << endl << endl << "Global: ";
		cout << endl << "Global best cost: " << global_best_cost();
		cout << endl << "Global worst cost: " << global_worst_cost();
		cout << endl << "Trial best found: " << trial_best_found();
		cout << endl << "Iteration best found: " << iteration_best_found();
		cout << endl << "Time best found: " << time_best_found();
		cout << endl << endl << "Best Solution: " << endl << global_best_solution();
		cout << endl << endl << "Current time spent (so far): " << current_time_spent() << endl;
	}

	Solver::~Solver()
	{
		_sc.removeAll();
        }

// Solver sequencial -----------------------------------------------------

	Solver_Seq::Solver_Seq (const Problem& pbm, const SetUpParams& setup)
	: Solver(pbm,setup)
	{
		random_seed(time(0));
		_end_trial=true;
	}

	Solver_Seq::~Solver_Seq ()
	{}

	void Solver_Seq::StartUp()
	{
		Swarm swarm(problem,params);			
		swarm.initialize();		
		StartUp(swarm);
		
	}

	void Solver_Seq::StartUp(const Swarm& s)
	{
		_start_trial=_used_time();
		_start_global=_total_time_spent;

		current_trial(current_trial()+1);
		current_iteration(0);
		// initialize state variables in the current trial		
		Solution initial_solution(problem);						
		_time_spent_in_trial=0.0;
		best_cost_trial((-1) * problem.direction() * infinity());
		worst_cost_trial((-1) * best_cost_trial());
				
		best_solution_trial(initial_solution);
		time_best_found_trial(0.0);	

		_swarm=s;	
		_swarm.evaluate();	
		// gets current interesting values in the current swarm
		_best_cost=_swarm.best_cost();
		_best_solution=_swarm.best_solution();
		_worst_cost=_swarm.worst_cost();
		_average_cost=_swarm.average_cost();
		_standard_deviation=_swarm.standard_deviation();
		// refresh state with these values
		RefreshState();
		RefreshCfgState();		
		_stat.update(*this);
		_userstat.update(*this);
		if (display_state())
			show_state();
	}


	


	void Solver_Seq::DoStep()
	{
		current_iteration(current_iteration()+1);
			
		/*Does an step of evolution*/
		_swarm.evolution(current_iteration());

		/* gets the new statistics */
		_best_cost=_swarm.best_cost();
		_best_solution=_swarm.best_solution();
		_worst_cost=_swarm.worst_cost();
		_average_cost=_swarm.average_cost();
		_standard_deviation = _swarm.standard_deviation();

		_time_spent_in_trial = _used_time(_start_trial);
		_total_time_spent    = _start_global + _time_spent_in_trial;

		// refresh state with these values
		RefreshState();
		RefreshCfgState();

		if( (current_iteration() % params.refresh_global_state()) == 0)
			UpdateFromCfgState();


		_stat.update(*this);
		_userstat.update(*this);

		if (display_state())
			show_state();
	}

	void Solver_Seq::run ()
	{
		while (current_trial() < params.independent_runs())// && !(terminateQ(problem,*this,params)))
			run(params.nb_evolution_steps());
	}

	void Solver_Seq::run (const unsigned long int nb_generations)
	{
		//cout << " D " << _swarm << endl;
		StartUp();
		while ((current_iteration() < nb_generations) ) //&& !(terminateQ(problem,*this,params)))
			DoStep();
		
	}

	void Solver_Seq::run (const Swarm& s,const unsigned long int nb_generations)
	{
		StartUp(s);
		while ((current_iteration() < nb_generations) )//&&  !(terminateQ(problem,*this,params)))
			DoStep();

	}

	// Solver LAN ------------------------------------------------------------
	  
        Solver_Lan::Solver_Lan (const Problem& pbm, const SetUpParams& setup, const Migration& m, int argc,char **argv):
                    _best_solution_trial(pbm),
                    Solver(pbm,setup),_migration(m),_netstream()
        {

                NetStream::init(argc,argv);
                mypid=_netstream.my_pid();
                random_seed(time(0) + (mypid+1));
                //  random_seed(time(0) + (mypid+1));
        }

        Solver_Lan::~Solver_Lan ()
        {
                NetStream::finalize();
        }

        int Solver_Lan::pid() const
        {
                return mypid;
        }

        NetStream& Solver_Lan::netstream()
        {
                return _netstream;
        }

        void Solver_Lan::StartUp()
        {
                Swarm swarm(problem,params);
                swarm.initialize();
                StartUp(swarm);
        }
	
	void Solver_Lan::StartUp(const Swarm& swarm)
        {
                _netstream << barrier;

                _start_trial=_used_time();
                _start_global=_total_time_spent;

                _end_trial=false;

                current_trial(current_trial()+1);
                current_iteration(0);

                // initialize state variables in the current trial

                Solution initial_solution(problem);

                _time_spent_in_trial=0.0;
                best_cost_trial((-1) * problem.direction() * infinity());
                worst_cost_trial((-1) * best_cost_trial());
                best_solution_trial(initial_solution);
                iteration_best_found_in_trial(0);
                time_best_found_trial(0.0);
                time_spent_trial(0.0);

                if (mypid!=0)
                {
                        _swarm=swarm;
                        _swarm.evaluate();

                        // gets current interesting values in the current population

                        _best_cost=_swarm.best_cost();
                        _best_solution=_swarm.best_solution();
                        _worst_cost=_swarm.worst_cost();
                        _average_cost=_swarm.average_cost();
                        _standard_deviation=_swarm.standard_deviation();
                       
                       	 // refresh state with these values
                        RefreshState();
                        RefreshCfgState();

                        send_local_state_to(mypid);

                        _stat.update(*this);
                        _userstat.update(*this);

                }
        }

        void Solver_Lan::DoStep()
        {
                current_iteration(current_iteration()+1);
                _swarm.evolution(current_iteration());
                _swarm.interchange(current_iteration(),_migration,_netstream);

                // gets current interesting values in the current population

                _best_cost=_swarm.best_cost();
                _best_solution=_swarm.best_solution();
                _worst_cost=_swarm.worst_cost();
                _average_cost=_swarm.average_cost();
                _standard_deviation=_swarm.standard_deviation();

                _time_spent_in_trial = _used_time(_start_trial);
                _total_time_spent    = _start_global + _time_spent_in_trial;

                // refresh state with these values
                RefreshState();
                RefreshCfgState();
                 
                // in this iteration i have to send data about my local state to the global state
                if ((int)current_iteration() % params.refresh_global_state() ==0)
                {
                        send_local_state_to(mypid);
                        UpdateFromCfgState();
                }

                _stat.update(*this);
                _userstat.update(*this);

                // if (display_state()) show_state();
        }


        void Solver_Lan::send_local_state_to(int _mypid)
        {
                _netstream << set_target(0);
                _netstream << pack_begin
                           << _mypid
                           << current_trial()
                           << current_iteration()
                           << best_cost_trial()
                           << best_solution_trial()
                           << iteration_best_found_in_trial()
                           << time_best_found_trial()
                           << worst_cost_trial()
                           << current_best_cost()
                           << current_best_solution()
                           << current_worst_cost()
                           << current_average_cost()
                           << current_standard_deviation()
                           << pack_end;
        }
                
          int Solver_Lan::receive_local_state()
        {
                int r_pid=0;

                _netstream._wait(packed);

                _netstream << pack_begin
                           >> r_pid
                           >> _current_trial
                           >> _current_iteration
                           >> _best_cost_trial
                           >> _best_solution_trial
                           >> _iteration_best_found_in_trial
                           >> _time_best_found_in_trial
                           >> _worst_cost_trial
                           >> _best_cost
                           >> _best_solution
                           >> _worst_cost
                           >> _average_cost
                           >> _standard_deviation
                           << pack_end;
                return r_pid;
        }

        void Solver_Lan::check_for_refresh_global_state() // Executed in process with pid 0
        {
                unsigned int nb_finalized_processes=0;
                int received_pid;
                int nb_proc=_netstream.pnumber();

                _netstream << set_source(MPI_ANY_SOURCE);

                while (!_end_trial)
                {
                        received_pid=0;
                        received_pid=receive_local_state();

                        current_trial(_current_trial);

                        // the process that has send data has finished the current trial
                        if (received_pid==-1) nb_finalized_processes++;
                        if (nb_finalized_processes==nb_proc-1) _end_trial=true;

                        // refresh the global state with received data ( a local state )
                        current_iteration(_iteration_best_found_in_trial);
                        KeepHistory(_best_solution_trial,_best_cost_trial,_worst_cost_trial,_time_best_found_in_trial,_start_global + _time_best_found_in_trial);
                        current_iteration(_current_iteration);

                        _time_spent_in_trial = _used_time(_start_trial);
                        _total_time_spent    = _start_global + _time_spent_in_trial;
                        RefreshState();
                        RefreshCfgState();

                        _stat.update(*this);
                        _userstat.update(*this);

                        // display current global state
                        if (display_state()) show_state();
                } // end while

                // display the global state at the end of the current trial
                if (display_state()) show_state();
        }
               	

		 void Solver_Lan::run ()
        {
                while (current_trial() < params.independent_runs())// && !(terminateQ(problem,*this,params)))
                        run(params.nb_evolution_steps());
        }

        void Solver_Lan::run (const unsigned long int nb_generations)
        {
                StartUp();
                if (mypid!=0)
                {
                        while ((current_iteration() < nb_generations))// && !(terminateQ(problem,*this,params)))
                                DoStep();
                        send_local_state_to(-1);
                }
                else
                {
                        check_for_refresh_global_state();
                }

                _netstream << barrier;
                reset();
        }
		
		
		void Solver_Lan::run (const Swarm& swarm,const unsigned long int nb_generations)
        {
                StartUp(swarm);
                if (mypid!=0)
                {
                        while ((current_iteration() < nb_generations))// && !(terminateQ(problem,*this,params)))
                                DoStep();
                        send_local_state_to(-1);
                }
                else
                {
                        check_for_refresh_global_state();
                }

                _netstream << barrier;
                reset();
        }

        void Solver_Lan::reset()
        {
                Solution left_solution(problem);
                if (mypid!=0)
                {
                        int pending=false;
                        do
                        {
                                pending=false;
                                _netstream._probe(any,pending);
                                if (pending) _netstream >> left_solution;
                         } while (pending);
                }
                _netstream << barrier;
        }
		

// Solver Wan


		Solver_Wan::Solver_Wan (const Problem& pbm, const SetUpParams& setup,const Migration& m, int argc,char **argv):
                    _best_solution_trial(pbm),
                    Solver(pbm,setup),_migration(m),_netstream()
        {

                NetStream::init(argc,argv);
                mypid=_netstream.my_pid();
                random_seed(time(0) + (mypid+1));
                //  random_seed(time(0) + (mypid+1));
        }

        Solver_Wan::~Solver_Wan ()
        {
                NetStream::finalize();
        }

        int Solver_Wan::pid() const
        {
                return mypid;
        }

        NetStream& Solver_Wan::netstream()
        {
                return _netstream;
        }

        void Solver_Wan::StartUp()
        {
                Swarm swarm(problem,params);
                swarm.initialize();
                StartUp(swarm);
        }
		
		void Solver_Wan::StartUp(const Swarm& swarm)
        {
                _netstream << barrier;

                _start_trial=_used_time();
                _start_global=_total_time_spent;

                _end_trial=false;

                current_trial(current_trial()+1);
                current_iteration(0);

                // initialize state variables in the current trial

                Solution initial_solution(problem);

                _time_spent_in_trial=0.0;
                best_cost_trial((-1) * problem.direction() * infinity());
                worst_cost_trial((-1) * best_cost_trial());
                best_solution_trial(initial_solution);
                iteration_best_found_in_trial(0);
                time_best_found_trial(0.0);
                time_spent_trial(0.0);

                if (mypid!=0)
                {
                        _swarm=swarm;
                        _swarm.evaluate();

                        // gets current interesting values in the current population

                        _best_cost=_swarm.best_cost();
                        _best_solution=_swarm.best_solution();
                        _worst_cost=_swarm.worst_cost();
                        _average_cost=_swarm.average_cost();
                        _standard_deviation=_swarm.standard_deviation();
						
						// refresh state with these values
                        RefreshState();
                        RefreshCfgState();

                        send_local_state_to(mypid);

                        UpdateFromCfgState();
                        _stat.update(*this);
                        _userstat.update(*this);

                }
        }

		 void Solver_Wan::DoStep()
        {
                current_iteration(current_iteration()+1);
                _swarm.evolution(current_iteration());
                _swarm.interchange(current_iteration(),_migration,_netstream);

                // gets current interesting values in the current population

                _best_cost=_swarm.best_cost();
                _best_solution=_swarm.best_solution();
                _worst_cost=_swarm.worst_cost();
                _average_cost=_swarm.average_cost();
                _standard_deviation=_swarm.standard_deviation();

                _time_spent_in_trial = _used_time(_start_trial);
                _total_time_spent    = _start_global + _time_spent_in_trial;

                // refresh state with these values
                RefreshState();
                RefreshCfgState();

                 // in this iteration i have to send data about my local state to the global state
                if ((int)current_iteration() % params.refresh_global_state() ==0)
                        send_local_state_to(mypid);

                UpdateFromCfgState();
                _stat.update(*this);
                _userstat.update(*this);

                // if (display_state()) show_state();
        }
		
        void Solver_Wan::send_local_state_to(int _mypid)
        {
                _netstream << set_target(0);
                _netstream << pack_begin
                           << _mypid
                           << current_trial()
                           << current_iteration()
                           << best_cost_trial()
                           << best_solution_trial()
                           << iteration_best_found_in_trial()
                           << time_best_found_trial()
                           << worst_cost_trial()
                           << current_best_cost()
                           << current_best_solution()
                           << current_worst_cost()
                           << current_average_cost()
                           << current_standard_deviation()
                           << pack_end;
        }
        
        
        int Solver_Wan::receive_local_state()
        {
                int r_pid=0;

                _netstream << wait(packed);

                _netstream << pack_begin
                           >> r_pid
                           >> _current_trial
                           >> _current_iteration
                           >> _best_cost_trial
                           >> _best_solution_trial
                           >> _iteration_best_found_in_trial
                           >> _time_best_found_in_trial
                           >> _worst_cost_trial
                           >> _best_cost
                           >> _best_solution
                           >> _worst_cost
                           >> _average_cost
                           >> _standard_deviation
                           << pack_end;
                return r_pid;
        }
        

    
		void Solver_Wan::check_for_refresh_global_state() // Executed in process with pid 0
        {
                unsigned int nb_finalized_processes=0;
                int received_pid;
                int nb_proc=_netstream.pnumber();

                _netstream << set_source(MPI_ANY_SOURCE);

                while (!_end_trial)
                {
                        received_pid=0;
                        received_pid=receive_local_state();

                        current_trial(_current_trial);

                        // the process that has send data has finished the current trial
                        if (received_pid==-1) nb_finalized_processes++;
                        if (nb_finalized_processes==nb_proc-1) _end_trial=true;

                        // refresh the global state with received data ( a local state )
                        current_iteration(_iteration_best_found_in_trial);
                        KeepHistory(_best_solution_trial,_best_cost_trial,_worst_cost_trial,_time_best_found_in_trial,_start_global + _time_best_found_in_trial);
                        current_iteration(_current_iteration);

                        _time_spent_in_trial = _used_time(_start_trial);
                        _total_time_spent    = _start_global + _time_spent_in_trial;
                        RefreshState();

                        _stat.update(*this);
                        _userstat.update(*this);

                        // display current global state
                        if (display_state()) show_state();
                } // end while

                // display the global state at the end of the current trial
                if (display_state()) show_state();
        }
		
		 void Solver_Wan::run ()
        {
                while (current_trial() < params.independent_runs())// && !(terminateQ(problem,*this,params)))
                        run(params.nb_evolution_steps());
        }

        void Solver_Wan::run (const unsigned long int nb_generations)
        {
                StartUp();
                if (mypid!=0)
                {
                        while ((current_iteration() < nb_generations))// && !(terminateQ(problem,*this,params)))
                                DoStep();
                        send_local_state_to(-1);
                }
                else
                {
                        check_for_refresh_global_state();
                }

                _netstream << barrier;
                reset();
        }
				
		void Solver_Wan::run (const Swarm& swarm,const unsigned long int nb_generations)
        {
                StartUp(swarm);
                if (mypid!=0)
                {
                        while ((current_iteration() < nb_generations)) //&& !(terminateQ(problem,*this,params)))
                                DoStep();
                        send_local_state_to(-1);
                }
                else
                {
                        check_for_refresh_global_state();
                }

                _netstream << barrier;
                reset();
        }

        void Solver_Wan::reset()
        {
                Solution left_solution(problem);
                if (mypid!=0)
                {
                        int pending=false;
                        do
                        {
                                pending=false;
                                _netstream._probe(any,pending);
                                if (pending) _netstream >> left_solution;
                         } while (pending);
                }
                _netstream << barrier;
        }
		

}//End of provides Skeleton
