#ifndef INC_REQ_newGA
#define INC_REQ_newGA
#include "newGA.hh"
#include <math.h>

skeleton newGA
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_nCustomers(0),_capacity(0),_distance(NULL),
			    _demand(NULL),_service_time(0),_maxRouteTime(0.0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "  Vehicle capacity: " << pbm._capacity
		   << endl << "  Number of Customers: " << pbm._nCustomers  << endl
		   << "  Service time: " << pbm._service_time << endl
		   << "  Max Time for any route: " << pbm._maxRouteTime << endl
		   << "  Distances: " << endl;

		for(int i = 0; i < (pbm._nCustomers + 1) ; i++)
		{
			for(int j = 0; j < pbm._nCustomers; j++)
		 		os << " " << pbm.distance(i,j);
			os << endl;
		}

		os << endl << "  Demand for each customer: " << endl;

		for(int i = 0; i < (pbm._nCustomers + 1)  ; i++)
		{
			os << "  " << i << "\t" << pbm._demand[i] << endl;
		}

		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int *x;
		int *y;

                is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer," %d %d %lf %d %ld",&pbm._nCustomers,&pbm._capacity,&pbm._maxRouteTime,&pbm._service_time,&pbm._best_cost);

		if( ((pbm._distance = new double* [pbm._nCustomers + 1]) == NULL)
		 || ((pbm._demand = new int[pbm._nCustomers + 1]) == NULL)
		 || ((x = new int[pbm._nCustomers + 1]) == NULL)
		 || ((y = new int[pbm._nCustomers + 1]) == NULL))
			show_message(7);

		// Read depot coordenates
		if( ((pbm._distance[0] = new double[pbm._nCustomers+1]) == NULL))
			show_message(7);

	 	is.getline(buffer,MAX_BUFFER,'\n');
	 	sscanf(buffer," %d %d",&(x[0]),&(y[0]));
	 	pbm._demand[0] = 0;

	 	// Read customers coordenates

		for(int i = 1; i < (pbm._nCustomers + 1); i++)
		{
			if( ((pbm._distance[i] = new double[pbm._nCustomers+1]) == NULL))
				show_message(7);

			is.getline(buffer,MAX_BUFFER,'\n');
			sscanf(buffer," %d %d %d", &(x[i]),&(y[i]),&(pbm._demand[i]));
		}

		pbm.genDistances(x,y);

		delete [] x;
                delete [] y;

		return is;
	}

	void Problem::genDistances(int *x,int *y)
	{
		for(int i = 0; i < (_nCustomers + 1); i++)
			for(int j = 0; j < (_nCustomers +1) ; j++)
				_distance[i][j] = sqrt( pow( (double) (x[j] - x[i]),2 ) +
						  pow( (double) (y[j] - y[i]),2 ) ) + _service_time ;
	}

	Problem& Problem::operator=  (const Problem& pbm)
	{
		if(_distance != NULL)
		{
			for(int i= 0;i < (_nCustomers+1); i++)
				if(_distance[i] != NULL) delete [] _distance[i];
			delete [] _distance;
		}

		if(_demand != NULL) delete [] _demand;

		if( ((_distance = new double*[pbm._nCustomers]) == NULL)
		 || ((_demand = new int[pbm._nCustomers + 1]) == NULL))
		  	show_message(7);

		_nCustomers = pbm.nCustomers();
		_capacity = pbm.capacity();
		_service_time = pbm.service_time();
		_maxRouteTime = pbm.maxRouteTime();

		for(int i = 0; i < (_nCustomers+1); i++)
		{
			_demand[i] = pbm.demand(i);

			if( (_distance[i] = new double[_nCustomers+1]) == NULL)
				show_message(7);

			for(int j = 0; j < (_nCustomers+1); i++)
		 		_distance[i][j] = pbm.distance(i,j);
		}

		return *this;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if( (_maxRouteTime != pbm.maxRouteTime()) || (_nCustomers != pbm.nCustomers())
		 || (_capacity != pbm.capacity()) || (_service_time != pbm.service_time()))
		 	return false;

		for(int i = 0; i < (_nCustomers+1); i++)
		{
			if(_demand[i] != pbm.demand(i))
			 	return false;

		 	for(int j = 0; j < (_nCustomers+1); i++)
		 		if(distance(i,j) != pbm.distance(i,j)) return false;
		}

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

	double Problem::maxRouteTime() const
	{
		return _maxRouteTime;
	}

	int Problem::nCustomers() const
	{
		return _nCustomers;
	}

	int Problem::capacity() const
	{
		return _capacity;
	}

	double Problem::distance(const int i, const int j) const
	{
		return _distance[i][j];
	}

	int Problem::demand(const int i) const
	{
		return _demand[i];
	}

	int Problem::service_time() const
	{
		return _service_time;
	}

	double Problem::best_cost() const
	{
		return _best_cost;
	}

	Problem::~Problem()
	{
		if(_distance != NULL)
		{
			for(int i= 0;i < (_nCustomers+1); i++)
				if(_distance[i] != NULL) delete [] _distance[i];

			delete [] _distance;
		}

		if(_demand != NULL) delete [] _demand;
	}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm)//,_dimension(pbm.nCustomers()+15),_routes(_dimension)
	{
		_dimension = pbm.nCustomers()+10;
		_routes = Rarray<int>(_dimension);
	}

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
		for (int i=0;i<sol.dimension(); i++)
		{
			is >> sol._routes[i];
		}
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.dimension(); i++)
		{
			os << (sol._routes[i]<10?" ":"") << sol._routes[i] << " ";
		}

		return os;
	}

	void	Solution::print()
	{
		int i = 0;
		int last = 0;
		double cost = 0;

		int nRoute = 1;

		cout << " Ruta " << nRoute << " =  0 ";
		while(i < dimension())
		{
			if(_routes[i] <= _pbm.nCustomers())
			{
				cout << _routes[i] << " ";
				cost += _pbm.distance(last,_routes[i]);
				last = _routes[i];
			}
			else
			{	
				cost += _pbm.distance(last,0);
				cout << "0" << endl << " Ruta " << ++nRoute << " = 0 ";
				last = 0;
			}
			i++;
		}
		cout << "0" << endl;
		cout << "Cost = " << cost << endl;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol.dimension();i++)
			ns << sol._routes[i];

		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol.dimension();i++)
			 ns >> sol._routes[i];

		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		_dimension = sol._dimension;
		_routes = sol._routes;
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
		register int aux,ind1,ind2;

		int max = _dimension;

		// Create Ordered Array from 0 to nCustomers
		for(int i = 0; i < max; i++)
			_routes[i] = i+1;

		cout << endl;

		// move values to create random array
		for(int i=0;i< (max*5) ; i++)
		{
			ind1 = rand_int(0,max-1);
			ind2 = rand_int(0,max-1);

			aux = _routes[ind1];
			_routes[ind1] = _routes[ind2];
			_routes[ind2] = aux;
		}

	}

	double Solution::fitness()
	{
		int i    = 0;
		int last = 0;
		double cost = 0;
		int cap  = 0;
		int capa = 0;
		double tim  = 0;
		double aux  = 0;
		double tima = 0;
		

		while(i < dimension())
		{
			if(_routes[i] <= _pbm.nCustomers())
			{
				aux   = _pbm.distance(last,_routes[i]);
				cost += aux;
				tim  += aux;
				cap  += _pbm.demand(_routes[i]);
				last  = _routes[i];
			}
			else
			{	
				aux   = _pbm.distance(last,0);
				cost += aux;
				tim  += aux;

				cap   = cap - _pbm.capacity();
				capa += (cap>0)? cap:0;

				tim   = tim - _pbm.maxRouteTime();
				tima += (tim>0)? tim:0;
				
				last = 0;
				tim  = 0;
				cap  = 0;
			}
			i++;
		}
		
		return cost;
	}

	char *Solution::to_String() const
	{
		return (char *)_routes.get_first();
	}

	void Solution::to_Solution(char *_routes_)
	{
	 	int *ptr=(int *)_routes_;

		for (int i=0;i<_dimension;i++)
		{
			_routes[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return ( dimension() * sizeof(int));
	}

	unsigned int Solution::dimension() const
	{
		return _dimension;
	}


	int & Solution::pos(const int index)
	{
		return _routes[index];
	}

 	Rarray<int>& Solution::routes()
	{
		return _routes;
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
		if( (solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
		       && !terminateQ(solver.pbm(),solver,solver.setup())))
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
			case 0: return new Crossover_PMX1();break;
			case 1: return new Mutation();break;
			case 2: return new Crossover_PMX2();break;
			case 3: return new Crossover_OX();break;
			case 4: return new Crossover_CX();break;
			case 5: return new Crossover_ER();break;
		}
	}

	ostream& operator<< (ostream& os, const Intra_Operator& intra)
	{
		switch (intra.number_operator())
		{
			case 0: os << (Crossover_PMX1&)intra;break;
			case 1: os << (Mutation&)intra;break;
			case 2: os << (Crossover_PMX2&)intra;break;
			case 3: os << (Crossover_OX&)intra;break;
			case 4: os << (Crossover_CX&)intra;break;
			case 5: os << (Crossover_ER&)intra;break;
		}
		return os;
	}

	Intra_Operator::~Intra_Operator()
	{}

//  Crossover_PMX1:Intra_operator -------------------------------------------------------------

	Crossover_PMX1::Crossover_PMX1():Intra_Operator(0)
	{
		probability = new float[1];
	}

	// PMX : Partially-Mapped Crossover
	void Crossover_PMX1::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		int i,j;

		// Copy old solutions
		Rarray<int> aux1(sol1.dimension());
		aux1=sol1.routes();
		Rarray<int> aux2(sol2.dimension());
		aux2=sol2.routes();

		int limit2=rand_int(1,sol1.dimension()-1);
		int limit1=rand_int(0,limit2-1);

		for (i = limit1; i < limit2; i++)
		{
			sol2.pos(i) = aux1[i];
			sol1.pos(i) = aux2[i];
		}

		for (i = 0; i < limit1; i++)
		{
			sol1.pos(i) = newValue(aux1[i],limit1,limit2,aux1,aux2);
			sol2.pos(i) = newValue(aux2[i],limit1,limit2,aux2,aux1);
		}

		for (i = limit2; i < sol1.pbm().nCustomers(); i++)
		{
			sol1.pos(i) = newValue(aux1[i],limit1,limit2,aux1,aux2);
		 	sol2.pos(i) = newValue(aux2[i],limit1,limit2,aux2,aux1);
		}

		complete(sol1);
		complete(sol2);
	}

	// Auxiliar function for PMX
	int Crossover_PMX1::newValue(const int oldValue,const int l1,const int l2, Rarray<int> & s1, Rarray<int> & s2) const
	{
		bool fin = false;
		int nv = oldValue;
		int n = s1.size();
		bool *examinado = new bool[n];

		for(int i = 0; i < n; i++) examinado[i] = false;

		while (!fin)
		{
			fin = true;
			for(int i = l1; i < l2; i++)
				if(nv == s2[i])
				{
					if(!examinado[i])
					{
        					nv = s1[i];
						examinado[i] = true;
        					fin = false;
					}
					else	nv = -1;
        				break;
				}
		}

		delete [] examinado;

		return nv;
	}

	void Crossover_PMX1::complete(Solution& s) const
	{
		int num = 0;
		int n = s.dimension();
		int j,k;
		bool *escogido = new bool[n];

		for(int i = 0; i < n; i++) escogido[i] = false;

		for(int i = 0; i < n; i++)
		{
			if(s.pos(i) != -1) escogido[s.pos(i)-1] = true;
			else num++;
		}

		j =  k = 0;
		for(int i = 0; i < num; i++)
		{
			while((j < n) && (s.pos(j) != -1)) j++;
			while((k < n) && escogido[k]) k++;

			s.pos(j) = k+1;
		}

		delete [] escogido;
	}

	void Crossover_PMX1::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover_PMX1&  cross)
	{
		 os << "Crossover_PMX (Version 1)." << " Probability: " << cross.probability[0];
		 return os;
	}

	void Crossover_PMX1::RefreshState(const StateCenter& _sc) const
	{
		 _sc.set_contents_state_variable("_crossover_probability",(char *)probability,1,sizeof(float));
	}

	void Crossover_PMX1::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_crossover_probability",(char *)probability,nbytes,length);
	}

	void Crossover_PMX1::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover_PMX1::~Crossover_PMX1()
	{
		delete [] probability;
	}

	//  Mutation: Sub_operator -------------------------------------------------------------

	Mutation::Mutation():Intra_Operator(1)
	{
		probability = new float[1];
	}

	// Simple Inversion Mutation (2-opt version)
	void Mutation::mutate(Solution& sol) const
	{
		Solution aux(sol);
		const int max = sol.dimension();
		float fit = sol.fitness();
	//	float fit_i = fit;
		float fit_a;
		static float prob = 0.5;
		Direction d = sol.pbm().direction();

		for(int i = 0; i < max; i++)
			for(int j = i+1; j < max; j++)
			{
				if(rand01() < prob)
				{
//			int i = rand() % max;
//			int j = rand() % max;				
					aux.routes().invert(i,j);
					fit_a = aux.fitness();
					if( ((d == minimize) && (fit_a < fit))
			 	 	 || ((d == maximize) && (fit_a > fit)))
					{
						fit = fit_a;
						sol = aux;
					}
/*					else if(rand01() < 0.1)
					{
						fit = fit_a;
						sol = aux;
					}*/
					else aux = sol;
				}
			}
	}

	void Mutation::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i<sols.size();i++)
			if(rand01() <= probability[0])	mutate(*sols[i]);
	}

	ostream& operator<< (ostream& os, const Mutation&  mutation)
	{
		os << "Mutation." << " Probability: " << mutation.probability[0];
		return os;
	}

	void Mutation::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	void Mutation::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_mutation_probability",(char *)probability,1,sizeof(probability));
	}

	void Mutation::UpdateFromState(const StateCenter& _sc)
	{
		unsigned long nbytes,length;
		_sc.get_contents_state_variable("_mutation_probability",(char *)probability,nbytes,length);
	}

	Mutation::~Mutation()
	{
		delete [] probability;
	}

        //  Crossover_PMX2:Intra_operator -------------------------------------------------------------

	Crossover_PMX2::Crossover_PMX2():Intra_Operator(2)
	{
		probability = new float[1];
	}

	// PMX : Partially-Mapped Crossover (version 1.0.2)
	void Crossover_PMX2::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		int i;

		// Copy old solutions
		Rarray<int> aux1(sol1.dimension());
		aux1=sol1.routes();

		Rarray<int> aux2(sol2.dimension());
		aux2=sol2.routes();

		// new1[i] = a -> the old value a+1 in sol1 is equivalent al new value i+1
		Rarray<int> new1(sol1.dimension());
		Rarray<int> new2(sol2.dimension());

		int limit2=rand_int(1,sol1.dimension()-1);
		int limit1=rand_int(0,limit2-1);

		for (i = 0; i < sol1.dimension(); i++)
		{
			new1[i] = -1;
			new2[i] = -1;
		}

		for (i = limit1; i < limit2; i++)
		{
			sol2.pos(i) = aux1[i];
			sol1.pos(i) = aux2[i];
			new1[aux2[i]-1] = aux1[i]-1;
			new2[aux1[i]-1] = aux2[i]-1;
		}

		for (i = 0; i < limit1; i++)
		{
			sol1.pos(i) = newValue(aux1[i],new1);
			sol2.pos(i) = newValue(aux2[i],new2);
		}

		for (i = limit2; i < sol1.dimension(); i++)
		{
			sol1.pos(i) = newValue(aux1[i],new1);
			sol2.pos(i) = newValue(aux2[i],new2);
		}

		complete(sol1);
		complete(sol2);

	}

	// Auxiliar function for PMX (version 1.0.2)
	int Crossover_PMX2::newValue(const int oldValue,const Rarray<int> & n) const
	{
		int nv = oldValue-1;
		bool *examinado = new bool[n.size()];

		for(int i = 0; i < n.size(); i++) examinado[i] = false;


		while (n[nv] != -1)
		{
			if(examinado[nv])
			{
				nv -2;
				break;
			}
			examinado[nv] = true;
			nv = n[nv];
		}

		delete [] examinado;

		return nv+1;
	}

	void Crossover_PMX2::complete(Solution& s) const
	{
		int num = 0;
		int n = s.dimension();
		int j,k;
		bool *escogido = new bool[n];

		for(int i = 0; i < n; i++) escogido[i] = false;

		for(int i = 0; i < n; i++)
		{
			if(s.pos(i) != -1) escogido[s.pos(i)-1] = true;
			else num++;
		}

		j =  k = 0;
		for(int i = 0; i < num; i++)
		{
			while((j < n) && (s.pos(j) != -1)) j++;
			while((k < n) && escogido[k]) k++;

			s.pos(j) = k+1;
		}

		delete [] escogido;
	}

	void Crossover_PMX2::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover_PMX2&  cross)
	{
		 os << "Crossover_PMX (Version 2)." << " Probability: " << cross.probability[0];
		 return os;
	}

	void Crossover_PMX2::RefreshState(const StateCenter& _sc) const
	{
		 _sc.set_contents_state_variable("_user_op_probability0",(char *)probability,1,sizeof(float));
	}

	void Crossover_PMX2::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_user_op_probability0",(char *)probability,nbytes,length);
	}

	void Crossover_PMX2::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover_PMX2::~Crossover_PMX2()
	{
		delete [] probability;
	}

        //  Crossover_OX:Intra_operator -------------------------------------------------------------

	Crossover_OX::Crossover_OX():Intra_Operator(3)
	{
		probability = new float[1];
	}

	// Order Crossover (OX)
	void Crossover_OX::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		int i;
		int j1,j2;
		const int max = sol1.dimension();

		// Copy old solutions
		Rarray<int> aux1(max);
		aux1=sol1.routes();
		Rarray<int> aux2(max);
		aux2=sol2.routes();

		// esta?[i] = true  if i+1 is in segement or false in otherwise
		bool *esta1 = new bool[max];
		bool *esta2 = new bool[max];

		int limit2=rand_int(1,max-1);
		int limit1=rand_int(0,limit2-1);

		for (i = 0; i < max; i++)
		{
			esta1[i] = false;
			esta2[i] = false;
		}

		for (i = limit1; i < limit2; i++)
		{
			sol1.pos(i) = aux1[i];
			sol2.pos(i) = aux2[i];
			esta1[aux1[i]-1] = true;
			esta2[aux2[i]-1] = true;
		}

		j1 = j2 = i = limit2;

		if((limit1 != 0) || (limit2 != max))
		{
			while( i != limit1 )
			{
				while( esta1[aux2[j1]-1])
					j1 = (j1 + 1) % max;

				sol1.pos(i) = aux2[j1];
				esta1[aux2[j1]-1] = true;
				j1 = (j1 + 1) % max;

				while( esta2[aux1[j2]-1])
					j2 = (j2 + 1) % max;

				sol2.pos(i) = aux1[j2];
				esta2[aux1[j2]-1] = true;
				j2 = (j2 + 1) % max;

				i = (i + 1) % max;
			}
		}

		delete [] esta1;
		delete [] esta2;

	}

	void Crossover_OX::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover_OX&  cross)
	{
		 os << "Crossover_OX." << " Probability: " << cross.probability[0];
		 return os;
	}

	void Crossover_OX::RefreshState(const StateCenter& _sc) const
	{
		 _sc.set_contents_state_variable("_user_op_probability1",(char *)probability,1,sizeof(float));
	}

	void Crossover_OX::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_user_op_probability1",(char *)probability,nbytes,length);
	}

	void Crossover_OX::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover_OX::~Crossover_OX()
	{
		delete [] probability;
	}

        //  Crossover_CX:Intra_operator -------------------------------------------------------------

	Crossover_CX::Crossover_CX():Intra_Operator(4)
	{
		probability = new float[1];
	}

	// Cycle Crossover (CX)
	void Crossover_CX::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		Solution *s1 = crossAux(sol1,sol2);
		Solution *s2 = crossAux(sol2,sol1);

		for (int i = 0; i < sol1.dimension(); i++)
		{
			sol1.pos(i) = s1->pos(i);
			sol2.pos(i) = s2->pos(i);
		}

		delete s1;
		delete s2;
	}

	// Auxiliar function for CX
	Solution *Crossover_CX::crossAux(Solution &sol1,Solution &sol2) const
	{
		Solution *s = new Solution(sol1);
		int max = sol1.dimension();
		bool *elegidos = new bool[max];
		int *pos1 = new int[max];
		int *pos2 = new int[max];
		bool fin = false;
		int elemento;
		int parent;

		if((!s) || (!elegidos) || (!pos1) || (!pos2))
			show_message(7);

	// pos?[i] = j -> si en la solucion? el cliente i+1 se visita en j-ésimo lugar.
		for (int i = 0; i < max; i++)
		{
			s->pos(i) = 0;
			elegidos[i] = false;
			pos1[sol1.pos(i)-1] = i;
			pos2[sol2.pos(i)-1] = i;
		}

		parent = 0;
		for (int i = 0 ; i < max; i++)
		{
			if(!elegidos[i])
			{
				s->pos(i) = (parent == 0?sol1.pos(i):sol2.pos(i));
				elemento = s->pos(i)-1;
				elegidos[i] = true;
				do
				{
					int pos = ((1-parent)==0?pos1[elemento]:pos2[elemento]);
                                	fin = elegidos[pos];
                                	if(!fin)
                                	{
						s->pos(pos) = (parent==0?sol1.pos(pos):sol2.pos(pos));
						elemento = s->pos(pos)-1;
						elegidos[pos] = true;
                                	}

				} while(!fin);
				parent = 1 - parent;
			}
		}

		delete [] elegidos;
		delete [] pos1;
		delete [] pos2;

		return s;
	}

	void Crossover_CX::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover_CX&  cross)
	{
		 os << "Crossover_CX." << " Probability: " << cross.probability[0];
		 return os;
	}

	void Crossover_CX::RefreshState(const StateCenter& _sc) const
	{
		 _sc.set_contents_state_variable("_user_op_probability2",(char *)probability,1,sizeof(float));
	}

	void Crossover_CX::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_user_op_probability2",(char *)probability,nbytes,length);
	}

	void Crossover_CX::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover_CX::~Crossover_CX()
	{
		delete [] probability;
	}

        //  Crossover_ER:Intra_operator -------------------------------------------------------------

	Crossover_ER::Crossover_ER():Intra_Operator(5)
	{
		probability = new float[1];
	}

	// Genetic Edge Recombination Crossover (ER)
	void Crossover_ER::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		Solution *s1 = crossAux(sol1,sol2);
		Solution *s2 = crossAux(sol2,sol1);

		for (int i = 0; i < sol1.dimension(); i++)
		{
			sol1.pos(i) = s1->pos(i);
			sol2.pos(i) = s2->pos(i);
		}

		delete s1;
		delete s2;
	}

	Solution *Crossover_ER::crossAux(Solution& sol1,Solution &sol2) const
	{
		const int max = sol1.dimension();

		int **map = new int * [max];
		int **pos = new int * [max];
		int *count_map = new int [max];
		int *count_pos = new int [max];
		Solution *s = new Solution(sol1);

		// Inicializacion del Mapa y sus estructuras auxiliares:
		//  map[i] : contiene los vecinos de i
		//  count_map[i] : número de vecinos de i
		//  pos[i] : contiene los vecinos de i
		//  count_pos[i] : número de vecinos de i
		//  pos y map se borran de diferente manera.

		if((!pos) || (!map) || (!count_map) || (!count_pos)) show_message(7);

		for (int i = 0; i < max; i++)
		{
			map[i] = new int [4];
			if(!map[i]) show_message(7);
			count_map[i] = 0;

			pos[i] = new int [4];
			if(!pos[i]) show_message(7);
			count_pos[i] = 0;

		}

		create_map(map,count_map,pos,count_pos, sol1,sol2);

		// Elegimos la ciudad current inicial

		int cur,ind;

		ind = 0;
		cur = sol1.pos(0)-1;

		while(ind < max)
		{
			s->pos(ind)= cur + 1;

			// Borramos todos los map[i][j] = current (auxiliados de pos)
			remove_of_map(cur,map,count_map,pos,count_pos);

			// Tiene vecinos
			if(count_map[cur] == 0)	cur = choose_random(count_pos,max); // sino elegimos aleatoriamente
			else cur = choose_minimum(cur,map[cur],count_map); // si tiene el que menos vecinos tenga

			ind++;
		}

		for(int i = 0; i < max; i++)
		{
			delete [] pos[i];
			delete [] map[i];
		}
		delete [] map;
		delete [] count_map;
		delete [] pos;
		delete [] count_pos;

		return s;
	}

	void Crossover_ER::create_map(int **m,int *c_m,int **p, int *c_p,Solution &s1, Solution s2) const
	{
		const int max = s1.dimension()-1;
		int act,sig;

		sig = s1.pos(0);

		m[sig-1][0] = s1.pos(max);

		for(int i = 0; i < max;i++)
		{
			act = sig;
			sig = s1.pos(i+1);

			m[act-1][1] = sig;
			m[sig-1][0] = act;
			c_m[act-1] = 2;
		}

		m[s1.pos(max)-1][1] = s1.pos(0);
		c_m[s1.pos(max)-1] = 2;

		sig = s2.pos(0);
		if(!is_in(s2.pos(max), m[sig-1], c_m[sig-1]))
		{
			m[sig-1][c_m[sig-1]] = s2.pos(max);
			c_m[sig-1] += 1;
		}

		for(int i = 0; i < max;i++)
		{
			act = sig;
			sig = s2.pos(i+1);

			if(!is_in(sig,m[act-1],c_m[act-1]))
			{
				m[act-1][c_m[act-1]] = sig;
				c_m[act-1] += 1;
			}

			if(!is_in(act,m[sig-1],c_m[sig-1]))
			{
				m[sig-1][c_m[sig-1]] = act;
				c_m[sig-1] += 1;
			}
		}

		if(!is_in(s2.pos(0),m[s2.pos(max)-1],c_m[s2.pos(max)-1]))
		{
			m[s2.pos(max)-1][c_m[s2.pos(max)-1]] = s2.pos(0);
			c_m[s2.pos(max)-1] +=1;
		}

		for(int i = 0; i < max+1;i++)
		{
			p[i][0] = m[i][0];
			p[i][1] = m[i][1];
			p[i][2] = m[i][2];
			p[i][3] = m[i][3];
			c_p[i] = c_m[i];
		}
	}

	void Crossover_ER::remove_of_map(int c, int **m,int *c_m, int **p, int *c_p) const
	{
		for(int i = 0; i < c_p[c]; i++)
		{
			remove_pos(c,p[c][i]-1,m,c_m);
		}
		c_p[c] = 0;
	}

	int Crossover_ER::choose_random(int *c,int size) const
	{
		int pos = rand_int(0,size-1);

		for(int i = pos; i < size; i++)
			if(c[i] != 0) return i;

		for(int i = 0; i < pos; i++)
			if(c[i] != 0) return i;

		return 0;
	}

	int Crossover_ER::choose_minimum(int p,int *m,int *c) const
	{
		int min = m[0]-1;

		for(int i = 1; i < c[p]; i++)
			if(c[min] >= c[m[i]-1]) min = m[i]-1;

		return min;
	}

	bool Crossover_ER::is_in(int p,int *m,int size) const
	{
		for(int i=0; i < size; i++)
			if(m[i] == p) return true;

		return false;
	}

	void Crossover_ER::remove_pos(int e,int l,int **m,int *c_m) const
	{
		int i = 0;

		while(m[l][i] != (e+1)) i++;

		for(int j = i; j < (c_m[l] - 1); j++)
			m[l][j] = m[l][j+1];
		c_m[l]--;
	}

	void Crossover_ER::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i+1<sols.size();i=i+2)
		 	if (rand01()<=probability[0]) cross(*sols[i],*sols[i+1]);
	}

	ostream& operator<< (ostream& os, const Crossover_ER&  cross)
	{
		 os << "Crossover_ER." << " Probability: " << cross.probability[0];
		 return os;
	}

	void Crossover_ER::RefreshState(const StateCenter& _sc) const
	{
		 _sc.set_contents_state_variable("_user_op_probability3",(char *)probability,1,sizeof(float));
	}

	void Crossover_ER::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_user_op_probability3",(char *)probability,nbytes,length);
	}

	void Crossover_ER::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover_ER::~Crossover_ER()
	{
		delete [] probability;
	}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return ((double)solver.best_cost_trial() == pbm.best_cost());
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

