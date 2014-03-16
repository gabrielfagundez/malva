/************************************************
***				  	      *** 
***  Particle Swarm      Skeleton v1.0        *** 
***  Required classes and methods             ***
***  Developed by: José Manuel Garcia Nieto   *** 
***                                           *** 
************************************************/

#include "PSO.hh"
#include "Mallba/random.hh"

skeleton PSO {


// Problem ---------------------------------------------------------------
		Problem::Problem ():_dimension(0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension
		   << endl;
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int i;

		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%d",&pbm._dimension);

		return is;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if (_dimension!=pbm.dimension()) return false;
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
	  return maximize;
	  //return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	Problem::~Problem()
	{
	}

// Solution --------------------------------------------------------------
  	
	Solution::Solution (const Problem& pbm):
	  _pbm(pbm),
	  _current(pbm.dimension()),
	  _next(pbm.dimension()),
	  _best(pbm.dimension()),
	  _velocity(pbm.dimension())
        {}

   const Problem& Solution::pbm() const
        {
                return _pbm;
        }

    Solution::Solution(const Solution& sol):
        _pbm(sol.pbm()),
        _current(sol.pbm().dimension()),
	  	_next(sol.pbm().dimension()),
	  	_best(sol.pbm().dimension()),
	  	_velocity(sol.pbm().dimension())
        {
        		
                *this=sol;
        }

    istream& operator>> (istream& is, Solution& sol)
        {
	  for (int i=0;i<sol.pbm().dimension();i++)
	    is >> sol._current[i];
	  //is >> endl;

	  for (int i=0;i<sol.pbm().dimension();i++)
	    is >> sol._next[i];
	  //is >> endl;

	  for (int i=0;i<sol.pbm().dimension();i++)
	    is >> sol._best[i];
	  //is >> endl;

	  for (int i=0;i<sol.pbm().dimension();i++)
	    is >> sol._velocity[i];
	  
          return is;
        }

        ostream& operator<< (ostream& os, const Solution& sol)
        {
        	//os << endl;
        	os << "current: ";
	  		for (int i=0;i<sol.pbm().dimension();i++)
	    		os << " " << sol._current[i];
	  		/*os << endl;
			os << "next: ";	  
	  		for (int i=0;i<sol.pbm().dimension();i++)
	    		os << " " << sol._next[i];
	  		os << endl;
			os << "best: ";
	  		for (int i=0;i<sol.pbm().dimension();i++)
	    		os << " " << sol._best[i];
	  		os << endl;
			os << "velocity: ";
	  		for (int i=0;i<sol.pbm().dimension();i++)
	    		os << " " << sol._velocity[i];*/
	  
	  		return os;
        }

	NetStream& operator << (NetStream& ns, const Solution& sol)
        {
                for (int i=0;i<sol._current.size();i++)
                        ns << sol._current[i];
                for (int i=0;i<sol._next.size();i++)
                        ns << sol._next[i];
                for (int i=0;i<sol._best.size();i++)
                        ns << sol._best[i];
                for (int i=0;i<sol._velocity.size();i++)
                        ns << sol._velocity[i];
                return ns;
        }


        NetStream& operator >> (NetStream& ns, Solution& sol)
        {
                for (int i=0;i<sol._current.size();i++)
                        ns >> sol._current[i];
                for (int i=0;i<sol._next.size();i++)
                        ns >> sol._next[i];
                for (int i=0;i<sol._best.size();i++)
                        ns >> sol._best[i];
                for (int i=0;i<sol._velocity.size();i++)
                        ns >> sol._velocity[i];
                return ns;
        }
	   

	Solution& Solution::operator= (const Solution &sol)
        {
                _current = sol._current;
				_best    = sol._best;
				_next    = sol._next;
				_velocity= sol._velocity;

                return *this;
        }

        bool Solution::operator== (const Solution& sol) const
        {
                if (sol.pbm() != _pbm) return false;
                return true;
        }

        bool Solution::operator!= (const Solution& sol) const
        {
                return !(*this == sol);
        }

        void Solution::initialize()
        {
	  		for (int i=0;i<_pbm.dimension();i++){
	    		_current[i]=rand_int(0,1);
	    		//_next[i]=_current[i];
	    		_best[i]=_current[i];	    		
	    		_velocity[i]=rand_int(-1,1);
	  		}
        }

	//for onemax problem
        double Solution::fitness ()
		{
	    	double fitness = 0.0;

            for (int i=0;i<_current.size();i++)
                fitness += _current[i];

			_current_fitness=fitness;
		
            return fitness;
        }

	double Solution::best_fitness()
	  {
	    return _best_fitness;
	  }

	void Solution::best_fitness(const double bf)
	  {
	    _best_fitness=bf;
	  }

	char *Solution::to_String() const
        {
                return (char *)_current.get_first();
        }

    void Solution::to_Solution(char *_string_)
        {
                int *ptr=(int *)_string_;
                for (int i=0;i<_pbm.dimension();i++)
                {
                        _current[i]=*ptr;
                        ptr++;
                }
        }
	                
    unsigned int Solution::size() const
        {
                return (_pbm.dimension() * sizeof(int));
        }


    int& Solution::current(const int index)
        {
                return _current[index];
        }

	int& Solution::next(const int index)
	  {
	    return _next[index];
	  }

	int& Solution::best(const int index)
	  {
	    return _best[index];
	  }
                
	int& Solution::velocity(const int index)
	  {
	    return _velocity[index];
	  }
                
	void Solution::current(const int index, const int value)
	  {
	    _current[index]=value;
	  }

	void Solution::next(const int index, const int value)
	  {
	    _next[index]=value;
	  }
                
	void Solution::best(const int index, const int value)
	  {
	    _best[index]=value;
	  }

                
	void Solution::velocity(const int index, const int value)
	  {
	    _velocity[index]=value;
	  }
                            
	Rarray<int>& Solution::current()
	  {
	    return _current;
	  }

	Rarray<int>& Solution::next()
	  {
	    return _next;
	  }

	Rarray<int>& Solution::best()
	  {
	    return _best;
	  }
                
	Rarray<int>& Solution::velocity()
	  {
	    return _velocity;
	  }


        Solution::~Solution()
        {}

	


// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;

		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t\t" << userstat.result_trials[i].worst_cost_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;
		}
		os << endl << "------------------------------------------------------------------" << endl;
		return os;
	}

	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		result_trials=userstats.result_trials;
		return (*this);
	}

	void UserStatistics::update(const Solver& solver)
	{
		if (!(solver.pid()==0 && (solver.end_trial()==true)
		  && (solver.current_iteration()==solver.setup().nb_evolution_steps())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         		 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     		 = solver.worst_cost_trial();
		new_stat->best_cost_trial     		 = solver.best_cost_trial();
		new_stat->time_best_found_trial		 = solver.time_best_found_trial();
		new_stat->time_spent_trial 		 = solver.time_spent_trial();

		result_trials.append(*new_stat);
	}

	void UserStatistics::clear()
	{
		result_trials.remove();
	}

	UserStatistics::~UserStatistics()
	{
		result_trials.remove();
	}
// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return ((int)solver.best_cost_trial() == pbm.dimension());
	}

	StopCondition_1::~StopCondition_1()
	{}

	//------------------------------------------------------------------------
	// Specific methods ------------------------------------------------------
	//------------------------------------------------------------------------

	bool terminateQ (const Problem& pbm, const Solver& solver,
			 const SetUpParams& setup)
	{
		StopCondition_1 stop;
		return stop.EvaluateCondition(pbm,solver,setup);
	}

	
}
