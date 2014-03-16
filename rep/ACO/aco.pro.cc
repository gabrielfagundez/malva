/***************************************************************************
			 Ant Colony System v1.10 
			 developed by Guillermo Daniel Ordóñez
			 email gordonez@unsl.edu.ar
****************************************************************************/

#include "aco.hh"

skeleton ACO
{
// -+----1----+----2----+----3----+----4----+----5---- (Funciones Auxiliares)


    inline double random_double(double _min, double _max) // [_min,_max)
	{
	    return _min + (_max-_min) * drand48();
	}
    
    inline int random_int   (int _min, int _max)  // [_min,_max)
	{
	    return (int) (_min + (_max-_min) * drand48());
	}
    struct individual
    {
	int index;
	double fitness;
    };
    
    static int __direction;
    int compare (const void * a, const void * b)
	{
	    struct individual * _a = (struct individual*) a;
	    struct individual * _b = (struct individual*) b;
	    if ((_a->fitness * __direction) >(_b->fitness * __direction)) return -1;
	    if ((_a->fitness * __direction) <(_b->fitness * __direction)) return +1;
	    return 0;
	}
    
    int *getBestK(const Rarray<Solution*> &sols, const double * fitness, int &k)
	{
	    __direction=sols[0]->pbm().direction();
	    int i,_size=k=k<sols.size()?k:sols.size();
	    struct individual *values =  (struct individual*) malloc(sizeof(struct individual)*sols.size());
	    if (!fitness)
		for (i=0;i<sols.size();i++)  values[values[i].index=i].fitness = sols[i]->fitness();
	    else
		for (i=0;i<sols.size();i++)  values[values[i].index=i].fitness = fitness[i];
	    qsort(values,sols.size(),sizeof(struct individual),compare);
	    int * answer = (int*) malloc(sizeof(int)*(_size));
	    for (i=0;i<_size;i++)   answer[i]=values[i].index;
	    free(values);
	    return answer;
	}

    int *getWorstK(const Rarray<Solution*> &sols, int &k)
	{
	    __direction=sols[0]->pbm().direction()*-1;
	    int i,_size=k=k<sols.size()?k:sols.size();
	    struct individual *values =  (struct individual*) malloc(sizeof(struct individual)*sols.size());
	    for (i=0;i<sols.size();i++)  values[values[i].index=i].fitness = sols[i]->fitness();
	    qsort(values,sols.size(),sizeof(struct individual),compare);
	    int * answer = (int*) malloc(sizeof(int)*(_size));
	    for (i=0;i<_size;i++)   answer[i]=values[i].index;
	    free(values);
	    return answer;
	}

// -+----1----+----2----+----3----+----4----+----5----+----6----+----7- (Ant)
    Ant::Ant(const Problem& pbm):
	_pbm(pbm)
	{
	    int i;
	    _L=new int[pbm.max_path_length()];
	    _string= new char[sizeof(double)+sizeof(int)*(3+(sizeof(int)*pbm.max_path_length()))];  
	    _L_count=0;
	};
    
    
    Ant::Ant(const class Ant &ant):
	_pbm(ant.problem())
	{
	    int i;
	    _L=new int[_pbm.max_path_length()];
	    _string= new char[sizeof(double)+sizeof(int)*(3+(sizeof(int)*_pbm.max_path_length()))];
	    _L_count=ant._L_count;
 	    for(int i=0;i<_pbm.max_path_length();i++) {
		_L[i]=ant._L[i];
	    }  
	};
    
    NetStream& operator<< (NetStream& ns, const Ant &ant)
	{
	    int i;
	    ns << ant._L_count;
	    for(i=0;i<ant._L_count;i++)
		ns << ant._L[i];
	}
    NetStream& operator>> (NetStream& ns, Ant &ant)
	{
	    int i,tmp;
	    ns >> ant._L_count;
	    for(i=0;i<ant._L_count;i++) 
		ns >> ant._L[i];
	}		
    
    ostream& operator<< (ostream& os, const Ant &ant)
	{
	    int i;
	    os << ant._L_count << " ";
	    for(i=0;i<ant._L_count;i++)
		os << ant._L[i] << " ";
	}
    
    istream& operator>> (istream& is, Ant &ant)
	{
	    int i,tmp;
	    is >> ant._L_count;
	    for(i=0;i<ant._L_count;i++)
		is >> ant._L[i];
	}

    Ant::~Ant(){
	free(_string);
	free(_L);
   };

    const class Problem& Ant::problem() const {
	return _pbm;
    };

    int Ant::operator[](int pos)
	{
	    return _L[pos];
	};

    int Ant::size() const
	{
	    return _L_count;
	};


    void Ant::move_to_node(int from, int through)
	{
	    _L[_L_count++]=through;
	};

   
    Ant& Ant::operator=  (const Ant& ant)
    {
	int i;
	_L_count=ant._L_count;
	for(int i=0;i<_L_count;i++)
	    _L[i]=ant._L[i];
    }; 
    
    bool Ant::operator== (const Ant& ant) const
    {
	int i;
	if(_L_count!=ant._L_count) return false;
	for(int i=0;i<_L_count;i++)
	    if(_L[i]!=ant._L[i]) return false;
	return true;
    };
    
    bool Ant::operator!= (const Ant& ant) const
    {
	return !((*this)==ant);
    };
    
    char *Ant::to_String() const
	{
	    int i;
	    int * dst=(int *)_string;
	    dst[0]=_L_count;
	    for(int i=0;i<_L_count;i++)
		dst[i+1]=_L[i];
	    return _string;
	};
    
    void Ant::to_Ant(char *cadena)
	{
	    int i;
	    int * src=(int *)cadena;
	    _L_count=src[0];
	    for(i=0;i<_L_count;i++)
		_L[i]=src[i+1];
	};
    
    int Ant::string_size() const 
	{
	    return (_L_count+1)*sizeof(int);
	}; 
    
// -+----1----+----2----+----3----+----4----+----5----+----6----+---- (Trail)

    Trail::Trail(const class Problem& pbm, const class SetUpParams& setup):
	_pbm(pbm)
	{}
    Trail::~Trail()
	{}
    void Trail::print(ostream& os) const
	{}
    ostream& operator<< (ostream& os, const Trail &trail)
	{
	    int i;
	    trail.print(os);
	}
    void Trail::set_best_fitness(double fitness)
	{}
// -+----1----+----2----+----3----+----4----+----5----+----6----+--- (STrail)
    
    STrail::STrail(const class Problem& pbm, const class SetUpParams& setup):
	Trail(pbm,setup),
	_t0(setup.t0()),
	_xi(setup.xi()),
	_a(setup.a()),
	_rho(setup.rho()),
	_min_max(setup.min_max()),
	_pheromonas(new Rarray<Rarray<double> *>(_pbm.get_node_count()))
	{
	    switch (_min_max){
		case 0: _max=infinity();   _min=0;break;
		case 1: _max=setup.max();  _min=_max/_a;break;
		case 2: _max=infinity();   _min=0;break;
		default: 
		    cout <<endl << "Error: min-max methos must be in [0,2]" << endl; 
		    exit(1);
	    }
	    int i;
	    for (i=0;i<_pbm.get_node_count();i++)
		(*_pheromonas)[i]= new Rarray<double>(_pbm.path_count_from(i));
	    reset();
	};

    double STrail::c_inv(double fitness)
	{ 
	    if (_pbm.direction()==maximize)		    
		return fitness;
	    return 1.0/fitness;
	    
	}
     void STrail::print(ostream& os) const
	{
	    os << _pbm.get_node_count();
	    for (int i=0;i<_pbm.get_node_count();i++)
	    {
		os << " " << _pbm.path_count_from(i);
		for (int j=0;j<_pbm.path_count_from(i);j++)
		    os << " " << (*(*_pheromonas)[i])[j];
	    }
	    os << endl;
	}

    STrail::~STrail()
	{
	    int i;
	    for (i=0;i<_pheromonas->size();i++)
		delete (*_pheromonas)[i];
	    delete _pheromonas;
	}

    STrail::STrail(const class Problem& pbm, const class SetUpParams& setup, STrail * src):
	Trail(pbm,setup),
	_pheromonas(new Rarray<Rarray<double> *>(_pbm.get_node_count()))
	{
	    _t0=src->_t0;
	    _xi=src->_xi;
	    _a=src->_a;
	    _rho=src->_rho;
	    _min_max=src->_min_max;
	    _max=src->_max;
	    _min=src->_min;
	    int i,j;
	    for (i=0;i<_pbm.get_node_count();i++)
	    {
		(*_pheromonas)[i]= new Rarray<double>(_pbm.path_count_from(i));
		for (j=0;j<_pbm.path_count_from(i);j++){
		    (*(*_pheromonas)[i])[j]=(*(*src->_pheromonas)[i])[j];
		}
	    }
	};

    void STrail::update(Solution * sol, double c1, double c2)
	{
	    int i,through;
	    double Delta=c_inv(sol->fitness());
	    Ant * ant = sol->ant();
	    int node=0;
	    for (i=0;i<ant->size();i++)
	    {
		through=(*ant)[i];
		Rarray<double> * aux=(*_pheromonas)[node];
		node = _pbm.get_destiny(node,through);
		double &T=(*aux)[through];
		T=c1*T+c2*Delta;
		if (T<_min) T=_min;
		if (T>_max) T=_max; 
	    }
	}

    void STrail::update(int from, int through)
	{
	    double &T=(*(*_pheromonas)[from])[through];
	    T=(1-_xi)*T+_xi*_t0;
	    if (T<_min) T=_min;
	    if (T>_max) T=_max; 
	}

    void STrail::reset()
	{
	    int i,j;
	    double value;
	    if (_min==0) value=1.0/_pbm.get_node_count();
	    else value = _min;
	    for(i=0;i<_pbm.get_node_count();i++)
		for(j=0;j<_pbm.path_count_from(i);j++)
		    (*(*_pheromonas)[i])[j]=value;
	}
    
    void STrail::evaporate(double e)
	{
	    int i,j;
	    if (e>0)
	    {
		for(i=0;i<_pbm.get_node_count();i++)
		    for(j=0;j<_pbm.path_count_from(i);j++)
		    {
			double &T=(*(*_pheromonas)[i])[j];
			T*=e;
			if (T<_min) T=_min;			
		    }
	    }
	}
    
    double STrail::get_value (int from, int through)
	{
	    return (*(*_pheromonas)[from])[through];
	}
    
    void STrail::set_best_fitness(double fitness)
	{
	    int i,j;
	    if (_min_max!=0) return;
	    double _c_inv=c_inv(fitness);
	    _max=_c_inv/_rho;
	    _min=_max/_a;
	    for (i=0;i<_pheromonas->size();i++)
		for(j=0;j<(*_pheromonas)[i]->size();j++){
		    if ((*(*_pheromonas)[i])[j]<_min) (*(*_pheromonas)[i])[j]=_min;
		    if ((*(*_pheromonas)[i])[j]>_max) (*(*_pheromonas)[i])[j]=_max;
		}
	}
// -+----1----+----2----+----3----+----4----+----5----+----6----+--- (MTrail)
    
    MTrail::MTrail(const class Problem& pbm, const class SetUpParams& setup, const Colony * col):
	Trail(pbm,setup),
	_pheromonas( new Rarray<Rarray<double> *>(_pbm.get_node_count())),
	_max_pop_count(setup.trail_pop_size()),
	_pi(setup.pi()),
	_p1(setup.p1()),
	_worst_fitness(pbm.direction()*infinity()),
	_pop_count(0),
	_rm(setup.rm()),
	_first(0),
	_col(col)
	{
	    int i;
	    _sol=new Rarray<Solution *>(_max_pop_count);
	    for (i=0;i<_max_pop_count;i++) (*_sol)[i]=new Solution(_pbm);
	    for (i=0;i<_pbm.get_node_count();i++)
		(*_pheromonas)[i]= new Rarray<double>(_pbm.path_count_from(i));
	    reset();
	};
    void MTrail::print(ostream& os) const
	{
	    os << _pbm.get_node_count();
	    for (int i=0;i<_pbm.get_node_count();i++)
	    {
		os << " " << _pbm.path_count_from(i);
		for (int j=0;j<_pbm.path_count_from(i);j++)
		    os << " " << (*(*_pheromonas)[i])[j];
	    }
	    os << endl;
	}

    MTrail::~MTrail()
	{
	    int i;
	    for (i=0;i<_max_pop_count;i++)
		delete (*_sol)[i];
	    delete _sol;
	    for (i=0;i<_pheromonas->size();i++)
		delete (*_pheromonas)[i];
	    delete _pheromonas;
	}

    MTrail::MTrail(const class Problem& pbm, const class SetUpParams& setup, const Colony * col, MTrail * src):
	Trail(pbm,setup),
	_pheromonas( new Rarray<Rarray<double> *>(_pbm.get_node_count())),
	_col(col)
	{
	    int i,j;
	    _max_pop_count=src->_max_pop_count;
	    _pi=src->_pi;
	    _p1=src->_p1;
	    _worst_fitness=src->_worst_fitness;
	    _pop_count=src->_pop_count;
	    _rm=src->_rm;
	    _first=src->_first;
	    _sol=new Rarray<Solution *>(_max_pop_count);
	    for (i=0;i<_max_pop_count;i++) (*_sol)[i]=new Solution(_pbm);
	    for (i=0;i<_pop_count;i++) *(*_sol)[i]=*(*src->_sol)[i];
	    for (i=0;i<_pbm.get_node_count();i++)
	    {
		(*_pheromonas)[i]= new Rarray<double>(_pbm.path_count_from(i));
		for (j=0;j<_pbm.path_count_from(i);j++)
		{
		    (*(*_pheromonas)[i])[j]=(*(*src->_pheromonas)[i])[j];
		}	    
	    };
	};

    void MTrail::update(Solution * sol, double c1, double c2)
	{
	    double fitness=sol->fitness();
	    if (_pop_count<_max_pop_count)
	    {
		*(*_sol)[(_first+_pop_count++)%_max_pop_count]=*sol;
		_update(sol->ant(),1);
		if (fitness*_pbm.direction()< _worst_fitness*_pbm.direction())
		    _worst_fitness = fitness;
		return;
	    }

	    switch (_rm){
		case 0:
		    if (fitness*_pbm.direction()> _worst_fitness*_pbm.direction())
		    {
			int *_worst=getWorstK(*_sol,_pop_count); 
			_update((*_sol)[_worst[0]]->ant(),-1);
			*(*_sol)[_worst[0]]=*sol;
			_worst_fitness= (*_sol)[_worst[1]]->fitness();
			_update(sol->ant(),1);
			if (fitness*_pbm.direction()< _worst_fitness*_pbm.direction())
			    _worst_fitness = fitness;
			delete _worst;
			return;
		    };
		    break;
		case 1:
		    _update((*_sol)[_first]->ant(),-1);
		    *(*_sol)[_first]=*sol;
		    _first=(_first+1)%_max_pop_count;
		    _update(sol->ant(),1);
		    break;
		default: break;
	    };
	}
    
    void MTrail::_update(Ant * ant, double dir)
	{   
	    int i,through;
	    double Delta=dir*_p1;
	    int node=0;
	    for (i=0;i<ant->size();i++)
	    {
		through=(*ant)[i];
		Rarray<double> * aux=(*_pheromonas)[node];
		node = _pbm.get_destiny(node,through);
		double &T=(*aux)[through];
		T=T+Delta;
	    }
	}
    void MTrail::update(int from, int to)
	{
	}
    void MTrail::evaporate(double e)
	{
	}
    void MTrail::reset()
	{
	    int i,j;
	    for(i=0;i<_pbm.get_node_count();i++)
		for(j=0;j<_pbm.path_count_from(i);j++)
		    (*(*_pheromonas)[i])[j]=_pi;
	    for (i=0;i<_pop_count;i++)
		_update((*_sol)[i]->ant(),+1);
	}
    double MTrail::get_value (int from, int to)
	{
	    return (*(*_pheromonas)[from])[to];
	}

// -+----1----+----2----+----3----+----4----+----5----+----6----+---- (Colony)
    Trail * new_trail(const Problem& pbm,  const SetUpParams& setup, const Colony * col)
	{
	    Trail *_trail;
	    switch (setup.trail())
	    {
	    	case 1: 
		    _trail = new STrail(pbm,setup); break;
	    	case 2: 
		    _trail = new MTrail(pbm,setup,col); break;
	    	default: 
		    cout << "Error: Trail should be in [1,3]"<< endl;
		    cout << "\t1 - one dimension trail (better for subset-like problems)" << endl;
		    cout << "\t1 - two dimension trail (standar)" << endl;
		    cout << "\t2 - one dimension population base aco" << endl; 
		    cout << "\t2 - standar population base aco" << endl; exit(1);
	    };
	    return _trail;

	};

    Trail * new_trail(const Problem& pbm,  const SetUpParams& setup, const Colony * col, Trail * src)
	{
	    Trail *_trail;
	    switch (setup.trail())
	    {
	    	case 1: 
		    _trail = new STrail(pbm,setup, (STrail *) src); break;
	    	case 2: 
		    _trail = new MTrail(pbm,setup, col, (MTrail *) src); break;
	    };
	    return _trail;
	};




    Colony::Colony(const Problem& pbm,const SetUpParams& setup):
	_problem(pbm),
	_setup(setup),
	nb_steps(setup.nb_evolution_steps()),
	_best_solution(* new Solution(pbm)),
	_global_best_solution(* new Solution(pbm)),
	_global_best_cost(-1 * pbm.direction() * infinity()),
	_rank(setup.rank()),
	_elitist(setup.elitist()), 
	_rho(setup.rho()),
	_w(setup.w()),
	_check_convergency(setup.check_convergency()),
	_q0(setup.q0()),
	_alpha(setup.alpha()),
	_beta(setup.beta()),
	_e1(setup.e1()),
	_e2(setup.e2())
	
	{
	    int i;
	    
	    _sols=new Rarray<Solution*>(setup.colony_size());
	    _fitness_values=new Rarray<double>(setup.colony_size());
	    _eta=new Eta(pbm,setup);
	    _trail=new_trail(pbm,setup,this);
	    
	    generate_solution(&_best_solution);
	    _best_cost = _worst_cost =_best_solution.fitness();
	    for (i=0;i<_sols->size();i++)
		(*_sols)[i]=new Solution(pbm);
	}
    
    Colony::~Colony()
	{	
	    for (int i=0;i< _sols->size();i++)
		delete (*_sols)[i];
	    delete _sols;
	    delete _eta;
	    delete _fitness_values;
	    delete _trail;
	    delete &_best_solution;
	    delete &_global_best_solution;
	}
    
    ostream& operator<< (ostream& os, const Colony& colony)
	{
	    os << "---------------------------------------------------------------" << endl;
	    os << "        Solutions        " << endl << endl;
	    
	    for (int i=0;i<colony._sols->size();i++)
		os << "Solution "<<i<<": "<<(*colony._sols)[i] << endl;
	    
	    os << endl << "---------------------------------------------------------------" << endl;
	    return os;
	}
    
    istream& operator>> (istream& is, Colony& colony)
	{
	    return is;
	};
    
    Colony& Colony::operator= (const Colony& colony)
    {
	int i;
	nb_steps=colony.nb_steps;

	for (i=0;i< _sols->size();i++)
	    delete (*_sols)[i];
	delete _sols;
	delete _fitness_values;

	_sols=new Rarray<Solution*>(colony._sols->size());
	_fitness_values= new Rarray<double>(colony._sols->size());
	for (i=0;i<colony._sols->size();i++){
	    (*_sols)[i]=new Solution(*(*colony._sols)[i]);
	    (*_fitness_values)[i]=(*colony._fitness_values)[i];
	}
	delete _trail;

	_trail =new_trail(_problem,_setup,this, colony._trail);

	_start_trial_time=colony._start_trial_time;
	_best_solution_time=colony._best_solution_time;
	_rank=colony._rank;
	_elitist=colony._elitist;
	_rho=colony._rho;
	_w=colony._w;
	_check_convergency=colony._check_convergency;
	_q0=colony._q0;
	_alpha=colony._alpha;
	_beta=colony._beta;
	_e1=colony._e1;
	_e2=colony._e2;
	
	
	*_eta=*colony._eta;
	_best_solution=colony._best_solution; 
	_best_cost=colony._best_cost;
	_worst_cost=colony._worst_cost;

	_global_best_solution=colony._global_best_solution; 
	_global_best_cost=colony._global_best_cost;
	_global_best_solution_time=colony._global_best_solution_time;
	return (*this);
    }
    
    const SetUpParams& Colony::setup() const
	{
	    return _setup;
	}
    
    void Colony::initialize()
	{
	    generate_solution(&_best_solution);
	    _best_cost = _worst_cost =_best_solution.fitness();
	}
    
    void Colony::advance()
	{
	    int i;
	    
	    _best_cost=-1 * _problem.direction() * infinity();
	    _worst_cost=-1 * _best_cost;
	    
	    // generación de soluciones
	    for(i=0;i<_sols->size();i++)
	    {
		// genera una solución
		generate_solution((*_sols)[i]);
		(*_fitness_values)[i]=(*_sols)[i]->fitness();
		
		// 
		if ((*_fitness_values)[i]*_problem.direction()> _best_cost*_problem.direction())
		{
		    _best_cost=(*_fitness_values)[i];
		    _best_solution=*(*_sols)[i];
		    _best_solution_time=_used_time();
		};
		if ((*_fitness_values)[i]*_problem.direction()<_worst_cost*_problem.direction())
		    _worst_cost=(*_fitness_values)[i];
	    }
	}
    
    void Colony::daemon()
        {
	    int i,k=_sols->size();
	    int * solution_index= getBestK(*_sols, (const double *) NULL,k);
	    Solution ** solution= (Solution **) malloc(sizeof(Solution *)*(k+1));
	    Solution ** _tmp=solution;

            Solution *_local_best= (*_sols)[solution_index[0]];
            if (_local_best->fitness()*_problem.direction()> _global_best_cost*_problem.direction())
	    {
		_global_best_cost=_best_cost;
		_global_best_solution=_best_solution;
		_global_best_solution_time=_best_solution_time;
	    };
	    solution[0]=&_global_best_solution; // trial_best
	    
	    //solution[0]=&_best_solution; // step_best
	    _trail->set_best_fitness(_best_cost);
	    for (i=0;i<k;i++) 
		solution[i+1]=(*_sols)[solution_index[i]];	    

	    if (solution[1]->fitness()*_problem.direction()> solution[1]->fitness()*_problem.direction())
		solution[0]=solution[1];


	    if (!_elitist) _tmp++;
	    // Evaporación de feromonas
	    _trail->evaporate(_e1);

	    // Actualización del rastro
	    for (i=_w-1;i>=0;i--)
		_trail->update(_tmp[i],1-_e2,_rho*(1-_rank*(_w-i-1)));
	    if (_check_convergency && test_convergency(*this))
	    {
		_trail->reset();
		_best_solution_time=_used_time();
	    }
	    delete solution;
	    delete solution_index;
        }

   
    Solution &Colony::best_solution() const
	{
	    return _best_solution;
	};
    
    void Colony::generate_solution(Solution* sol)
	{	    
	    Ant *ant=new Ant(_problem);

	    double *b;

	    Neighborhood *N =new Neighborhood(ant,_problem, _setup);
	    sol->ant(ant);
	    
	    _eta->reset(ant,N);
	    int r=0;

	    while (N->size()>0 && ant->size()<_problem.max_path_length())
	    {
		double sum=0;
		int s,i;	       
		b = new double[N->size()];
		for (i=0;i<N->size();i++)
		{
		    double x= pow(_trail->get_value(r,(*N)[i]),alpha())*
			      pow(_eta->get_value(r,(*N)[i]),beta());
		    b[i]= x;
		    sum=sum+x;
		}
		if ( random_double(0,1) < _q0){
		    s=0;
		    for(i=0;i<N->size();i++)
			if (b[i]>b[s]) s=i;
		    s=(*N)[s];
		} else {
		    double R=random_double(0,sum);
		    for(i=0;i<N->size()-1&&b[i]<R;i++,R-=b[i]);
		    s=(*N)[i];
		}
		ant->move_to_node(r,s);
		N->move_to_node(r,s);
		_eta->move_to_node(r,s);
		_trail->update(r,s);
		r=_problem.get_destiny(r,s);
		delete b;
	    };
	    delete N;
	};
   
    // interchange solutions between colony
    void Colony::interchange(const unsigned long current_generation, NetStream& channel)
	{
	    for (int i=0;i<_setup.inter_operators_size();i++)
		_setup.pool().inter_operator(_setup.inter_operator_index(i)).execute((*this),current_generation,channel,_setup.synchronized(),_setup.check_asynchronous());
	};
    
    Solution& Colony::solution(const unsigned int index) const
	{ 
	    return *(*_sols)[index];
	}
    
    Trail& Colony::trail() const
	{
	    return *_trail;
	}
    
    double Colony::fitness(const unsigned int index) const
	{
	    return (*_fitness_values)[index];
	}

    double Colony::best_cost() const
	{
	    return _best_cost;
	}

    double Colony::worst_cost() const
	{
	    return _worst_cost;
	}

    const Problem & Colony::problem() const
	{
	    return _problem;
	}
    float Colony::start_trial_time() const
	{
	    return _start_trial_time;
	}
    float Colony::best_solution_time() const
	{
	    return _best_solution_time;
	}

    double Colony::rank() const
	{
	    return _rank;
	}
    bool Colony::elitist() const
	{
	    return _elitist;
	}
    double Colony::rho() const
	{
	    return _rho;
	}
    unsigned int Colony::w() const
	{
	    return _w;
	}
    bool Colony::check_convergency() const
	{
	    return _check_convergency;
	}
    double Colony::q0() const
	{
	    return _q0;
	}
    double Colony::alpha() const
	{
	    return _alpha;
	}
    double Colony::beta() const
	{
	    return _beta;
	}
    Rarray<Solution*> &Colony::solutions() const
	{
	    return *_sols;
	}  
    
// -+----1----+----2----+----3----+----4----+----5----+----6----+ (Statistics)

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
		   << " Generation: " << stats.stats_data[i].nb_steps
		   << " Current best cost: " << stats.stats_data[i].best_cost
		   << " Global best cost: " << stats.stats_data[i].global_best_cost;
		
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
/*
           // Esta funcion no debe ir comentada si se quiere usar estos datos!!!!
            struct stat *new_stat=(struct stat *)malloc(sizeof(struct stat));
	    
	    new_stat->trial=solver.current_trial();
	    new_stat->nb_steps=solver.current_step();
	    new_stat->best_cost=solver.current_best_cost();
	    new_stat->global_best_cost=solver.global_best_cost();
	    
	    stats_data.append(*new_stat);
*/	}
    
    void Statistics::clear()
	{
	    stats_data.remove();
	}
    
    Statistics::~Statistics()
	{}
    
// -+----1----+----2----+----3----+----4----+----5----+----6---- (SetUpParams)

    SetUpParams::SetUpParams (Operator_Pool& pool):
	_independent_runs(0),
	_nb_evolution_steps(0),
	_colony_size(0),
	_display_state(0),

	_refresh_global_state(1),
	_synchronized(0),
	_check_asynchronous(1),

	_trail(0),
	_trail_pop_size(20),
	_pi(1),
	_p1(0.04),

	_rho(.5),
	_w(1),
	_elitist(1),
	_a(0),
	_rm(0),
	_max(infinity()),
	_min_max(0),
	_rank(0),
	_check_convergency(0),
	_q0(0),
	_xi(0),
	_t0(0),

	_alpha(1),
	_beta(5),

	_eta(),
	_neighborhood(),
	_inter_operators(),
	_pool(pool)
	{
	}
    
    Operator_Pool& SetUpParams::pool() const
	{
	    return _pool;
	}
    
    istream& operator>> (istream& is, SetUpParams& setup)
	{
	    char buffer[MAX_BUFFER]; // current line in the setup file
	    char command[50];
	    long op;
	    double op2;
	    int parameter;
	    short int nb_section=0;
	    short int nb_tp = 0;
	    short int nb_selection = 0;
	    short int nb_param=0;
	    short int nb_paramMT=0;
	    short int nb_paramST=0;
	    short int nb_LAN_param=0;
	    short int nb_neighborhood=0;
	    char * tmp;

	    
	    while (is.getline(buffer,MAX_BUFFER,'\n'))
	    {
		sscanf(buffer," %s ",command);
		if (!(strcmp(command,"General"))) nb_section=0;
		if (!(strcmp(command,"STrail"))) nb_section=1;
		if (!(strcmp(command,"MTrail"))) nb_section=2;
		if (!(strcmp(command,"Eta"))) nb_section=3;
		if (!(strcmp(command,"Neighborhood"))) nb_section=4;
		if (!(strcmp(command,"Inter-Operators"))) nb_section=5;
		if (!(strcmp(command,"LAN-configuration"))) nb_section=6;
		op=-1;
		op2=-1;
		sscanf(buffer," %lf ",&op2);
		if (op2<0) continue;
		sscanf(buffer," %d ",&op);
		switch (nb_section)
		{
		    case 0: 
			switch (nb_param)
			{
			    case 0: setup.independent_runs(op); break;
			    case 1: setup.nb_evolution_steps(op); break;
			    case 2: setup.colony_size(op); break;
			    case 3: setup.trail(op); break;
			    case 4: setup.elitist(op); break;
			    case 5: setup.w(op); break;
			    case 6: setup.rank(op); break;
			    case 7: setup.check_convergency(op); break;
			    case 8: setup.q0(op2); break;
			    case 9: 
				double alpha, beta;
				sscanf(buffer,"%lf %lf", &alpha, &beta);
				setup.alpha(alpha);
				setup.beta(beta);			
				break;
			    case 10: setup.display_state(op); break;
			}
			nb_param++;
			break;
		    case 1:
			switch (nb_paramST)
			{
			    case 0: setup.t0(op2); break;
			    case 1: setup.xi(op2); break;
			    case 2: setup.rho(op2); break;
			    case 3: 
				int min_max;
				double a, max;
				sscanf(buffer,"%d %lf %lf", &min_max, &a, &max);
				setup.min_max(min_max);
				setup.a(a);
				setup.max(max);
				break;
			    case 4: setup.e1(op2); break;
			    case 5: setup.e2(op2); break;
			}
			nb_paramST++;			
			break;
		    case 2:
			switch (nb_paramMT)
			{
			    case 0: setup.trail_pop_size(op); break;
			    case 1: setup.pi(op2); break;
			    case 2: setup.p1(op2); break;
			    case 3: setup.rm(op); break;
			}
			nb_paramMT++;			
			break;
		    case 3: 
			tmp=new char[strlen(buffer)+1];
			strcpy(tmp,buffer);
			
			setup.eta(tmp); 
			break;
		    case 4: 
			tmp=new char[strlen(buffer)+1];
			strcpy(tmp,buffer);
			
			setup.neighborhood(tmp); 
			break;
		    case 5: 
			setup._inter_operators.append(new unsigned int(op));
			setup.pool().inter_operator(op).setup(buffer);
			break;
		    case 6: if (nb_LAN_param>=3) break;
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
	    int i;

	    os << "#Configuration --------------------------------------------"<< endl<< endl
	       << "General" << endl
	       << " " << setup.independent_runs()    << "\t# Independent runs " << endl
	       << " " << setup.nb_evolution_steps()  << "\t# Steps " << endl
	       << " "  <<  setup.colony_size()       << "\t# Colony size" << endl
	       << " " << setup.trail()               << "\t# Trail kind (1:STrail / 2:MTrail)" << endl
	       << " " << setup.elitist()             << "\t# elitist" << endl
	       << " " << setup.w()                   << "\t# w (number of solutions send to the trail/step)" << endl   
	       << " " << setup.rank()                << "\t# rank" << endl
	       << " " << setup.check_convergency()   << "\t# check convergency" << endl
	       << " " << setup.q0()                  << "\t# q0" << endl
	       << " " << setup.alpha() << " " << setup.beta() << "# alpha beta " << endl
	       << " " << setup.display_state() << "#display state" << endl;
	    if (setup.trail()==1){
		os <<  "STrail" << endl
		   << setup.t0()                      << "\t# t0" << endl
		   << setup.xi()                      << "\t# xi" << endl
		   << setup.rho()                     << "\t# rho" << endl
		   << " " << setup.min_max() << " " << setup.a() << " " << setup.max() << "\t# (0: use dinamyc max, 1: use fixed max, 2:min=0 max=infty) (a:max/min) (max)" << endl
		   << setup.e1()                      << "\t# evaporation method 1 (global)" << endl
		   << setup.e2()                      << "\t# evaporation method 2 (local)" << endl;
	    }
	    if (setup.trail()==2){
		os <<  "STrail" << endl
		   << setup.trail_pop_size()                << "\t# pop size" << endl
		   << setup.pi()                      << "\t# pi" << endl
		   << setup.p1()                      << "\t# p1" << endl
		   << setup.rm()                      << "\t# replace method (0: worst, 1:oldest)" << endl;
	    }
	    os << "Eta"<< endl;
	    for (i=0;i<setup.eta().size();i++)
		os<< (&(setup.eta()[i])) << endl;
	    os << "Neighborhood"<< endl;
	    for (i=0;i<setup.neighborhood().size();i++)
		os<< (&(setup.neighborhood()[i])) << endl;
	    os << "Inter-Operators" << endl;	
	    for (int i=0;i<setup.inter_operators_size();i++)
		os << " "  << i << "  " << setup.pool().inter_operator(setup.inter_operator_index(i)) << endl;
	    os << "LAN-configuration" << endl
	       << " " << setup.refresh_global_state() << "# Refresh global state in number of generations"<< endl
	       << " " << setup.synchronized()         << "# 1 - synchronous mode/ 2 - asynchronous mode" << endl
	       << " " << setup.check_asynchronous()   << "#Interval for checking asynchronous receptions" << endl;
	    os << endl << endl << "#End configuration -------------------------------------------" << endl << endl;
	    return os;
	}
    
    const unsigned int    SetUpParams::independent_runs() const
	{
	    return _independent_runs;
	}
    
    const unsigned long    SetUpParams::nb_evolution_steps() const
	{
	    return _nb_evolution_steps;
	}
    
    const unsigned int SetUpParams::colony_size() const
	{
	    return _colony_size;
	}
    
    const bool  SetUpParams::display_state() const
	{
	    return _display_state;
	}

    const unsigned long SetUpParams::refresh_global_state() const
	{
	    return _refresh_global_state;
	}
    
    const bool SetUpParams::synchronized() const
	{
	    return _synchronized;
	}
    
    const unsigned int SetUpParams::check_asynchronous() const
	{
	    return _check_asynchronous;
	}
    const double  SetUpParams::alpha() const
	{
	    return _alpha;
	}
 
   const double  SetUpParams::beta() const
	{
	    return _beta;
	}

    const unsigned int  SetUpParams::trail() const
	{
	    return _trail;
	}
    const unsigned int  SetUpParams::trail_pop_size() const
	{
	    return _trail_pop_size;
	}
    const double  SetUpParams::pi() const 
	{
	    return _pi;
	}
    const double  SetUpParams::p1() const 
	{
	    return _p1;
	}
    const double  SetUpParams::rho() const 
	{
	    return _rho;
	}
    const double  SetUpParams::e1() const 
	{
	    return _e1;
	}
    const double  SetUpParams::e2() const 
	{
	    return _e2;
	}
    const unsigned int SetUpParams::w() const 
	{
	    return _w;
	}
    const bool SetUpParams::elitist() const 
	{
	    return _elitist;
	}
    const double SetUpParams::a() const
	{
	    return _a;
	}
    const double SetUpParams::max() const
	{
	    return _max;
	}
    const unsigned int SetUpParams::min_max() const
	{
	    return _min_max;
	}
    const unsigned int SetUpParams::rm() const
	{
	    return _rm;
	}
    const bool SetUpParams::check_convergency() const 
	{
	    return _check_convergency;
	}
    const double  SetUpParams::q0() const
	{
	    return _q0;
	}
    const double  SetUpParams::xi() const 
	{
	    return _xi;
	}
    const double  SetUpParams::t0() const 
	{
	    return _t0;
	}
    const double SetUpParams::rank()const
	{
	    return _rank;
	}

    const Rlist<char>& SetUpParams::eta() const 
	{
	    return _eta;
	}
    const Rlist<char>& SetUpParams::neighborhood() const 
	{
	    return _neighborhood;
	}

    void SetUpParams::independent_runs(const unsigned int val)
	{
	    _independent_runs=val;
	}
    
    void SetUpParams::nb_evolution_steps(const unsigned long val)
	{
	    _nb_evolution_steps=val;
	}
    
    void SetUpParams::colony_size(const unsigned int val)
	{
	    _colony_size=val;
	}
    
    void SetUpParams::display_state(const bool val)
	{
	    _display_state=val;
	}
    
    void SetUpParams::refresh_global_state(const unsigned long val)
	{
	    _refresh_global_state=val;
	}
    
    void SetUpParams::synchronized(const bool val)
	{
	    _synchronized=val;
	}
    
    void SetUpParams::check_asynchronous(const unsigned int val)
	{
	    _check_asynchronous=val;
	}

    void SetUpParams::alpha(const double val)
	{
	    _alpha=val;
	}

    void SetUpParams::beta(const double val)
	{
	    _beta=val;
	}

    void SetUpParams::trail(const unsigned int val)
	{
	    _trail=val;
	}
    void SetUpParams::trail_pop_size(const unsigned int val)
	{
	    _trail_pop_size=val;
	}
    void SetUpParams::pi(const double val)
	{
	    _pi=val;
	}
    void SetUpParams::p1(const double val)
	{
	    _p1=val;
	}
    void SetUpParams::rho(const double val)
	{
	    _rho=val;
	}
    void SetUpParams::e1(const double val)
	{
	    _e1=val;
	}
    void SetUpParams::e2(const double val)
	{
	    _e2=val;
	}
    void SetUpParams::w(const unsigned int val)
	{
	    _w=val;
	}
    void SetUpParams::elitist(const bool val)
	{
	    _elitist=val;
	}
    void SetUpParams::a(const double val)
	{
	    _a=val;
	}
    void SetUpParams::max(const double val)
	{
	    _max=val;
	}
    void SetUpParams::rm(const unsigned int val)
	{
	    _rm=val;
	}
    void SetUpParams::min_max(const unsigned int val)
	{
	    _min_max=val;
	}
    void SetUpParams::check_convergency(const bool val)
	{
	    _check_convergency=val;
	}
    void SetUpParams::q0(const double val)
	{
	    _q0=val;
	}
    void SetUpParams::xi(const double val)
	{
	    _xi=val;
	}
    void SetUpParams::t0(const double val)
	{
	    _t0=val;
	}
    void SetUpParams::rank(const double val)
	{
	    _rank=val;
	}
    void SetUpParams::eta(char * val)
	{
	    _eta.append(*val);
	}
    void SetUpParams::neighborhood(char * val)
	{
	    _neighborhood.append(*val);
	}

    const unsigned int SetUpParams::inter_operator_index(const unsigned int index) const
	{
	    return _inter_operators[index];
	}
    
    const unsigned int SetUpParams::inter_operators_size() const
	{
	    return _inter_operators.size();
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
    
// -+----1----+----2----+----3----+----4----+----5----+----6- (Inter_Operator)
    Inter_Operator::Inter_Operator(const unsigned int _number_op,const Direction dir):
	_number_operator(_number_op),
	direction(dir),
	migration_rate(1),
	migration_size(1)
	{}
    
    unsigned int Inter_Operator::number_operator() const
	{
	    return _number_operator;
	}
    
    void Inter_Operator::setup(char line[MAX_BUFFER])
	{
	    int op;
	    int new_migration_rate=1;
	    int new_migration_size=1;
	    
	    sscanf(line," %d %d %d ",&op,&new_migration_rate,&new_migration_size);
	    
	    assert(new_migration_rate>0);
	    assert(new_migration_size>0);
	    
	    migration_rate=new_migration_rate;
	    migration_size=new_migration_size;
	}
    
    void Inter_Operator::RefreshState(const StateCenter& _sc) const
	{
	    _sc.set_contents_state_variable("_migration_rate",(char *)&migration_rate,1,sizeof(migration_rate));
	    _sc.set_contents_state_variable("_migration_size",(char *)&migration_size,1,sizeof(migration_size));
	}
    
    void Inter_Operator::UpdateFromState(const StateCenter& _sc)
	{
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_migration_rate",(char *)&migration_rate,nitems,length);
	    _sc.get_contents_state_variable("_migration_size",(char *)&migration_size,nitems,length);
	}
    
    ostream& operator<< (ostream& os, const Inter_Operator& inter)
	{
	    switch (inter.number_operator())
	    {
		case 0: os << (Migration&)inter;break;
	    }
	    return os;
	}
    
    Inter_Operator::~Inter_Operator()
	{}
    
// -+----1----+----2----+----3----+----4----+----5- (Inter_Operator/Migration)
    
    Migration::Migration(const Direction dir):
	Inter_Operator(0,dir),
	_initialized(*new bool(false))
	{}
    
    void Migration::setup(char line[MAX_BUFFER])
	{
	    int op;
	    int new_migration_rate=1;
	    int new_migration_size=1;
	    int new_mode=0;
	    
	    sscanf(line," %d %d %d %d",&op,&new_migration_rate,&new_migration_size,&new_mode);
	    
	    assert(new_migration_rate>0);
	    assert(new_migration_size>0);

	    migration_rate=new_migration_rate;
	    migration_size=new_migration_size;
	    mode=new_mode;
	    to = new Rarray<int> (0);
	    from = new Rarray<int> (0);



	}
    
    void Migration::initialize(NetStream& _netstream) const
	{
	    int nb_proc=_netstream.pnumber();
	    int mypid=_netstream.my_pid();
	    int i;
	    switch (mode){
		case 1: // island		    
		    *to= *new Rarray<int>(1);
		    *from = *new Rarray<int>(1);
		    (*to)[0]  = (mypid + 1) % nb_proc;
		    (*from)[0] = (nb_proc + mypid - 1) % nb_proc;	    
		    if ((*to)[0]==0) (*to)[0]=1;
		    if ((*from)[0]==0) (*from)[0]=nb_proc - 1;  
		    break;
		case 2: // star
		    if (mypid==1){
			*from=*to=*new Rarray<int>(nb_proc - 2);
			for (i=2;i<nb_proc;i++)
			{
			    (*from)[i-2]=i;
			    (*to)[i-2]=i;
			}
		    } else {
			*from=*to= *new Rarray<int>(1);
			(*to)[0]=1;
			(*from)[0]=1;
			}
		    break;
//		    case 3: // add other topology here
		default:
		    cout << "Error: migration mode should be in [1,2]" << endl;
		    exit(1);
	    }
	    _initialized=true;
	}

    void Migration::send(int to,Colony& col,NetStream& _netstream) const
	{	    
//	    cout << endl <<"s " << _netstream.my_pid() << " to " << to << endl;
	    int _size=migration_size;
	    int * _selection=getBestK(col.solutions(),NULL,_size);
	    int from=_netstream.my_pid();
	    _netstream << set_target(to) 
		       << get_target(&to)
		       << set_source(from)
		       << get_source(&from);
	    for (int i=0;i<_size;i++)
	    {
		Solution* solution_to_send = &(col.solution(_selection[i]));
		_netstream << *solution_to_send;
		cout << "p: " << from << " send: " << *solution_to_send << endl;
	    }
	    free (_selection);
	}
    void Migration::reciv(int from,Colony& col,NetStream& _netstream,const bool synchronized) const
	{
//	    cout << endl << "r " << _netstream.my_pid() << " from " << from << endl;
	    int to=_netstream.my_pid();
	    Solution* solution_received;
	    _netstream << set_target(to) 
		       << get_target(&to)
		       << set_source(from)
		       << get_source(&from);
	    if (synchronized)
	    {
		    solution_received=new Solution(col.problem());
		    int _size=migration_size;
		    int *_replace=getWorstK(col.solutions(),_size);
		    for (int i=0;i<migration_size;i++)
		    {
			_netstream << wait(regular);
			_netstream >> *solution_received;
			cout << "p: " << to << " rec: " << *solution_received << endl;
			
			if (col.solution(_replace[i]).fitness() * col.problem().direction() <
			    solution_received->fitness() * col.problem().direction()){
			    col.solution(_replace[i])= *solution_received;
			    cout << "p: " << to << " Storage in " <<_replace[i] << ": "<< col.solution(_replace[i]) << endl;
			}
		    };
		    free(_replace);
		    delete(solution_received);
	    }
	    else
	    {
		int pending=false;
		_netstream._probe(regular,pending);                
		if (pending)
		{
		    int _size=migration_size;
		    solution_received=new Solution(col.problem());
		    int *_replace=getWorstK(col.solutions(),_size);
		    for (int i=0;i<migration_size;i++)
		    {
			pending=false;
			_netstream._probe(regular,pending);
			if (!pending) break;
			_netstream >> *solution_received;
			if (col.solution(_replace[i]).fitness() * col.problem().direction() <
			    solution_received->fitness() * col.problem().direction())
 			    col.solution(_replace[i])= *solution_received;
		    };
		    free(_replace);
		    delete(solution_received);
		}                
	    }
	}
    
    void Migration::execute(Colony& col,const unsigned long current_generation,NetStream& _netstream,const bool synchronized,const unsigned int check_asynchronous) const
	{ 
	    int i;
	    if (!_initialized) initialize(_netstream);
	    if ( (current_generation % migration_rate) == 0
		 && (current_generation!=col.setup().nb_evolution_steps())) 
		for (i=0;i<to->size();i++)
		    send((*to)[i],col,_netstream);
	    
	    if ( ((synchronized &&
		   (current_generation % migration_rate) == 0 && 
		   (current_generation!=col.setup().nb_evolution_steps())) ||
		  (!synchronized && 
		   ((current_generation % check_asynchronous) ==0))))
		for (i=0;i<from->size();i++)
		    reciv((*from)[i],col,_netstream,synchronized);
	}
    
    ostream& operator<< (ostream& os, const Migration& migration)
	{
	    os << migration.migration_rate << " " << migration.migration_size << "#* rate size ";
	    return os;
	}
    
    Migration::~Migration()
	{}
    
    
// -+----1----+----2----+----3----+----4----+----5----+----6-- (StopCondition)
    StopCondition::StopCondition()
	{};
    
    StopCondition::~StopCondition()
	{};

// -+----1----+----2----+----3----+----4----+----5----+----6-- (Operator_Pool)
    Operator_Pool::Operator_Pool(const Problem& pbm)
	{
	    _inter_operators.append(new Migration(pbm.direction()));   	// 0
	}
    
    Inter_Operator& Operator_Pool::inter_operator(const unsigned int index) const
	{
	    assert(index < _inter_operators.size());
	    return _inter_operators[index];
	}
    
    const Rlist<Inter_Operator>& Operator_Pool::inter_operators() const
	{
	    return _inter_operators;
	}
    
    Operator_Pool::~Operator_Pool()
	{}

// Solver (superclasse)---------------------------------------------------

    Solver::Solver (const Problem& pbm, const SetUpParams& setup):
	problem(pbm),
	params(setup),
	_userstat(),
	_stat(),
	_sc(),
	current_colony(pbm,setup),
	best_cost((-1) * pbm.direction() * infinity()),
	worst_cost((-1) * best_cost),
	best_solution(current_colony.best_solution()),
	time_spent_in_trial(0.0),
	total_time_spent(0.0),
	start_trial(0.0),
	start_global(0.0),
	_current_trial("_current_trial",_sc),
	_current_step("_current_step",_sc),
	_current_best_solution("_current_best_solution",_sc),
	_current_best_cost("_current_best_cost",_sc),
	_current_worst_cost("_current_worst_cost",_sc),
	_current_time_spent("_current_time_spent",_sc),
	_best_solution_trial("_best_sol_trial",_sc),
	_best_cost_trial("_best_cost_trial",_sc),
	_worst_cost_trial("_worst_cost_trial",_sc),
	_step_best_found_in_trial("_step_best_found_in_trial",_sc),
	_time_best_found_trial("_time_best_found_trial",_sc),
	_time_spent_trial("_time_spent_trial",_sc),
	_trial_best_found("_trial_best_found",_sc),
	_step_best_found("_step_best_found",_sc),
	_global_best_solution("_global_best_solution",_sc),
	_global_best_cost("_global_best_cost",_sc),
	_global_worst_cost("_global_worst_cost",_sc),
	_time_best_found("_time_best_found",_sc),		  
	_independent_runs("_independent_runs",_sc),
	_trail("_trail",_sc),
	_rho("_rho",_sc),
	_w("_w",_sc),
	_elitist("_elitist",_sc),
	_a("_a",_sc),
	_max("_max",_sc),
	_min_max("_min_max",_sc),
	_rm("_rm",_sc),
	_check_convergency("_check_convergency",_sc),
	_q0("_q0",_sc),
	_xi("_xi",_sc),
	_t0("_t0",_sc), 
	_alpha("_alpha",_sc),
	_beta("_beta",_sc),
	_migration_rate("_migration_rate",_sc),
	_migration_size("_migration_size",_sc),
	_display_state("_display_state",_sc)
	
	{
	    current_trial(0);
	    current_step(0);
	    current_best_solution(best_solution);
	    current_best_cost(best_cost);
	    current_worst_cost(worst_cost);
	    current_time_spent(total_time_spent);
	    best_solution_trial(best_solution);
	    best_cost_trial(best_cost);
	    worst_cost_trial(worst_cost);
	    step_best_found_in_trial(0);
	    time_best_found_trial(time_spent_in_trial);
	    time_spent_trial(time_spent_in_trial);
	    trial_best_found(0);
	    step_best_found(0);
	    global_best_solution(best_solution);
	    global_best_cost(best_cost);
	    global_worst_cost(worst_cost);
	    time_best_found(total_time_spent);
	    independent_runs(setup.independent_runs());
	    trail(setup.trail());
	    rho(setup.rho());
	    w(setup.w());
	    elitist(setup.elitist());
	    a(setup.a());
	    max(setup.max());
	    min_max(setup.min_max());
	    rm(setup.rm());
	    check_convergency(setup.check_convergency());
	    q0(setup.q0());
	    xi(setup.xi());
	    t0(setup.t0());
	    alpha(setup.alpha());
	    beta(setup.beta());


	    migration_rate(0);
	    migration_size(0);


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
    
    unsigned int Solver::independent_runs() const
	{
	    unsigned int value=0;
	    unsigned long nitems,length;
	    _independent_runs.get_contents((char *)&value, nitems, length);
	    return value;
	}
    unsigned int Solver::current_trial() const
	{
	    unsigned int value=0;
	    unsigned long nitems,length;
	    _current_trial.get_contents((char *)&value, nitems, length);
	    return value;
	}
    
    unsigned long Solver::current_step() const
	{
	    unsigned long value=0;
	    unsigned long nitems,length;
	    _current_step.get_contents((char *)&value, nitems, length);
	    return value;
	}
    
    Solution Solver::current_best_solution() const
	{
	    Solution sol(problem);
	    unsigned long nitems,length;
	    char data_stored[_current_best_solution.get_nitems() * _current_best_solution.get_length()];
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
	    char data_stored[_best_solution_trial.get_nitems() * _best_solution_trial.get_length()];
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
    
    unsigned int Solver::step_best_found_in_trial() const
	{
	    unsigned int value=0;
	    unsigned long nitems,length;
	    _step_best_found_in_trial.get_contents((char *)&value, nitems, length);
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
    
    unsigned int Solver::step_best_found() const
	{
	    unsigned int value=0;
	    unsigned long nitems,length;
	    _step_best_found.get_contents((char *)&value, nitems, length);
	    return value;
	}
    
    Solution Solver::global_best_solution() const
	{
	    Solution sol(problem);
	    char data_stored[_global_best_solution.get_nitems() * _global_best_solution.get_length()];
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
  

    unsigned int Solver::trail()  const
	{
	    unsigned int value=1;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_trail",(char *)&value,nitems,length);
	    return value;
	}
  
    double Solver::rho() const
	{
	    double value=0.5;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_rho",(char *)&value,nitems,length);
	    return value;
	}
    
    unsigned int Solver::w()  const
	{
	    unsigned int value=1;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_w",(char *)&value,nitems,length);
	    return value;
	}
    
    bool Solver::elitist() const
	{
	    bool value=true;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_elitist",(char *)&value,nitems,length);
	    return value;
	}
    
    double Solver::a() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_a",(char *)&value,nitems,length);
	    return value;
	}
    double Solver::max() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_max",(char *)&value,nitems,length);
	    return value;
	}
    unsigned int Solver::rm()  const
	{
	    unsigned int value=1;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_rm",(char *)&value,nitems,length);
	    return value;
	}
    unsigned int Solver::min_max()  const
	{
	    unsigned int value=1;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_min_max",(char *)&value,nitems,length);
	    return value;
	}
     bool Solver::check_convergency() const
	{
	    bool value=true;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_check_convergency",(char *)&value,nitems,length);
	    return value;
	}
	    
    
    double Solver::q0() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_q0",(char *)&value,nitems,length);
	    return value;
	}

    double Solver::xi() const 
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_xi",(char *)&value,nitems,length);
	    return value;
	}

    double Solver::t0() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_t0",(char *)&value,nitems,length);
	    return value;
	}
    
     double Solver::alpha() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_alpha",(char *)&value,nitems,length);
	    return value;
	}
     double Solver::beta() const
	{
	    double value=0.0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_beta",(char *)&value,nitems,length);
	    return value;
	}

    unsigned int Solver::migration_rate() const
	{
	    unsigned int rate=0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_migration_rate",(char *)&rate,nitems,length);
	    return rate;
	}
    
    unsigned int Solver::migration_size() const
	{
	    unsigned int size=0;
	    unsigned long nitems,length;
	    _sc.get_contents_state_variable("_migration_size",(char *)&size,nitems,length);
	    return size;
	}
    
    void Solver::independent_runs(const unsigned int value)
	{
	    _independent_runs.set_contents((char *)&value,1,sizeof(int));
	}

    void Solver::current_trial(const unsigned int value)
	{
	    _current_trial.set_contents((char *)&value,1,sizeof(int));
	}
    
    void Solver::current_step(const unsigned long value)
	{
	    _current_step.set_contents((char *)&value,1,sizeof(long));
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
    
    void Solver::step_best_found_in_trial(const unsigned int value)
	{
	    _step_best_found_in_trial.set_contents((char *)&value,1,sizeof(int));
	}
    
    void Solver::time_best_found_trial(const float value)
	{
	    _time_best_found_trial.set_contents((char *)&value,1,sizeof(float));
	}
      
    void Solver::time_spent_trial(const float value)
	{
	    _time_spent_trial.set_contents((char *)&value,1,sizeof(float));
	}
    
   
    void Solver::display_state(const int value)
	{
	    _display_state.set_contents((char *)&value,1,sizeof(float));
	}
    
    void Solver::trial_best_found(const unsigned int value)
	{
	    _trial_best_found.set_contents((char *)&value,1,sizeof(int));
	}
    
    void Solver::step_best_found(const unsigned int  value)
	{
	    _step_best_found.set_contents((char *)&value,1,sizeof(int));
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
   
    void Solver::trail(unsigned int value)
	{
	    _sc.set_contents_state_variable("_trail",(char *)&value,1,sizeof(unsigned int));
	}

    void Solver::rho(double value)
	{
	    _sc.set_contents_state_variable("_rho",(char *)&value,1,sizeof(double));
	}
    void Solver::w(unsigned int value)
	{
	    _sc.set_contents_state_variable("_w",(char *)&value,1,sizeof(unsigned int));
	}
    void Solver::elitist(bool value)
	{
	    _sc.set_contents_state_variable("_elitist",(char *)&value,1,sizeof(bool));
	}
    void Solver::a(double value)
	{
	    _sc.set_contents_state_variable("_a",(char *)&value,1,sizeof(double));
	}
    void Solver::max(double value)
	{
	    _sc.set_contents_state_variable("_max",(char *)&value,1,sizeof(double));
	}
    void Solver::min_max(unsigned int value)
	{
	    _sc.set_contents_state_variable("_min_max",(char *)&value,1,sizeof(unsigned int));
	}
    void Solver::rm(unsigned int value)
	{
	    _sc.set_contents_state_variable("_rm",(char *)&value,1,sizeof(unsigned int));
	}
    void Solver::check_convergency(bool value)
	{
	    _sc.set_contents_state_variable("_check_convergency",(char *)&value,1,sizeof(bool));
	}
    void Solver::q0(double value)
	{
	    _sc.set_contents_state_variable("_q0",(char *)&value,1,sizeof(double));
	}
    void Solver::xi(double value)
	{
	    _sc.set_contents_state_variable("_xi",(char *)&value,1,sizeof(double));
	}
    void Solver::t0(double value)
	{
	    _sc.set_contents_state_variable("_t0",(char *)&value,1,sizeof(double));
	}
    void Solver::alpha(double value)
	{
	    _sc.set_contents_state_variable("_alpha",(char *)&value,1,sizeof(double));
	}
    void Solver::beta(double value)
	{
	    _sc.set_contents_state_variable("_beta",(char *)&value,1,sizeof(double));
	}

    void Solver::migration_rate(const unsigned int rate)
	{
	    _sc.set_contents_state_variable("_migration_rate",(char *)&rate,1,sizeof(int));
	}
    
    void Solver::migration_size(const unsigned int size)
	{
	    _sc.set_contents_state_variable("_migration_size",(char *)&size,1,sizeof(int));
	}
    
    Statistics& Solver::statistics()
	{
	    return _stat;
	}
    
    UserStatistics& Solver::userstatistics()
	{
	    return _userstat;
	}
    
    const Colony& Solver::colony() const
	{
	    return current_colony;
	}
    
    const SetUpParams& Solver::setup() const
	{
	    return params;
	}
    
    const Problem& Solver::pbm() const
	{
	    return problem;
	}
    
    void Solver::KeepHistory(const Solution& best_sol,const double best_cost,const double worst_cost,const float time_spent_in_trial,const float total_time_spent)
	{
	    bool betterG=false;
	    bool worseG=false;
	    bool betterT=false;
	    bool worseT=false;
	    
	    switch (problem.direction())
	    {
		case minimize: betterG = (best_cost < global_best_cost() || (best_cost == global_best_cost() && time_spent_in_trial < time_best_found()));
		    worseG  = (worst_cost > global_worst_cost());
		    betterT = (best_cost < best_cost_trial() || (best_cost == best_cost_trial() && time_spent_in_trial < time_best_found_trial()));
		    worseT  = (worst_cost > worst_cost_trial());
		    break;
		case maximize: betterG = (best_cost > global_best_cost() || (best_cost == global_best_cost() && time_spent_in_trial < time_best_found()));
		    worseG  = (worst_cost < global_worst_cost());
		    betterT = (best_cost > best_cost_trial() || (best_cost == best_cost_trial() && time_spent_in_trial < time_best_found_trial()));
		    worseT  = (worst_cost < worst_cost_trial());
		    break;
	    }

	    if (betterT)
	    {
		best_solution_trial(best_sol);
		best_cost_trial(best_cost);
		time_best_found_trial(time_spent_in_trial);
		step_best_found_in_trial(current_step());
	    }
	    if (betterG)
	    {
		global_best_solution(best_sol);
		global_best_cost(best_cost);
		time_best_found(time_spent_in_trial);
		trial_best_found(current_trial());
		step_best_found(current_step());
	    }
	    
	    if (worseT)
	    {
		worst_cost_trial(worst_cost);
	    }
	    if (worseG)
		global_worst_cost(worst_cost);
	}
    
    StateCenter *Solver::GetState()
	{
	    return &_sc;
	}
    
    void Solver::RefreshState()
	{
	    current_best_solution(best_solution);
	    current_best_cost(best_cost);
	    current_worst_cost(worst_cost);
	    current_time_spent(total_time_spent);
	    time_spent_trial(time_spent_in_trial);
	    KeepHistory(best_solution,best_cost,worst_cost,time_spent_in_trial,total_time_spent);
	}
 

    
    void Solver::RefreshCfgState()
	{
	    for (int i=0;i<params.pool().inter_operators().size();i++)
		params.pool().inter_operator(i).RefreshState(_sc);
	    params.RefreshState(_sc);
	}
    
    void Solver::UpdateFromState()
	{
	    best_solution=current_best_solution();
	    best_cost=current_best_cost();
	    worst_cost=current_worst_cost();
	    total_time_spent=current_time_spent();
	    time_spent_in_trial=time_spent_trial();
	    KeepHistory(best_solution,best_cost,worst_cost,time_spent_in_trial,total_time_spent);
	}
    void Solver::UpdateFromCfgState()
	{
	    for (int i=0;i<params.pool().inter_operators().size();i++)
		params.pool().inter_operator(i).UpdateFromState(_sc);
	    params.UpdateFromState(_sc);
	}
    
    void Solver::show_state() const
	{
	    cout << endl << "Current State ---------------------------------------------" << endl;
	    cout<< endl << "[Configuration]";
	    cout<< endl << "independent runs: " << (int) params.independent_runs();
	    cout<< endl << "steps: " << (int) params.nb_evolution_steps();
	    cout<< endl << "colony size: "  << (int) params.colony_size();
	    cout<< endl << "trail kind: " << (int) params.trail();
	    cout<< endl << "elitist: " << (int) params.elitist();
	    cout<< endl << "w: " << (int) params.w();
	    cout<< endl << "rank: " << (int) params.rank();
	    cout<< endl << "check_convergency: " << (int) params.check_convergency();
	    cout<< endl << "q0:  " << (double) params.q0();
	    cout<< endl << "alpha: " << (double) params.alpha();
	    cout<< endl << "beta: " << (double) params.beta();
	    if ((int) params.trail()==1){
		cout<< endl << "t0 " << (double) params.t0();
		cout<< endl << "xi: " << (double) params.xi();
		cout<< endl << "rho: " << (double) params.rho();
		cout<< endl << "min max method: " << (double) params.min_max();
		cout<< endl << "a: " << (double) params.a();
		cout<< endl << "max: " << (double) params.max();
		cout<< endl << "evaporation (as-like): "<< (double) params.e1();
		cout<< endl << "evaporation (aco-like): "<< (double) params.e2();
	    };
	    if ((int) params.trail()==2){
		cout<< endl << "Trail pop size " << (double) params.trail_pop_size();
		cout<< endl << "pi " << (double) params.pi();
		cout<< endl << "p1 " << (double) params.p1();
		cout<< endl << "repleace method " << (double) params.rm();
	    };
	    cout<< endl << "migration_rate: " << (int) migration_rate();
	    cout<< endl << "migration_size: " << (int) migration_size();
	    cout<< endl<< endl << "[Current]";
	    cout<< endl <<  "trial: " << (int) current_trial();
	    cout<< endl <<  "step: " << (int) current_step();
	    cout<< endl <<  "best cost: " << (double) current_best_cost();
	    cout<< endl <<  "worst cost: " << (double) current_worst_cost();
	    cout<< endl<< endl <<  "[Trial]";
	    cout<< endl << "best cost trial: " << best_cost_trial();
	    cout<< endl << "worst cost trial: " << worst_cost_trial();
	    cout<< endl << "step best found in trial: " << step_best_found_in_trial();
	    cout<< endl << "time best found trial: " << time_best_found_trial();
	    cout<< endl << "time spent in trial: " << time_spent_trial();
	    cout<< endl<< endl << "[Global:]";
	    cout<< endl << "global best cost: " << global_best_cost();
	    cout<< endl << "global worst cost: " << global_worst_cost();
	    cout<< endl << "trial best found: " << trial_best_found();
	    cout<< endl << "step best found: " << step_best_found();
	    cout<< endl << "time best found: " << time_best_found() << endl;
	    cout<< endl << "current time spent (so far): " << current_time_spent() << endl;
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
	    Colony col(problem,params);
	    col.initialize();
	    StartUp(col);
	}
    
    void Solver_Seq::StartUp(const Colony& col)
	{
	    start_trial=_used_time();
	    start_global=total_time_spent;
	    
	    current_trial(current_trial()+1);
	    current_step(0);
	    
	    current_colony=col;
	    time_spent_in_trial=0.0;


	    step_best_found_in_trial(0);
	    time_best_found_trial(time_spent_in_trial);

	    
	    // gets current interesting values in the current colony
	    
	    best_cost=current_colony.best_cost();
	    best_solution= current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();
   
	    best_solution_trial(best_solution);
	    best_cost_trial(best_cost);
	    worst_cost_trial(worst_cost);

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
	    
	    current_step(current_step()+1);
	    
	    current_colony.advance();
	    current_colony.daemon();
	    // gets current interesting values in the current colony
	      	    
	    best_cost=current_colony.best_cost();
	    best_solution=current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();
	    
	    time_spent_in_trial = _used_time(start_trial);
	    total_time_spent    = start_global + time_spent_in_trial;
	    
	    // refresh state with these values
	    RefreshState();
	    RefreshCfgState();
	    
	    if( (current_step() % params.refresh_global_state()) == 0)
			UpdateFromCfgState();
	    
	    
	    _stat.update(*this);
	    _userstat.update(*this);
	    
	    if (display_state())
			show_state();
	}
    
    void Solver_Seq::run ()
	{
	    while (current_trial() < params.independent_runs() )// && !(terminateQ(problem,*this,params)))
	    run(params.nb_evolution_steps());
	}
    
    void Solver_Seq::run (const unsigned long int nb_steps)
	{
	    StartUp();
	    while ((current_step() < nb_steps)  && !(terminateQ(problem,*this,params)))
		DoStep();   
    
	}
    
    void Solver_Seq::run (const Colony& col,const unsigned long int nb_steps)
	{
	    StartUp(col);
	    while ((current_step() < nb_steps) && !(terminateQ(problem,*this,params)))
			DoStep();
	    
	}

    
    // Solver LAN ------------------------------------------------------------
    
    Solver_Lan::Solver_Lan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv):
	_best_solution_trial(pbm),
	Solver(pbm,setup),_netstream()
	{
	    NetStream::init(argc,argv);
	    mypid=_netstream.my_pid();
	    random_seed(time(0) + (mypid+1));
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
	    Colony col(problem,params);
	    col.initialize();
	    StartUp(col);
	}
    
    void Solver_Lan::StartUp(const Colony& col)
	{
	    int i;
	    _netstream << barrier;
	    
	    start_trial=_used_time();
	    start_global=total_time_spent;
	    
	    _end_trial=false;

	    current_trial(current_trial()+1);
	    current_step(0);
	    
	    current_colony=col;

	    time_spent_in_trial=0.0;


	    step_best_found_in_trial(0);
	    time_best_found_trial(time_spent_in_trial);
	    best_solution_trial(current_colony.best_solution());
	    best_cost_trial(current_colony.best_cost());
	    worst_cost_trial(current_colony.worst_cost());

	    // gets current interesting values in the current colony
	    
	    best_cost=current_colony.best_cost();
	    best_solution= current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();


	    time_spent_trial(0.0);
    	    if (mypid!=0)
	    {
			current_colony=col;

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
	    current_step(current_step()+1);
	    current_colony.advance();
	    current_colony.interchange(current_step(),_netstream);
	    current_colony.daemon();
	    // gets current interesting values in the current colony
	    
	    best_cost=current_colony.best_cost();
	    best_solution=current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();

	    time_spent_in_trial = _used_time(start_trial);
	    total_time_spent    = start_global + time_spent_in_trial;
	    
	    // refresh state with these values
	    RefreshState();
	    RefreshCfgState();
	    
	    // in this step i have to send data about my local state to the global state
	    if ((int)current_step() % params.refresh_global_state() ==0)
	    {
		send_local_state_to(mypid);
		UpdateFromCfgState();
	    }
	    _stat.update(*this);
	    _userstat.update(*this);   
	}
    

    void Solver_Lan::send_local_state_to(int _mypid)
	{
	    _netstream << set_target(0);
	    _netstream << pack_begin;
	    _netstream       << _mypid;
	    _netstream       << current_trial();
	    _netstream       << current_step();
	    _netstream       << best_cost_trial();
	    _netstream       << best_solution_trial();
	    _netstream       << step_best_found_in_trial();
	    _netstream       << time_best_found_trial();
	    _netstream       << worst_cost_trial();
	    _netstream       << current_best_cost();
	    _netstream       << current_best_solution();
	    _netstream       << current_worst_cost();
	    _netstream << pack_end;
	}
    
    int Solver_Lan::receive_local_state()
	{
	    int r_pid=0;	   
	
	    _netstream._wait(packed);

	    _netstream << pack_begin;
	    _netstream >> r_pid;
	    _netstream >> _current_trial;
	    _netstream >> _current_step;
	    _netstream >> _best_cost_trial;
	    _netstream >> _best_solution_trial;
	    _netstream >> _step_best_found_in_trial;
	    _netstream >> _time_best_found_in_trial;
	    _netstream >> _worst_cost_trial;
	    _netstream >> best_cost;
	    _netstream >> best_solution;
	    _netstream >> worst_cost;
	    _netstream << pack_end;

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
		current_step(_step_best_found_in_trial);
		KeepHistory(_best_solution_trial,_best_cost_trial,_worst_cost_trial,_time_best_found_in_trial,start_global + _time_best_found_in_trial);
		current_step(_current_step);
		
		time_spent_in_trial = _used_time(start_trial);
		total_time_spent    = start_global + time_spent_in_trial;
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
	    while (current_trial() < params.independent_runs() ) //&& !(terminateQ(problem,*this,params)))
		run(params.nb_evolution_steps());
	}
    
    void Solver_Lan::run (const unsigned long int nb_steps)
	{
	    StartUp();
	    if (mypid!=0)
	    {
		while ((current_step() < nb_steps) && !(terminateQ(problem,*this,params)))
		{
		    DoStep();
		}
		send_local_state_to(-1);
	    }
	    else
	    {
		check_for_refresh_global_state();
	    }
	    
	    _netstream << barrier;
	    reset();
	}
    
    void Solver_Lan::run (const Colony& col,const unsigned long int nb_steps)
	{
	    StartUp(col);
	    if (mypid!=0)
	    {
		while ((current_step() < nb_steps) && !(terminateQ(problem,*this,params)))
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
    
    // Solver WAN ------------------------------------------------------------
    
    Solver_Wan::Solver_Wan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv):
	_best_solution_trial(pbm),
	Solver(pbm,setup),_netstream()
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
	    Colony col(problem,params);
	    col.initialize();
	    StartUp(col);
	}
    
    void Solver_Wan::StartUp(const Colony& col)
	{
	    int i;
	    _netstream << barrier;
	    
	    start_trial=_used_time();
	    start_global=total_time_spent;
	    
	    _end_trial=false;

	    current_trial(current_trial()+1);
	    current_step(0);
	    
	    current_colony=col;
	    time_spent_in_trial=0.0;


	    step_best_found_in_trial(0);
	    time_best_found_trial(time_spent_in_trial);
	    best_solution_trial(current_colony.best_solution());
	    best_cost_trial(current_colony.best_cost());
	    worst_cost_trial(current_colony.worst_cost());
	    
	    // gets current interesting values in the current colony
	    
	    best_cost=current_colony.best_cost();
	    best_solution= current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();


	    time_spent_trial(0.0);
	    
	    if (mypid!=0)
	    {
	
		// refresh state with these values
		RefreshState();
		RefreshCfgState();
		
		send_local_state_to(mypid);
		
		_stat.update(*this);
		_userstat.update(*this);
		
	    }
	}
    
    void Solver_Wan::DoStep()
	{
	    current_step(current_step()+1);
	    current_colony.advance();
	    current_colony.interchange(current_step(),_netstream);
	    current_colony.daemon();
	    
	    // gets current interesting values in the current colony
	    
	    best_cost=current_colony.best_cost();
	    best_solution=current_colony.best_solution();
	    worst_cost=current_colony.worst_cost();

	    time_spent_in_trial = _used_time(start_trial);
	    total_time_spent    = start_global + time_spent_in_trial;
	    
	    // refresh state with these values
	    RefreshState();
	    RefreshCfgState();
	    
	    // in this step i have to send data about my local state to the global state
	    if ((int)current_step() % params.refresh_global_state() ==0)
	    {
		send_local_state_to(mypid);
		UpdateFromCfgState();
	    }
	    
	    _stat.update(*this);
	    _userstat.update(*this);
	    
	    // if (display_state()) show_state();
	}
    

    void Solver_Wan::send_local_state_to(int _mypid)
	{
	    _netstream << set_target(0);
	    _netstream << set_target(0);
	    _netstream << pack_begin;
	    _netstream       << _mypid;
	    _netstream       << current_trial();
	    _netstream       << current_step();
	    _netstream       << best_cost_trial();
	    _netstream       << best_solution_trial();
	    _netstream       << step_best_found_in_trial();
	    _netstream       << time_best_found_trial();
	    _netstream       << worst_cost_trial();
	    _netstream       << current_best_cost();
	    _netstream       << current_best_solution();
	    _netstream       << current_worst_cost();
	    _netstream     << pack_end;
	}
    
    int Solver_Wan::receive_local_state()
	{
	    int r_pid=0;
	    
	    _netstream._wait(packed);
	    
	    _netstream << pack_begin;
	    _netstream >> r_pid;
	    _netstream >> _current_trial;
	    _netstream >> _current_step;
	    _netstream >> _best_cost_trial;
	    _netstream >> _best_solution_trial;
	    _netstream >> _step_best_found_in_trial;
	    _netstream >> _time_best_found_in_trial;
	    _netstream >> _worst_cost_trial;
	    _netstream >> best_cost;
	    _netstream >> best_solution;
	    _netstream >> worst_cost;
	    _netstream << pack_end;

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
		current_step(_step_best_found_in_trial);
		KeepHistory(_best_solution_trial,_best_cost_trial,_worst_cost_trial,_time_best_found_in_trial,start_global + _time_best_found_in_trial);
		current_step(_current_step);
		
		time_spent_in_trial = _used_time(start_trial);
		total_time_spent    = start_global + time_spent_in_trial;
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
    
    void Solver_Wan::run ()
	{
	    while (current_trial() < params.independent_runs() ) //&& !(terminateQ(problem,*this,params)))
		run(params.nb_evolution_steps());
	}
    
    void Solver_Wan::run (const unsigned long int nb_steps)
	{
	    StartUp();
	    if (mypid!=0)
	    {
		while ((current_step() < nb_steps) && !(terminateQ(problem,*this,params)))
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
    
    void Solver_Wan::run (const Colony& col,const unsigned long int nb_steps)
	{
	    StartUp(col);
	    if (mypid!=0)
	    {
		while ((current_step() < nb_steps) && !(terminateQ(problem,*this,params)))
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
	
}
