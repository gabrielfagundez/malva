/***************************************************************************
			 Ant Colony System v1.10
			 developed by Guillermo Daniel Ordóñez
			 email gordonez@unsl.edu.ar
****************************************************************************/
#ifndef INC_REQ_ACO
#define INC_REQ_ACO
#include "aco.hh"
#include <math.h>


skeleton ACO
{
    
// -+----1----+----2----+----3----+----4----+----5----+----6----+--- (Problem)
    Problem::Problem ()
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	}
    
    ostream& operator<< (ostream& os, const Problem& pbm)
	{
	    // Problem-dependent information (-)
	    int i;
	    os << "Size: "<< pbm._size<< endl;
	    for (int i=0;i<pbm._size;i++){
		for (int j=0;j<pbm._size;j++)
		    os << pbm.cost(i+1,j) << "\t";
		os << endl;
	    }
	    // ---------------------------------
	    return os;
	}
    
    istream& operator>> (istream& is, Problem& pbm)
	{
	    // Problem-dependent information (+)
	    int i,j;
	    char buf[255];
	    // Ignorar las primera 3 lineas y leer la 4
	    for (i=0;i<4;i++) is.getline(buf,255,'\n');
	    sscanf(buf,"DIMENSION: %d",&pbm._size);
	    // Ignorar las primera 3 lineas 
	    for (i=0;i<3;i++) is.getline(buf,255,'\n');

	    pbm._cost = new int *[pbm._size];
	    for (i=0;i<pbm._size;i++)
		pbm._cost[i] = new int[pbm._size];	
	    for (i=0;i<pbm._size ;i++)
		for (j=0;j< pbm._size;j++)
		    is >> pbm._cost[i][j]; 

	    pbm._avg_cost= new double[pbm._size];
	    for (i=0;i<pbm._size;i++)
	    {
		pbm._avg_cost[i]=0;
		for (j=0;j< pbm._size;j++)
		    pbm._avg_cost[i]+=pbm._cost[j][i];
		pbm._avg_cost[i]/=pbm._size;
	    };
	    // ---------------------------------
	    return is;
	}
    
    Problem& Problem::operator=  (const Problem& pbm)
    {
	// Problem-dependent information (-)
	// ---------------------------------
	return *this;
    }
    
    bool Problem::operator== (const Problem& pbm) const
    {
	// Problem-dependent information (-)
	// ---------------------------------
	return true;
    }
    
    bool Problem::operator!= (const Problem& pbm) const
    {
	return !(*this == pbm);
    }

    Direction Problem::direction() const
	{
	    // Problem-dependent information (+)
	    return minimize;
	    // ---------------------------------
	}
    
    Problem::~Problem()
	{
	    // Problem-dependent information (+/-)
	    for (int i=0;i<_size;i++)
		delete _cost[i];
	    delete _cost;
	    delete _avg_cost;
	    // ---------------------------------
	}
    int Problem::max_path_length()const
	{
	    // Problem-dependent information (+)
	    return _size;
	    // ---------------------------------
	}
    int Problem::get_node_count() const
	{
	    // Problem-dependent information (+)
	    return _size+1;
	    // ---------------------------------
	};
    int Problem::path_count_from(int node) const
	{
	    // Problem-dependent information (+)
	    return _size;
	    // ---------------------------------
	}
    int Problem::get_destiny(int from, int through) const
	{
	    // Problem-dependent information (+)
	    return through+1;
	    // ---------------------------------
	}
    // Problem-dependent methos 
    int Problem::cost (int from, int through) const
	{
	    if (from==0) return 0;
	    return _cost[from-1][through];
	}
    double Problem::AvgCostTo(int node) const
        {
           return _avg_cost[node-1];
	};
// -+----1----+----2----+----3----+----4----+----5----+----6----+-- (Solution)
    
    Solution::Solution (const Problem& pbm):
	_pbm(pbm),
	_ant(NULL),
	_evaluated(*new bool(false)),
	_fitness(*new int(0))
	{
	    _string=new char[2*sizeof(int)+sizeof(double)+sizeof(int)*(3+(2*sizeof(int)*pbm.max_path_length()))];
	}
    
    const Problem& Solution::pbm() const
	{
	    return _pbm;
	}
    
    Solution::Solution(const Solution& sol):
	_pbm(sol.pbm()),
	_ant(NULL),
	_evaluated(*new bool(sol._evaluated)),
	_fitness(*new int(sol._fitness))
	{

	    _string=new char[2*sizeof(int)+sizeof(double)+sizeof(int)*(3+(2*sizeof(int)*_pbm.max_path_length()))];
	    if (sol._ant!=NULL)
		_ant=new Ant(*sol._ant);
	}
    
    istream& operator>> (istream& is, Solution& sol)
	{
	    // complete -
	    return is;
	}
    
    ostream& operator<< (ostream& os, const Solution& sol)
	{
	    if (sol._ant !=NULL)
	    {
		// Problem-dependent information (+)
		int i;
		for (i=0;i<sol.ant()->size();i++)
		    cout << " " <<(*sol.ant())[i];
		cout << endl;
		// ---------------------------------
	    }
	    else 
		os << "Undefined solution";
	    return os;
	}
    
    NetStream& operator << (NetStream& ns, const Solution& sol)
	{
	    ns << sol._evaluated 
	       << sol._fitness
	       << *sol._ant;
	    return ns;
	}
    
    NetStream& operator >> (NetStream& ns, Solution& sol)
	{
	    ns >> sol._evaluated 
	       >> sol._fitness;
	    if (sol._ant==NULL) sol._ant = new Ant(sol._pbm);
	    ns >> *sol._ant;
	    return ns;
	}
    
    Ant * Solution::ant() const
	{
	    return _ant;
	}

    void Solution::ant(Ant *new_ant)
	{
	    delete(_ant); 
	    _ant = new_ant;
	    _evaluated=false;
	}

    Solution& Solution::operator= (const Solution &sol) 
    {
	_evaluated=sol._evaluated;
	_fitness=sol._fitness;
	delete (_ant);
	if (sol._ant!=NULL)
	    _ant = new Ant (*sol._ant);	
	else _ant=NULL;
	return *this;
    }
    
    bool Solution::operator== (const Solution& sol) const
    {
	
	if (_evaluated!=sol._evaluated) return false;
	if (_fitness!=sol._fitness) return false;
	if (sol._ant       != _ant) return false;
	if (sol.pbm()      != pbm())  return false;
	return true;
    }
    
    bool Solution::operator!= (const Solution& sol) const
    {
		return !(*this == sol);
    }
        
    double Solution::fitness () const
	{
	    if (_ant==NULL) {
		cout << "? this shouldn't happend" <<endl;
		return 0;
	    }
	    if (!_evaluated){
		// Problem-dependent information (+)
		int i,node=0;
		_fitness = 0;		
		for (i=0;i<_ant->size();i++)
		{
		    _fitness +=_pbm.cost(node,(*_ant)[i]);
		    node=_pbm.get_destiny(node,(*_ant)[i]);
		}
		// ---------------------------------
		_evaluated=true;
	    }
	    return (double) _fitness;
	}

    char *Solution::to_String() const
	{
	    int * dst=(int *) _string;
	    int _size=size();
	    dst[0]=_evaluated;
	    dst[1]=_fitness;
	    dst[2]=_size;
	    if (_ant!=NULL)
		memcpy(&dst[3],_ant->to_String(),_ant->string_size());
	    return _string;
	}
	    
    void Solution::to_Solution(char *_string_)
	{
	    int _size;
	    int * src=(int *) _string_;
	    _evaluated=src[0];
	    _fitness=src[1];
	    _size=src[2];
	    if (_size >3*sizeof(int))
	    {
		if (_ant==NULL)
		    _ant=new Ant(_pbm);
		_ant->to_Ant((char*)&src[3]);
	    }
	    else
		delete _ant;
	}
    
    unsigned int Solution::size() const
	{
	    int _size=3*sizeof(int);
	    if (_ant!=NULL) _size+=_ant->string_size();
	    return _size;
	}
    
   Solution::~Solution()
	{	
	    delete &_fitness;
	    delete &_evaluated;
	    delete _string;

	    delete _ant;
    
	};

// -+----1----+----2----+----3----+----4----+----5----+----6----+----7-- (Eta)
   Eta::Eta(const Problem& pbm,  const SetUpParams& setup):
	_problem(pbm)
       {
	   // Problem-dependent information (+)
 	   int i,j;
	   _node_count=pbm.get_node_count();
	   _data=new double *[_node_count];
	   for (i=0;i<_node_count;i++){
	       _data[i]=new double[_node_count-1];
	       for (j=0; j<_node_count-1;j++)
		   if (pbm.cost(i,j)==0) _data[i][j]=_node_count;// Corregir
		   else _data[i][j]= 1.0 / pbm.cost(i,j) * pbm.AvgCostTo(j+1);
       	   }
	   // ---------------------------------
       }
   Eta& Eta::operator= (const Eta& eta)
       {
	   // Problem-dependent information (+)
	   int i,j;
	   _node_count=eta._node_count;
	   for (i=0;i<_node_count;i++)
	       for (j=0;j<_node_count-1;j++)
		   _data[i][j]=eta._data[i][j];
	   // ---------------------------------
       }
   Eta::~Eta()
       {
	   // Problem-dependent information (+)
	   int i;
	   for (i=0;i<_problem.get_node_count();i++)
	       delete _data[i];
	   delete _data;
	   // ---------------------------------
       }
   void Eta::move_to_node(int from, int through)
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }
   double Eta::get_value (int from, int through)
       {
	   // Problem-dependent information (+)
	   return 1.0 + _data[from][through];;	
	   // ---------------------------------
       }
   void Eta::reset (Ant *ant, Neighborhood *N)
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
      }

    
// -+----1----+----2----+----3----+----4----+----5----+----6----+- (Neighborhood)
   Neighborhood::Neighborhood (Ant *ant, const Problem &pbm,  const SetUpParams& setup):
	_pbm(pbm),
       _data(new int[pbm.get_node_count()+1]),
       _pos(new int[pbm.get_node_count()+1]),
       _size(pbm.max_path_length())
       {
	   // Problem-dependent information (+)
	   int i;
	   for (i=0;i<_size;i++)	   
	       _data[i]=_pos[i]=i;
	   // ---------------------------------
       }
   Neighborhood::~Neighborhood()
       {
	   // Problem-dependent information (+)
	   delete _data;
	   delete _pos;
	   // ---------------------------------
       }

   int Neighborhood::size() const
       {
	   // Problem-dependent information (+)
	   return _size;
	   // ---------------------------------
      }
   int Neighborhood::operator[](int pos) const
       {
	   // Problem-dependent information (+)
 	   return _data[pos];
 	   // ---------------------------------
       }

 
   void Neighborhood::move_to_node (int from, int through)
       {
	   // Problem-dependent information (+)
	   _size--;
	   _pos[_data[_size]]=_pos[through];
	   _data[_pos[through]]=_data[_size];  
	   // ---------------------------------
       }

  
// -+----1----+----2----+----3----+----4----+--- (StopCondition/StopCondition)
    StopCondition_1::StopCondition_1():StopCondition()
	{}
    
    bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
	    return ((solver.current_trial()==setup.independent_runs()) && (solver.current_step()==setup.nb_evolution_steps()));
	}
    
    StopCondition_1::~StopCondition_1()
	{}
    
    bool terminateQ (const Problem& pbm, const Solver& solver,
		     const SetUpParams& setup)
	{
	    StopCondition_1 stop;
	    return stop.EvaluateCondition(pbm,solver,setup);
	}
// -+----1----+----2----+----3----+----4----+----5----+----6- (UserStatistics)
    UserStatistics::UserStatistics ()
	{
	    // Solo para utilizar con el secuencial!!
	    //TrailFile.open("Trail.dat");
	}
    
    ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
	  os << endl;
	  os << "-------------------------------------------------------------------------------" << endl;
	  os << "                             STATISTICS OF TRIALS                   	 " << endl;
	  os << "-------------------------------------------------------------------------------" << endl;
	  os << "Trial"
	     << "\tBest Cost"
	     << "\tWorst Cost" 
	     << "\tBest Iteration" 
	     << "\tTime Best" 
	     << "\tTime" ;
	  
	  for (int i=0;i< userstat.result_trials.size();i++)
	    {
	      os << endl
		 << userstat.result_trials[i].trial
		 << "\t" << userstat.result_trials[i].best_cost_trial
		 << "\t\t" << userstat.result_trials[i].worst_cost_trial
		 << "\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
		 << "\t\t" << userstat.result_trials[i].time_best_found_trial
		 << "\t\t" << userstat.result_trials[i].time_spent_trial;
	    }
	  os << endl;
	  os << "-------------------------------------------------------------------------------" << endl;
	  return os;
	}
    
    UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
    {
	result_trials=userstats.result_trials;
	return (*this);
    }
    
    void UserStatistics::update(const Solver& solver)
	{
	    // Solo para utilizar con el secuencial!!
	    // TrailFile << solver.colony().trail();


	    if (!(solver.pid()==0 && (solver.end_trial()==true)
		  && (solver.current_step()==solver.setup().nb_evolution_steps())))
		return;
	    
	    struct user_stat *new_stat;
	    
	    if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
		show_message(7);
	    new_stat->trial         		 = solver.current_trial();
	    new_stat->nb_evaluation_best_found_trial = solver.step_best_found_in_trial();
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
// -----------------------------------------------------------------------------------------
     int test_convergency(const Colony &colony)
        {
	    // Complete --------------------------
	    return false;
	}

};

#endif




