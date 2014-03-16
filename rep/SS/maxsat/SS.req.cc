#ifndef INC_REQ_SS
#define INC_REQ_SS
#include "SS.hh"
#include <math.h>

skeleton SS
{

	// Problem ---------------------------------------------------------------
    Problem::Problem ():_numvar(0),_numclause(0),_clauses(NULL),_lenclause(0)
    {}

    ostream& operator<< (ostream& os, const Problem& pbm)
    {
        os << endl << endl << "Number of variables " << pbm._numvar
           << endl << endl << "Number of clauses " << pbm._numclause
           << endl << endl << "Length of clauses " << pbm._lenclause
           << endl << endl
           << " Clauses: " << endl;

        for (int i=0;i<pbm._numclause;i++)
        {
            os << "\t clause " << i << "\t ";
            for(int j = 0; j < pbm._lenclause; j++)
                os << pbm._clauses[i][j] << " ";
            os << endl;
        }
        return os;
    }

    istream& operator>> (istream& is, Problem& pbm)
    {
        int l;
        int n;
        is >> pbm._numvar >> pbm._numclause >> pbm._lenclause;

        n = pbm._lenclause;
        // read clauses
        pbm._clauses = new int*[pbm._numclause];

        for (int i = 0; i < pbm._numclause; i++)
        {
            pbm._clauses[i] = new int[n];
            for(int j = 0; j < n;j++)
            {
                is >> l;
                pbm._clauses[i][j] = l;
            }
            is >> l;
        }

        return is;
    }
	
    Problem& Problem::operator=  (const Problem& pbm)
    {
        int n;
        _numvar=pbm.numvar();
        for(int i = 0; i < _numclause;i++)
            delete [] _clauses[i];
        delete [] _clauses;

        _numclause = pbm.numclause();
        n = _lenclause = pbm.lenclause();

        _clauses = new int *[_numclause];

        for(int i = 0; i < pbm._numclause;i++)
        {
            _clauses[i] = new int [n];
            for(int j = 0; j < n ; j++)
                _clauses[i][j] = pbm._clauses[i][j];
        }
        return *this;
    }

	bool Problem::operator== (const Problem& pbm) const
	{
        if (_numvar!=pbm.numvar()) return false;
        for (int i = 0; i < _numclause; i++)
            for(int j = 0; j < _lenclause;j++)
            if ( _clauses[i][j] != pbm._clauses[i][j])
                return false;
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

    int Problem::numvar() const
    {
        return _numvar;
    }

    int Problem::numclause() const
    {
        return _numclause;
    }

    int Problem::lenclause() const
    {
        return _lenclause;
    }

    int *Problem::clause(const int i) const
    {
        return _clauses[i];
    }

    Problem::~Problem()
    {
        for(int i = 0;i < _numclause;i++)
            delete [] _clauses[i];

        delete [] _clauses;
    }

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),_var(pbm.numvar())
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
        for (int i=0;i<sol.pbm().numvar();i++)
            is >> sol._var[i];
        return is;
    }

    ostream& operator<< (ostream& os, const Solution& sol)
    {
        for (int i=0;i<sol.pbm().numvar();i++)
            os << " " << sol._var[i];
        return os;
    }

    NetStream& operator << (NetStream& ns, const Solution& sol)
    {
        for (int i=0;i<sol._var.size();i++)
            ns << sol._var[i];
        return ns;
    }

    NetStream& operator >> (NetStream& ns, Solution& sol)
    {
        for (int i=0;i<sol._var.size();i++)
            ns >> sol._var[i];
        return ns;
    }

    Solution& Solution::operator= (const Solution &sol)
    {
        _var=sol._var;
        return *this;
    }

    bool Solution::operator== (const Solution& sol) const
    {
        if (sol.pbm() != _pbm) return false;
        for(int i = 0; i < _var.size(); i++)
            if(_var[i] != sol._var[i]) return false;
        return true;
    }

    bool Solution::operator!= (const Solution& sol) const
    {
        return !(*this == sol);
    }

    void Solution::initialize()
    {
        for (int i=0;i<_pbm.numvar();i++)
            _var[i]=rand_int(0,1);
    }

    double Solution::fitness ()
    {
        double fitness = 0.0;
        int acum = 0;

        for(int i = 0; i < _pbm.numclause(); i++)
        {
            int *rl = _pbm.clause(i);
            acum = 0;
            for(int j = 0; (j < _pbm.lenclause()) && (acum != 1);j++)
            {
                if( ((rl[j] < 0) && (_var[(int)abs(rl[j])-1] == 0))
                 || ((rl[j] > 0) && (_var[rl[j]-1] == 1)) )
                    acum = 1;
            }
            fitness += acum;
        }
        return fitness;
    }

	void Solution::improve ()
	{
		Solution aux(*this);

		double f = aux.fitness();

		for(unsigned i = 0; i < 50; i++)
		{
			for(int j = 0; j < _pbm.numvar(); j++)
			{
        		if(rand01() < 1/_pbm.numvar())
            	{
					if(_var[j] == 1) _var[j] = 0;
					else _var[j] = 1;	
        		}
			}

			double nf = fitness();
			if(nf > f) f = nf;
			else *this = aux;
		}
	}
		
	void Solution::initialize(const Rarray<Solution*>&sols, unsigned size)
	{
		initialize();
	}

	char *Solution::to_String() const
	{
		return (char *)_var.get_first();
	}

	void Solution::to_Solution(char *_string_)
	{
		int  *ptr=(int*)_string_;
		
		for (int i=0;i<_pbm.numvar();i++, ptr++)
				_var[i] = *ptr;
	}

	unsigned int Solution::size() const
	{
		return (_pbm.numvar() * sizeof(int));
	}

	double Solution::distance(const Solution &sol) const
	{
		double dist = 0;
		for(int i = 0; i < _pbm.numvar(); i++)
		{
			dist += (_var[i] == sol._var[i])?0:1;
		}

		return dist;
	}

	int& Solution::var(const int index)
	{
		return _var[index];
	}


	Rarray<int> &Solution::array_var()
	{
		return _var;
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
		int i=0;
		const int size = sols[1]->pbm().numvar();

		int limit=rand_int(size/2+1,size-1);
		int limit2=rand_int(0,limit-1);

		for (i=0;i<limit2;i++)
			sol.var(i)=sols[0]->var(i);
		for (i=limit2;i<limit;i++)
			sol.var(i)=sols[1]->var(i);
		for (i=limit;i<size;i++)
			sol.var(i)=sols[0]->var(i);
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
        return ((int)solver.best_cost_trial() == pbm.numclause());
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

