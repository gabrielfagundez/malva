#ifndef INC_REQ_SS
#define INC_REQ_SS
#include "SS.hh"
#include <math.h>

skeleton SS
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ()
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		return is;
	}
	
	bool Problem::operator== (const Problem& pbm) const
	{
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

	Problem::~Problem()
	{
	}


	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm)
	{}

	const Problem& Solution::pbm() const
	{
		return _pbm;
	}

    Solution::Solution(const Solution& sol):_pbm(sol.pbm())
    {
        *this=sol;
    }

    istream& operator>> (istream& is, Solution& sol)
    {
        return is;
    }

    ostream& operator<< (ostream& os, const Solution& sol)
    {
        return os;
    }

    NetStream& operator << (NetStream& ns, const Solution& sol)
    {
        return ns;
    }

    NetStream& operator >> (NetStream& ns, Solution& sol)
    {
        return ns;
    }

    Solution& Solution::operator= (const Solution &sol)
    {
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
    }

    double Solution::fitness ()
    {
        double fitness = 0.0;

        return fitness;
    }

	void Solution::improve ()
	{
	}
		
	void Solution::initialize(const Rarray<Solution*>&sols, unsigned size)
	{
	}

	char *Solution::to_String() const
	{
		return NULL;
	}

	void Solution::to_Solution(char *_string_)
	{
	}

	unsigned int Solution::size() const
	{
		return 1;
	}

	double Solution::distance(const Solution &sol) const
	{
		double dist = 0;
		return dist;
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
			   << "\t\t\t" << userstat.result_trials[i].nb_iteration_best_found_trial
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
		if( (solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
		       && !terminateQ(solver.pbm(),solver,solver.setup())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         		 		 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     		 	 = solver.worst_cost_trial();
		new_stat->best_cost_trial     		 	 = solver.best_cost_trial();
		new_stat->time_best_found_trial		 	 = solver.time_best_found_trial();
		new_stat->time_spent_trial 		 		 = solver.time_spent_trial();

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

// Intra_operator  --------------------------------------------------------------

	Intra_Operator::Intra_Operator(const unsigned int _number_op):_number_operator(_number_op),probability(NULL)
	{}

	unsigned int Intra_Operator::number_operator() const
	{
		return _number_operator;
	}

	Intra_Operator *Intra_Operator::create(const unsigned int _number_op)
	{
		switch (_number_op)
		{
			case 0: return new Crossover;break;
		}
	}

	ostream& operator<< (ostream& os, const Intra_Operator& intra)
	{
		switch (intra.number_operator())
		{
			case 0: os << (Crossover&)intra;break;
		}
		return os;
	}

	Intra_Operator::~Intra_Operator()
	{}

//  Crossover:Intra_operator -------------------------------------------------------------

	Crossover::Crossover():Intra_Operator(0)
	{
		probability = new float[2];
	}

	void Crossover::cross(const Rarray<Solution*>& sols, Solution & sol) const // dadas dos soluciones de la poblacion, las cruza
	{
	}

	void Crossover::execute(Rarray<Solution*>& sols) const
	{
		Rarray<Solution*> pop(rs1+rs2);
		for(unsigned i = 0; i < rs1 + rs2; i++)
			pop[i] = new Solution(*sols[i]);
	
		unsigned k = 0;		
		for (unsigned i=0; i<pop.size();i++)
		{
			for(unsigned j = i+1; j < pop.size(); j++)
			{
				Rarray<Solution*> pop_aux(2);
				pop_aux[0] = pop[i];
				pop_aux[1] = pop[j];
		 		cross(pop_aux, *sols[k]);
				k++;
			}
		}

		for(unsigned i = 0; i < pop.size(); i++)
			delete pop[i];
	}

	ostream& operator<< (ostream& os, const Crossover&  cross)
	{
		 os << "Crossover." << " Popsizes: "
                    << cross.rs1
                    << " " << cross.rs2
		    << endl;
		 return os;
	}

	void Crossover::RefreshState(const StateCenter& _sc) const
	{
//		_sc.set_contents_state_variable("_crossover_probability",(char *)probability,2,sizeof(float));
	}

	void Crossover::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
//		 _sc.get_contents_state_variable("_crossover_probability",(char *)probability,nbytes,length);
	}

	void Crossover::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line,"%d %d ", &rs1, &rs2);
	}

	Crossover::~Crossover()
	{
		delete [] probability;
	}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
        return (false);
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
#endif

