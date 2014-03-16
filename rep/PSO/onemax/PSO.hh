/*****************************************************************
 *
 * First version of PSO for Mallva v0.0
 * José Manuel García Nieto 01/12/2006
 * File : Pso.cpp BINARY PSO
 * Definition of the PSO skeleton for Mallba
 * For the problem instance: 
 *
 *****************************************************************
 **  Proyecto Fin de Carrera 2005/2006 ****************************/



#ifndef INC_PSO
#define INC_PSO

#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <Mallba/Rlist.h>
#include <Mallba/Rarray.h>
#include <Mallba/mallba.hh>
#include <Mallba/States.hh>
#include <Mallba/random.hh>
#include <Mallba/time.hh>
#include <Mallba/netstream.hh>

skeleton PSO
{

  provides class SetUpParams;
  provides class Statistics;
  provides class Swarm;
  provides class Migration;
  provides class Solver;
  provides class Solver_Seq;
  provides class Solver_Lan;
  provides class Solver_Wan;
  provides class StopCondition;

  requires class Problem;
  requires class Solution;
  requires class UserStatistics;
  requires class StopCondition_1;
  requires bool terminateQ (const Problem& pbm, const Solver& solver, const SetUpParams& setup);

  struct particle // index of a particle in the swarm and its fitness
  {
        int index;
        double fitness;
  };
  
// Problem ----------------------------------------------------------------------------

  requires class Problem
  {
	public:
		Problem();
		~Problem();

		friend ostream& operator<< (ostream& os, const Problem& pbm);
		friend istream& operator>> (istream& is, Problem& pbm);

		Problem& operator=  (const Problem& pbm);
		bool operator== (const Problem& pbm) const;
		bool operator!= (const Problem& pbm) const;

		Direction direction () const;

		int dimension() const;

	private:

		int _dimension;
  };

//Solution ----------------------------------------------------------------------------

  requires class Solution
  {
	public:
		Solution (const Problem& pbm);
		Solution (const Solution& sol);
		~Solution();

 		friend ostream& operator<< (ostream& os, const Solution& sol);
		friend istream& operator>> (istream& is, Solution& sol);
		friend NetStream& operator << (NetStream& ns, const Solution& sol);
 		friend NetStream& operator >> (NetStream& ns, Solution& sol);

		const Problem& pbm() const;

		Solution& operator=  (const Solution& sol);
		bool operator== (const Solution& sol) const;
		bool operator!= (const Solution& sol) const;

		char *to_String() const;
		void to_Solution(char *_cadena_);

		void initialize();
		double fitness();
        double best_fitness();
        void best_fitness(const double bf);

		unsigned int size() const;

		int& current(const int index);
        int& next(const int index);
        int& best(const int index);
        int& velocity(const int index);
                
        void current(const int index, const int value);
        void next(const int index, const int value);
        void best(const int index, const int value);
        void velocity(const int index, const int value);

		Rarray<int>& current();
        Rarray<int>& next();
        Rarray<int>& best();
        Rarray<int>& velocity();

	private:

        Rarray<int> _current;
        Rarray<int> _next;
        Rarray<int> _best;         
        Rarray<int> _velocity;
    
        double _current_fitness;
        double _best_fitness;
                  
		const Problem& _pbm;
  };

// UserStatistics ----------------------------------------------------------------------------

  requires class UserStatistics
  {
	private:
		struct user_stat
		{
			unsigned int trial;
			unsigned long nb_evaluation_best_found_trial;
			double best_cost_trial;
			double worst_cost_trial;
			float time_best_found_trial;
			float time_spent_trial;
		};

		Rlist<struct user_stat> result_trials;

	public:
 		UserStatistics ();
		~UserStatistics();

		friend ostream& operator<< (ostream& os, const UserStatistics& usertats);
        UserStatistics& operator= (const UserStatistics& userstats);
		void update(const Solver& solver);
		void clear();
 };

// StopCondition ----------------------------------------------------------------------------------
  provides class StopCondition
  {
	public:
		StopCondition();
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)=0;
		~StopCondition();
  };

// StopCondition_1 {subclase-------------------------------------------------------------------------

  requires class StopCondition_1 : public StopCondition
  {
	public:
		StopCondition_1();
		virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
		~StopCondition_1();
  };
// SetUpParams -------------------------------------------------------------------------------

  provides class SetUpParams
  {
	private:
		unsigned int    _independent_runs;
		unsigned long   _nb_evolution_steps;
		unsigned int    _swarm_size;		// number of particles in the swarm
		unsigned int    _particle_size;	        // size of each particle
        unsigned int    _neighborhood_size;     // size of the neighborhood in a swarm

		bool _display_state;

        // For LAN execution configuration
        unsigned long    _refresh_global_state;
        unsigned int     _migration_rate;
        bool 		     _synchronized;
        unsigned int 	 _check_asynchronous;

		 /* Limits fot location changes */
                 double _delta_min;
                 double _delta_max;
                 /* Individuality and sociality */
                 double _individuality_weight;
                 double _ind_min_weight; /* low stochastic weight factor*/
                 double _ind_max_weight; /* high stochastic weight factor*/
                 double _sociality_weight;
                 double _soc_min_weight; /* low stochastic weight factor */
                 double _soc_max_weight; /* high stochastic weight factor */
                 /* Max Min weight factors*/
                 double _weight_max;
                 double _weight_min;

	public:
		SetUpParams ();

 		friend ostream& operator<< (ostream& os, const SetUpParams& setup);
		friend istream& operator>> (istream& is, SetUpParams& setup);

		const unsigned int   independent_runs() const;
		const unsigned long  nb_evolution_steps() const;
		const unsigned int   swarm_size() const;
		const unsigned int   particle_size() const;
        const unsigned int   neighborhood_size() const;
		const bool           display_state() const;
		const unsigned long  refresh_global_state() const;
		const unsigned int migration_rate() const;
        const bool synchronized() const;
        const unsigned int check_asynchronous() const;

		void independent_runs(const unsigned int val);
		void nb_evolution_steps(const unsigned long val);
		void swarm_size(const unsigned int val);
		void particle_size(const unsigned int val);
                void neighborhood_size(const unsigned int val);
		void display_state(const bool val);
		void refresh_global_state(const unsigned long val);
		void migration_rate(const unsigned int val);
        void synchronized(const bool val);
        void check_asynchronous(const unsigned int val);

                const double delta_min() const;
                const double delta_max() const;
                const double individuality_weight() const;
                const double ind_min_weight() const;
                const double ind_max_weight() const;
                const double sociality_weight() const;
                const double soc_min_weight() const;
                const double soc_max_weight() const;
                const double weight_min() const;
                const double weight_max() const;
    
                void individuality_weight(const double val);
                void ind_min_weight(const double val);
                void ind_max_weight(const double val);
                void sociality_weight(const double val);
                void soc_min_weight(const double val);
                void soc_max_weight(const double val);
                void weight_min(const double val);
                void weight_max(const double val);
                void delta_min(const double val);
                void delta_max(const double val);
                
                void RefreshState(const StateCenter& _sc) const;
                void UpdateFromState(const StateCenter& _sc) const;

		~SetUpParams();
  };

// Statistics ---------------------------------------------------------------------------------

  provides class Statistics
  {
	private:
		struct stat
		{
			unsigned int trial;
			unsigned long nb_generation;
			double best_cost;
			double global_best_cost;
			double average_cost;
			double standard_deviation_cost;
		};

		Rlist<struct stat> stats_data;

	public:
		Statistics();
		~Statistics();

		friend ostream& operator<< (ostream& os, const Statistics& stats);

	 	Statistics& operator= (const Statistics& stats);
		void update(const Solver& solver);
		void clear();
  };

// Swarm ---------------------------------------------------------------------------------

  provides class Swarm
  {
	private:
		Rarray<Solution*> _particles;     // individuals in population
		Rarray<struct particle> _fitness_values;
		const SetUpParams& _setup;
		unsigned int _upper_cost,_lower_cost; // lower and upper fitness of individuals in population
		double _average_cost;

	public:
		Swarm(const Problem& pbm,const SetUpParams& setup); 
		~Swarm();

		friend ostream& operator<< (ostream& os, const Swarm& swarm);
		friend istream& operator>> (istream& is, Swarm& swarm);
		Swarm& operator= (const Swarm& swarm);
		const SetUpParams& setup() const;
	  	void initialize();

		// interchange solutions between island
		//void interchange(const unsigned long current_generation, NetStream& channel);

		// creates a array with fitness of all particles in swarm and its position in the swarm
        void evaluate();
		void local_search(Solution& sol);
	 	const Rarray<Solution*>& particles() const;
		Rarray<struct particle>& fitness_values();

		unsigned int upper_cost() const;
		unsigned int lower_cost() const;
		Solution& solution(const unsigned int index) const;
        Solution& neighbor_with_best_fitness(const unsigned int index) const;
		double fitness(const unsigned int index) const;
        bool neighbor_solution(unsigned int index ,unsigned int ns) const;
		int distance(int index, int in) const;

		double best_cost() const;
		double worst_cost() const;
		Solution& best_solution() const;
		Solution& worst_solution() const;
		double average_cost() const;
		double standard_deviation() const;
		double constrict(double v);  
        double sig(double v);
        double weight(int iter,int miter);
		void evolution(int iter); /*makes an evolution step*/
		void interchange(const unsigned long current_generation, const Migration& migration ,NetStream& channel);
  };	

// Migration ------------------------------------------------------------------------------

 provides class Migration
  {
  		private:
  			const Direction direction;
  			const int migration_rate;
        public:
                Migration(const Direction dir, const int mig_rate);
                
                virtual ~Migration();

                friend ostream& operator<< (ostream& os, const Migration& migration);

                virtual void execute(Swarm& swarm,const unsigned long current_generation,NetStream& _netstream,const bool synchronized,const unsigned int check_asyncrhonous) const;
  };



// Solver  ---------------------------------------------------------------------------------

  provides class Solver
  {
	protected:
		const Problem&     problem;
		const SetUpParams& params;
		Swarm              _swarm;
 		UserStatistics     _userstat;
		Statistics         _stat;		
		StateCenter        _sc;

		double 	   _best_cost;
		double 	   _worst_cost;
		Solution   _best_solution;
		double     _average_cost;
		double     _standard_deviation;
		float      _total_time_spent;
		float      _time_spent_in_trial;
		float 	   _start_trial;
		float 	   _start_global;

		bool 	    _end_trial;

		State_Vble  _current_trial;
		State_Vble  _current_iteration;

		State_Vble  _current_best_solution;
		State_Vble  _current_best_cost;
		State_Vble  _current_worst_cost;
		State_Vble  _current_average_cost;
		State_Vble  _current_standard_deviation;
		State_Vble  _current_time_spent;

	 	State_Vble  _best_solution_trial;
		State_Vble  _best_cost_trial;
		State_Vble  _worst_cost_trial;
		State_Vble  _iteration_best_found_in_trial;
		State_Vble  _time_best_found_trial;
		State_Vble  _time_spent_trial;

		State_Vble  _trial_best_found;
		State_Vble  _iteration_best_found;
		State_Vble  _global_best_solution;
		State_Vble  _global_best_cost;
		State_Vble  _global_worst_cost;
		State_Vble  _time_best_found;

		State_Vble _delta_min;
                State_Vble _delta_max;
                 /* Individuality and sociality */
                State_Vble _individuality_weight;
                State_Vble _ind_min_weight; /* low stochastic weight factor*/
                State_Vble _ind_max_weight; /* high stochastic weight factor*/
                State_Vble _sociality_weight;
                State_Vble _soc_min_weight; /* low stochastic weight factor */
                State_Vble _soc_max_weight; /* high stochastic weight factor */
                 /* Max Min weight factors*/
                State_Vble _weight_max;
                State_Vble _weight_min;
	
		State_Vble  _display_state;


	public:
		Solver (const Problem& pbm, const SetUpParams& setup);
		virtual ~Solver ();

		virtual int pid() const;
		bool end_trial() const;
		void end_trial(bool et);

		// Execution methods -----------------------------------------------------------------------

		// Full execution
		virtual void run () =0;
		virtual void run (const unsigned long int nb_generations) =0;
		virtual void run (const Swarm& pop,const unsigned long int nb_generations) =0;

		//Partial execution
		virtual void StartUp()=0;
		virtual void StartUp(const Swarm& swarm)=0;

		virtual void DoStep()=0;

		// Statistics handling ----------------------------------------------------------------------

		Statistics& statistics();
 		UserStatistics& userstatistics();        
		const SetUpParams& setup() const;
		const Problem& pbm() const;
		Swarm& swarm();
		
		// State handling ---------------------------------------------------------------------------

		void RefreshState();
		void RefreshCfgState();
		void UpdateFromState();
		void UpdateFromCfgState();
		StateCenter* GetState();

		unsigned int current_trial() const;
		unsigned long current_iteration() const;
		Solution current_best_solution() const;
		double current_best_cost() const;
		double current_worst_cost() const;
		double current_average_cost() const;
		double current_standard_deviation() const;
		float  current_time_spent() const;
		Solution  best_solution_trial() const;
		double best_cost_trial() const;
		double worst_cost_trial() const;
		unsigned int iteration_best_found_in_trial() const;
		float time_best_found_trial() const;
		float time_spent_trial() const;
		unsigned int trial_best_found() const;
		unsigned int iteration_best_found() const;
		Solution global_best_solution() const;
		double global_best_cost() const;
		double global_worst_cost() const;
		float time_best_found() const;
		int display_state() const;

		void current_trial(const unsigned int value);
		void current_iteration(const unsigned long value);
		void current_best_solution(const Solution& sol);
		void current_best_cost(const double value);
		void current_worst_cost(const double value);
		void current_average_cost(const double value);
		void current_standard_deviation(const double value);
		void current_time_spent(const float value);
		void best_solution_trial(const Solution& sol);
		void best_cost_trial(const double value);
		void worst_cost_trial(const double value);
		void iteration_best_found_in_trial(const unsigned int value);
		void time_best_found_trial(const float value);
		void time_spent_trial(const float value);
		void trial_best_found(const unsigned int value);
		void iteration_best_found(const unsigned int  value);
		void global_best_solution(const Solution& sol);
		void global_best_cost(const double value);
		void global_worst_cost(const double value);
		void time_best_found(const float value);
		void display_state(const int value);

        const double delta_min() const;
        const double delta_max() const;
                const double individuality_weight() const;
                const double ind_min_weight() const;
                const double ind_max_weight() const;
                const double sociality_weight() const;
                const double soc_min_weight() const;
                const double soc_max_weight() const;
                const double weight_min() const;
                const double weight_max() const;
    
                void individuality_weight(const double value);
                void ind_min_weight(const double value);
                void ind_max_weight(const double value);
                void sociality_weight(const double value);
                void soc_min_weight(const double value);
                void soc_max_weight(const double value);
                void weight_min(const double value);
                void weight_max(const double value);
                void delta_min(const double value);
                void delta_max(const double value);

	 	void show_state() const;
		void KeepHistory(const Solution& best_sol,const double _best_cost,const double _worst_cost,const float _time_spent_trial,const float _total_time_spent);

                
  };

  provides class Solver_Seq: public Solver
  {
	public:
		Solver_Seq ( const Problem& pbm, const SetUpParams& setup);
		virtual ~Solver_Seq ();

		// Execution methods -----------------------------------------------------------------------
                
		// Full execution
		virtual void run ();
		virtual void run (const unsigned long int nb_generations);
		virtual void run (const Swarm& s,const unsigned long int nb_generations);

		//Partial execution
		virtual void StartUp();
		virtual void StartUp(const Swarm& s);        
		virtual void DoStep();
  };
  
  
  provides class Solver_Lan: public Solver
  {
        private:
                NetStream _netstream;
                const Migration _migration;
                int mypid;

                int receive_local_state();

                unsigned int _current_trial;
                unsigned long _current_iteration;
                double _best_cost_trial;
                Solution _best_solution_trial;
                double _worst_cost_trial;
                float _time_best_found_in_trial;
                unsigned long _iteration_best_found_in_trial;


        public:
                Solver_Lan (const Problem& pbm, const SetUpParams& setup, const Migration& m ,int argc,char **argv);
                virtual ~Solver_Lan ();
                virtual int pid() const;
                NetStream& netstream();

                // Execution methods -----------------------------------------------------------------------

                // Full execution
                virtual void run ();
                virtual void run (const unsigned long int nb_generations);
                virtual void run (const Swarm& swarm,const unsigned long int nb_generations);

                //Partial execution
                virtual void StartUp();
                virtual void StartUp(const Swarm& swarm);

                virtual void DoStep();

                //Communication
                void send_local_state_to(int _mypid);
                void check_for_refresh_global_state();
                void reset();
  };
  
  provides class Solver_Wan: public Solver
  {
        private:
                NetStream _netstream;
                int mypid;
				const Migration _migration;
                void send_local_state_to(int _mypid);
                int receive_local_state();

                void check_for_refresh_global_state();

                unsigned int _current_trial;
                unsigned long _current_iteration;
                double _best_cost_trial;
                Solution _best_solution_trial;
                double _worst_cost_trial;
                float _time_best_found_in_trial;
                unsigned long _iteration_best_found_in_trial;
				

        public:
                Solver_Wan (const Problem& pbm, const SetUpParams& setup, const Migration& m, int argc,char **argv);
                virtual ~Solver_Wan ();
                virtual int pid() const;
                NetStream& netstream();

                // Execution methods -----------------------------------------------------------------------

                // Full execution
                virtual void run ();
                virtual void run (const unsigned long int nb_generations);
                virtual void run (const Swarm& swarm,const unsigned long int nb_generations);

                //Partial execution
                virtual void StartUp();
                virtual void StartUp(const Swarm& swarm);

                virtual void DoStep();

                void reset();
  };
}


#endif
