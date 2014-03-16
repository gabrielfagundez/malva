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
	    // ---------------------------------
	    return os;
	}
    
    istream& operator>> (istream& is, Problem& pbm)
	{
	    // Problem-dependent information (+)
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
	    //return maximize;
	    // ---------------------------------
	}
    
    Problem::~Problem()
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	}
     
    int Problem::max_path_length() const
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	}
    int Problem::get_node_count() const
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	};

    int Problem::path_count_from(int node) const
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	}
    int Problem::get_destiny(int from, int through) const
	{
	    // Problem-dependent information (+)
	    // ---------------------------------
	}

    // Problem-dependent methos 

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
		// --------------------------------
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
		_fitness = ?;
		// --------------------------------
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
	   // ---------------------------------
       }
   Eta& Eta::operator= (const Eta& eta)
   {
       // Problem-dependent information (+)
       // ---------------------------------
   }
   Eta::~Eta()
       {
	   // Problem-dependent information (+)
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
	   // ---------------------------------
       }
   void Eta::reset (Ant *ant, Neighborhood *N)
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
      }

    
// -+----1----+----2----+----3----+----4----+----5----+----6---- (Neighborhood)
   Neighborhood::Neighborhood (Ant *ant, const Problem &pbm,  const SetUpParams& setup):
	_pbm(pbm)
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }

   Neighborhood::~Neighborhood()
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }

   int Neighborhood::size() const
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }
   int Neighborhood::operator[](int pos) const
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }

   void Neighborhood::move_to_node (int from, int through)
       {
	   // Problem-dependent information (+)
	   // ---------------------------------
       }

 // Problem-dependent methos
   
// -+----1----+----2----+----3----+----4----+---- (StopCondition/StopCondition)
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
// -+----1----+----2----+----3----+----4----+----5----+----6-- (UserStatistics)
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
	    //TrailFile << solver.colony().trail();


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
// --------------------------------------------------------------------------
     int test_convergency(const Colony &colony)
        {
	    // Complete --------------------------
	    return false;
	}

};

#endif




