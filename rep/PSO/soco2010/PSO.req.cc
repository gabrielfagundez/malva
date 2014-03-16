/************************************************
***                       		      *** 
***  Particle Swarm      Skeleton v1.0        *** 
***  RPSO-vm                                  ***
***  Required classes and methods             ***
***  Developed by: José Manuel Garcia Nieto   *** 
***  for SOCO 2010 Benchmark                  *** 
************************************************/
#ifndef INC_REQ_PSO
#define INC_REQ_PSO

//#include <math.h>
#include <string>
#include <sstream>

#include "Mallba/random.hh"
//#include "PSO.hh"
#include "funsoft/funsoft.h"
#include "funsoft/cec08data.h"
#include "funsoft/funsoft.c"





skeleton PSO {


// Problem ---------------------------------------------------------------
    Problem::Problem ():_dimension(0),_nfunction(0),_x(NULL)
    {

    }

    ostream& operator<< (ostream& os, const Problem& pbm)
    {
        os << endl << endl << "Dimension: " << pbm._dimension
           << endl << "Number of Funcion: " << pbm._nfunction <<  endl;

        for (int i=0;i<pbm._dimension;i++)
            os << i << "\t " << pbm._x[i] << "\t ";
        os<< endl;
        return os;
    }

    istream& operator>> (istream& is, Problem& pbm)
    {
        char buffer[MAX_BUFFER];
        int i;

        is.getline(buffer,MAX_BUFFER,'\n');
        sscanf(buffer,"%d %d",&pbm._dimension, &pbm._nfunction);

        return is;
    }

    Problem& Problem::operator=(const Problem& pbm)
    {
        _dimension=pbm.dimension();
        _nfunction=pbm.nfunction();     

        if (_x!=NULL) free(_x);

        if ((_x = new double[_dimension]) == NULL)
            show_message(7);

        for (int i = 0; i < _dimension; i++)
        {
            _x[i] = pbm.x(i);
                }

        return *this;
    }

    bool Problem::operator== (const Problem& pbm) const
    {
        if (_dimension!=pbm.dimension()) return false;
        if (_nfunction!=pbm.nfunction()) return false;      

        for (int i = 0; i < _dimension; i++)
            if (_x[i] != pbm.x(i))
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

    double Problem::x(const int i) const
    {
        return _x[i];
    }

    int Problem::nfunction() const
    {
        return _nfunction;
    }

    double* Problem::x() const
    {
        return _x;
    }
                
    void Problem::dimension(const int d)
    {
        _dimension=d;
    }
    
        void Problem::nfunction(const int f)
    {
        _nfunction=f;
    }
                
    void Problem::x(const int index, const double value)
    {
        _x[index]=value;
    }


    /*double Problem::best_cost() const
    {
        return _best_cost;
    }*/
    
    Problem::~Problem()
    {
        delete [] _x;
    }

// Solution --------------------------------------------------------------
    
    Solution::Solution (const Problem& pbm):
      _pbm(pbm),
      _current(pbm.dimension()),
      //_next(pbm.dimension()),
      _best(pbm.dimension()),
      _velocity(pbm.dimension()),
      _current_fitness(infinity()), 
      _best_fitness(infinity())
        {}

   const Problem& Solution::pbm() const
        {
                return _pbm;
        }

    Solution::Solution(const Solution& sol):
        _pbm(sol.pbm())
        {               
                *this=sol;
        }

    istream& operator>> (istream& is, Solution& sol)
        {
      for (int i=0;i<sol.pbm().dimension();i++)
        is >> sol._current[i];


      //for (int i=0;i<sol.pbm().dimension();i++)
        //is >> sol._next[i];
    

      for (int i=0;i<sol.pbm().dimension();i++)
        is >> sol._best[i];
    

      for (int i=0;i<sol.pbm().dimension();i++)
        is >> sol._velocity[i];
      
          return is;
        }

        ostream& operator<< (ostream& os, const Solution& sol)
        {
            
            for (int i=0;i<sol.pbm().dimension();i++)
                    os << " " << sol._current[i];
            os << endl;
            //for (int i=0;i<sol.pbm().dimension();i++)
                //  os << " " << sol._next[i];
            //os << endl;
            /*for (int i=0;i<sol.pbm().dimension();i++)
                    os << " " << sol._best[i];
            os << endl;
	    os << "vel : " << endl;
            for (int i=0;i<sol.pbm().dimension();i++)
                    os << " " << sol._velocity[i];
            os << endl;*/
      
            return os;
        }
        
    NetStream& operator << (NetStream& ns, const Solution& sol)
        {
                for (int i=0;i<sol._current.size();i++)
                        ns << sol._current[i];
                //for (int i=0;i<sol._next.size();i++)
                  //      ns << sol._next[i];
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
                //for (int i=0;i<sol._next.size();i++)
                  //      ns >> sol._next[i];
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
        //  	_next    = sol._next;
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

        
        
        double Solution::fitness ()
        {
 	       	double * x;
        	//double f;
		long double f;


        /*if (_pbm.dimension()!=50 && _pbm.dimension()!=100 && _pbm.dimension()!=200 && _pbm.dimension()!=500)
            {
                fprintf(stderr,"\n Wrong value of 'nreal' entered, only 50, 100, 200, 500 variables are supported\n");
                exit(0);
            }*/
        
            	/* Call these routines to initialize random number generator */
            	/* require for computing noise in some test problems */

            	/* Variable vector */
            	x = (double *)malloc(_pbm.dimension()*sizeof(double));

            	for (int i=0; i<_pbm.dimension(); i++)
            	{
                    	x[i]=current(i);
            	}
        	//if (_pbm.nfunction()<7) /* functions of CEC'08 */
                //	f = func(_pbm.nfunction(),_pbm.dimension(),x);
        	//else{ /* new functions of ISDA'09 */
		/*	if (_pbm.nfunction() == 7){
				f = f_Schwefel2_22(_pbm.dimension(),x);
			}else if (_pbm.nfunction() == 8)
				f = f_Schwefel1_2(_pbm.dimension(),x); 
			else if (_pbm.nfunction() == 9)
				f = Extended_f_10(_pbm.dimension(),x); 
			else if (_pbm.nfunction() == 10)
				f = f_Bohachevsky(_pbm.dimension(),x); 
			else if (_pbm.nfunction() == 11)
				f = f_Schaffer(_pbm.dimension(),x); 
		}*/
		/* soft computing functions */
		if (_pbm.nfunction() == 1)
			f = Shifted_Sphere(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 2)
			f = Schwefel_Problem(_pbm.dimension(),x);
                else if (_pbm.nfunction() == 3)
                        f = Shifted_Rosenbrock(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 4)
                        f = Shifted_Rastrigin(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 5)
                        f = Shifted_Griewank(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 6)
                        f = Shifted_Ackley(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 7)
                        f = f_Schwefel2_22(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 8)
                        f = f_Schwefel1_2(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 9)
                        f = Extended_f_10(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 10)
                        f = f_Bohachevsky(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 11)
                        f = f_Schaffer(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 12)
                        f = f_Hybrid_12(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 13)
                        f = f_Hybrid_13(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 14)
                        f = f_Hybrid_14(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 15)
                        f = f_Hybrid_15(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 16)
                        f = f_Hybrid_16new(_pbm.dimension(),x);
                else if (_pbm.nfunction() == 17)
                        f = f_Hybrid_17new(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 18)
                        f = f_Hybrid_18new(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 19)
                        f = f_Hybrid_19new(_pbm.dimension(),x);
		/*else if (_pbm.nfunction() == 20)
                        f = f_Hybrid_20new(_pbm.dimension(),x);
		else if (_pbm.nfunction() == 21)
                        f = f_Hybrid_21new(_pbm.dimension(),x);*/
		

        	/* Routine to free the memory allocated at run time */
    
        	free (x);
        
        	_current_fitness = f;
        	return _current_fitness;
        }

    
	void Solution::initialization(double dmin, double dmax)
	{
 		double m   = 5.0; 	// divides the variable space into 5 partitions  
		double vs  = dmax-dmin;	// variables scape dimension
	    	double pd  = vs/m; 	// partition dimension: number of variables per partition
		double ps  = 0.0;       // probability of selecting a given partition 			
		double min = dmin;
		double max = dmax;


	    	for (int i=0;i<_pbm.dimension();i++){
			/*ps = rand01();
			if (0.9<=ps<1.0){
				max = dmax;
			}else if (0.5<=ps<0.9){
				max = dmax + pd;
			}else if (0.4<=ps<0.5){
				max = dmax + pd*2;
			}else if (0.3<=ps<0.4){
				max = dmax + pd*3;
			}else{
				max = dmax + pd*4;
			}
			min = max + pd; */	

	        	_current[i]= min + rand01()*(max-min);
			_best[i]=_current[i];
                        _velocity[i]= (min + rand01()*(max-min))/2.0;
	        }/* end for */
	}/* end initialization */

		double Solution::current_fitness()
		{
		    return _current_fitness;
		}   

		void Solution::current_fitness(const double f)
		{
			_current_fitness = f;
		}



	    double Solution::best_fitness()
	      {
		return _best_fitness;
	      }

	    void Solution::best_fitness(const double bf)
	      {
		_best_fitness=bf;
	      }

	    char * Solution::to_String() const
		{
		/*char * cad;
		std::ostringstream out;
		out << _current;
		cad = new char[out.str().size()+1];
		strcpy(cad,out.str().c_str());  
		return cad;*/
		return (char *)_current.get_first();       
		}

		void Solution::to_Solution(char *_string_)
		{
		        double *ptr =(double *)_string_;
		        for (int i=0;i<_pbm.dimension();i++)
		        {
		                _current[i]=*ptr;
		                ptr++;
		        }
		}
		            
		unsigned int Solution::size() const
		{
		        return (_pbm.dimension() * sizeof(double));
		}


		double& Solution::current(const int index)
		{
		        return _current[index];
		}

	    double& Solution::next(const int index)
	      {
		return _next[index];
	      }

	    double& Solution::best(const int index)
	      {
		return _best[index];
	      }
		        
	    double& Solution::velocity(const int index)
	      {
		return _velocity[index];
	      }
		        
	    void Solution::current(const int index, const double value)
	      {
		_current[index]=value;
	      }

	    void Solution::next(const int index, const double value)
	      {
		_next[index]=value;
	      }
		        
	    void Solution::best(const int index, const double value)
	      {
		_best[index]=value;
	      }

		        
	    void Solution::velocity(const int index, const double value)
	      {
		_velocity[index]=value;
	      }
		                    
	    Rarray<double>& Solution::current()
	      {
		return _current;
	      }

	    Rarray<double>& Solution::next()
	      {
		return _next;
	      }

	    Rarray<double>& Solution::best()
	      {
		return _best;
	      }
		        
	    Rarray<double>& Solution::velocity()
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
        //os << "\n---------------------------------------------------------------" << endl;
        //os << "                   STATISTICS OF TRIALS                     " << endl;
        //os << "------------------------------------------------------------------" << endl;

        for (int i=0;i< userstat.result_trials.size();i++)
        {
            os << userstat.result_trials[i].trial
               << "\t" << userstat.result_trials[i].best_cost_trial
               << "\t\t" << userstat.result_trials[i].worst_cost_trial
               << "\t\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
               << "\t\t\t" << userstat.result_trials[i].time_best_found_trial
               << "\t\t" << userstat.result_trials[i].time_spent_trial
               << endl;
        }
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
        new_stat->trial                  = solver.current_trial();
        new_stat->nb_evaluation_best_found_trial = solver.iteration_best_found_in_trial();
        new_stat->worst_cost_trial           = solver.worst_cost_trial();
        new_stat->best_cost_trial            = solver.best_cost_trial();
        new_stat->time_best_found_trial      = solver.time_best_found_trial();
        new_stat->time_spent_trial       = solver.time_spent_trial();

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
        //return ((double)solver.best_cost_trial()==pbm.best_cost());
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
