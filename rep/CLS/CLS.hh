
/************************************************
***				  	      ***
***  Cooperative Local Search Skeleton  v1.0  ***
***  Developed by: Carlos Cotta Porras        ***
***                                           ***
************************************************/


#ifndef INC_CLS
#define INC_CLS

#include "Mallba/mallba.hh"
#include "Mallba/States.hh"



#include "SA/SA.hh"
using skeleton SA;
#define BaseSkeleton SA



skeleton CLS {

// Required items---------------------------------------------
  requires class UserStatistics;

// Provided items---------------------------------------------
  provides class SetUpParams;
  provides class Statistics;
  provides class Solver;
  provides class Solver_Seq;
  provides class Solver_Lan;
  provides class Solver_Wan;
 #define BaseSolution    BaseSkeleton::Solution
 #define BaseProblem     BaseSkeleton::Problem
 #define BaseSolver      BaseSkeleton::Solver_Seq
 #define BaseSetUpParams BaseSkeleton::SetUpParams





 // UserStatistics ----------------------------------------------------------------------------


  requires class UserStatistics {
  	public:

		UserStatistics ();
		~UserStatistics();

		friend ostream& operator<< (ostream& os, const UserStatistics& usertats);
	     // friend NetStream& operator << (NetStream& ns, const UserStatistics& usertats);
	     // friend NetStream& operator >> (NetStream& ns, UserStatistics& usertats);

		UserStatistics& operator= (const UserStatistics& userstats);
		void update(const Solver_Seq& solver);
		void clear();
 };



  // SetUpParams -------------------------------------------------------------------------------

  provides class SetUpParams {

	private:

		unsigned int    _independent_runs;
		unsigned int    _max_evaluations;
		unsigned int    _number_of_solvers;
		unsigned int    _execution_granularity;
		BaseSetUpParams _baseSetUp;

	public:
		SetUpParams ();
		~SetUpParams(); 

		friend ostream& operator<< (ostream& os, const SetUpParams& setup);
		friend istream& operator>> (istream& is, SetUpParams& setup);

	     // friend NetStream& operator << (NetStream& ns, const SetUpParams& setup);
	     // friend NetStream& operator >> (NetStream& ns, SetUpParams& setup);

		const unsigned int independent_runs() const;
		const unsigned int max_evaluations() const;
		const unsigned int number_of_solvers() const;
		const unsigned int execution_granularity() const;
		const BaseSetUpParams baseSetUp() const; 
		
		void independent_runs(const unsigned int val);
		void max_evaluations(const unsigned int val);
		void number_of_solvers(const unsigned int val);
		void execution_granularity(const unsigned int val);
		void baseSetUp(const BaseSetUpParams& val);

  };


 // Statistics ---------------------------------------------------------------------------------

  provides class Statistics {
	private:
		struct stat {
			unsigned int trial;
			unsigned int nb_evaluations;
			double best_cost;
			double current_cost;
		};

		Rlist<struct stat> stats_data;


	public:

		Statistics();
		~Statistics(); 

		friend ostream& operator<< (ostream& os, const Statistics& stats);
	     // friend NetStream& operator << (NetStream& ns, const Statistics& stats );
	     // friend NetStream& operator >> (NetStream& ns, Statistics& stats );

		Statistics& operator= (const Statistics& stats);
		void update(const Solver_Seq& solver);
		void clear();
	};




// Solver  ---------------------------------------------------------------------------------

  provides class Solver {
	protected:
		UserStatistics 	   _userstat;
		Statistics         _stat;
		StateCenter        _sc;
		BaseSolver**        solvers;
		const BaseProblem&  problem;
		const SetUpParams&  params;

		State_Vble         _current_trial;
		State_Vble  	   _current_iteration;
		State_Vble         _current_best_solution;
		State_Vble         _current_best_cost;
		State_Vble         _current_solution; 
		State_Vble         _current_cost; 
		State_Vble         _global_best_solution;
		State_Vble         _global_best_cost;
		State_Vble         _display_state;

		const Direction    _direction;


		void KeepHistory(const BaseSolution& sol, const double curfit) ;

    public:

		// Constructor - Destructor -------------------------

		Solver (const BaseProblem& pbm, const SetUpParams& setup); 
		virtual ~Solver ();


		// Execution methods --------------------------------

		// Full execution
		virtual void run () =0;
		virtual void run (unsigned long int nb_evaluations) =0;
		virtual void run (const BaseSolution& sol, unsigned long int nb_evaluations) =0;

		// Partial execution
		virtual void StartUp () =0;
		virtual void StartUp (const BaseSolution& sol) =0;
		virtual void DoStep () =0;


		// Statistics handling ------------------------------

		const Statistics& statistics() const;
		const UserStatistics& userstatistics () const;


		// State handling -----------------------------------

		void RefreshState();
		void UpdateFromState();
		StateCenter* GetState();


		unsigned int current_trial() const;
		unsigned int current_iteration() const;
		BaseSolution current_best_solution() const;
		double current_best_cost() const;
		BaseSolution current_solution() const; 
		double current_cost() const; 
		BaseSolution global_best_solution() const;
		double global_best_cost() const;
		int display_state() const;

		void current_trial(const unsigned int value);
		void current_iteration(const unsigned int value);
		void current_best_solution(const BaseSolution& sol);
		void current_best_cost(const double value);
		void current_solution(const BaseSolution& sol); 
		void current_cost(const double value); 
		void global_best_solution(const BaseSolution& sol);
		void global_best_cost(const double value);
		void display_state(const int value);
	
		void show_state() const;

  };



  provides class Solver_Seq: public Solver {

	public:
		// Constructor - Destructor -------------------------

		Solver_Seq (const BaseProblem& pbm, const SetUpParams& setup);
		virtual ~Solver_Seq ();

		// Execution methods --------------------------------

		// Full execution
		void run ();
		virtual void run (unsigned long int max_evaluations);
		virtual void run (const BaseSolution& sol, unsigned long int max_evaluations);

		// Partial execution
		virtual void StartUp ();
		virtual void StartUp (const BaseSolution& sol);
		virtual void DoStep ();  
  };


  provides class Solver_Lan: public Solver {

	public:

		// Constructor - Destructor -------------------------
		Solver_Lan (const BaseProblem& pbm, const SetUpParams& setup);
		virtual ~Solver_Lan ();

		// Execution methods --------------------------------

		// Full execution
		void run ();

  };


  provides class Solver_Wan: public Solver {

	public:

		// Constructor - Destructor -------------------------
		Solver_Wan (const BaseProblem& pbm, const SetUpParams& setup);
		virtual ~Solver_Wan ();

		// Execution methods --------------------------------

		// Full execution
		void run ();

  };



  bool TerminateQ (const BaseProblem& pbm, const Solver_Seq& solver, const SetUpParams& setup); 

};

#endif
