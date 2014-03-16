#ifndef INC_REQ_ES
#define INC_REQ_ES
#include "ES.hh"

skeleton ES
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_dimension(0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension  << endl;
		os << "Variable Range: [" << pbm._minvalue << "," << pbm._maxvalue << "]" << endl;
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];

		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%d",&pbm._dimension);

		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%f %f",&pbm._minvalue,&pbm._maxvalue);

		return is;
	}

	Problem& Problem::operator=  (const Problem& pbm)
	{
		_dimension=pbm.dimension();
		_minvalue = pbm.minvalue(0);
		_maxvalue = pbm.maxvalue(0);
		return *this;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if ((_dimension!=pbm.dimension()) ||
		    (fabs(_minvalue - pbm.minvalue(0)) > 0.01) ||
		    (fabs(_maxvalue - pbm.maxvalue(0)) > 0.01))
			return false;
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		//return maximize;
		return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	float Problem::minvalue(const int index) const
	{
		return _minvalue;
	}

	float Problem::maxvalue(const int index) const
	{
		return _maxvalue;
	}

	Problem::~Problem()
	{}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),_variables(pbm.dimension()),
						_parameters(pbm.dimension()),
						_angles(((pbm.dimension() * (pbm.dimension() - 1)) / 2))
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
		for (int i=0;i<sol.pbm().dimension();i++)
			is >> sol._variables[i];
		for (int i=0;i<sol.pbm().dimension();i++)
			is >> sol._parameters[i];
		for (int i=0;i<((sol.pbm().dimension() * (sol.pbm().dimension() - 1)) / 2);i++)
			is >> sol._angles[i];
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			os << " " << sol._variables[i];
	        os << " (";
		for (int i=0;i<sol.pbm().dimension();i++)
			os << " " << sol._parameters[i];
		os << " <->";
		for (int i=0;i<((sol.pbm().dimension() * (sol.pbm().dimension() - 1)) / 2);i++)
			os << " " << sol._angles[i];
		os << " )" << endl;
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			ns << sol._variables[i];
		for (int i=0;i<sol.pbm().dimension();i++)
			ns << sol._parameters[i];
		for (int i=0;i<((sol.pbm().dimension() * (sol.pbm().dimension() - 1)) / 2);i++)
			ns << sol._angles[i];

		return ns;
	}


	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			ns >> sol._variables[i];
		for (int i=0;i<sol.pbm().dimension();i++)
			ns >> sol._parameters[i];
		for (int i=0;i<((sol.pbm().dimension() * (sol.pbm().dimension() - 1)) / 2);i++)
			ns >> sol._angles[i];

		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		_variables=sol._variables;
 		_parameters=sol._parameters;
 		_angles=sol._angles;
		return *this;
	}

	bool Solution::operator== (const Solution& sol) const
	{
		if( (_variables != sol._variables)
		 || (_parameters != sol._parameters)
		 || (_angles != sol._angles))
		 	return false;
		else	return true;
	}

	bool Solution::operator!= (const Solution& sol) const
	{
		return !(*this == sol);
	}

	void Solution::initialize()
	{
		float min = _pbm.minvalue(0);
		float rang = _pbm.maxvalue(0) - min;

		for (int i=0;i<_pbm.dimension();i++)
		{
			_variables[i]  = (float) (rand01()*rang) + min;
			_parameters[i] = (float) rand01()*2.0 - 1.0;
		}
		for (int i = 0; i < ((_pbm.dimension() * (_pbm.dimension() - 1)) / 2);i++)
		{
			_angles[i]     = (float) (rand01() * 2 * PI);
		}
	}

	double Solution::fitness () const
	{
		double fitness = 0.0,acum,aux;

		for(int i = 0; i < _pbm.dimension(); i++)
		{
			aux = (double) _variables[i];
			acum = aux*aux  - 10*cos(2*PI*aux);
			fitness += acum;
		}
		fitness += (10.0 * _pbm.dimension());

		//return 1.0/fitness;
		return fitness;
	}

	unsigned int Solution::size() const
	{
		int n = _pbm.dimension();
	 	return ((2*n + (n*(n-1))/2 ) * sizeof(float));
	}

	char *Solution::to_String() const
	{
		static char *cad = NULL;

		if(cad != NULL) delete [] cad;

		cad = new char[size()];
		if(cad == NULL) show_message(7);

		float *aux = (float *)cad;
		int n = _pbm.dimension();
		int j ,i;

		if(!aux) show_message(7);

		for(i = 0,j = 0; i < n; i++,j++)
		{
		 	aux[j] = _variables[i];
		}
		for(i = 0,j = n; i < n; i++,j++)
		{
		 	aux[j] = _parameters[i];
		}
		for(i = 0,j = n*2; i < (n * (n-1))/2; i++,j++)
		{
		 	aux[j] = _angles[i];
		}

	 	return cad;
	}


	void Solution::to_Solution(char *_cadena_)
	{
	 	float *ptr=(float *)_cadena_;
		int n = _pbm.dimension();

	 	for (int i=0;i< n;i++)
	 	{
		 	_variables[i]=*ptr;
		 	ptr++;
		}

	 	for (int i=0;i< n;i++)
	 	{
		 	_parameters[i]=*ptr;
		 	ptr++;
		}


	 	for (int i=0;i< (n * (n-1))/2 ;i++)
	 	{
		 	_angles[i]=*ptr;
		 	ptr++;
		}

	}

	float& Solution::variable(const int index)
	{
		return _variables[index];
	}

	float& Solution::parameter(const int index)
	{
		return _parameters[index];
	}

	float& Solution::angle(const int index)
	{
		return _angles[index];
	}

	Matrix<float>& Solution::variables()
	{
		return _variables;
	}

	Matrix<float>& Solution::parameters()
	{
		return _parameters;
	}

	Matrix<float>& Solution::angles()
	{
		return _angles;
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
		if ((solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
			 && !terminateQ(solver.pbm(), solver, solver.setup())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         				 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     			 = solver.worst_cost_trial();
		new_stat->best_cost_trial     			 = solver.best_cost_trial();
		new_stat->time_best_found_trial			 = solver.time_best_found_trial();
		new_stat->time_spent_trial 				 = solver.time_spent_trial();

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

	//  User_Operator:Intra_operator ---------------------------------------------------------

	User_Operator::User_Operator(const unsigned int _number_op):Intra_Operator(_number_op)
	{}

	void User_Operator::execute(Rarray<Solution*>& sols) const
	{}

	void User_Operator::setup(char line[MAX_BUFFER])
	{}

	Intra_Operator *User_Operator::create(const unsigned int _number_op)
	{
		return new User_Operator(_number_op);
	}

	ostream& operator<< (ostream& os, const User_Operator&  u_op)
	{
		 os << "User Operator.";
		 return os;
	}

	void User_Operator::RefreshState(const StateCenter& _sc) const
	{}

	void User_Operator::UpdateFromState(const StateCenter& _sc)
	{}

	User_Operator::~User_Operator()
	{}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return ((double)solver.best_cost_trial() < 0.01);
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

