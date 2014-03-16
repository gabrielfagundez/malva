/***************************************************************************
			 Ant Colony System v1.10
			 developed by Guillermo Daniel Ordóñez
			 email gordonez@unsl.edu.ar
****************************************************************************/

#include <stdio.h>
#include "Mallba/mallba.hh"
#include "Mallba/Rarray.h"
#include "Mallba/Rlist.h"
#include "Mallba/States.hh"
#include "Mallba/netstream.hh"
#include "Mallba/time.hh"
#include "Mallba/random.hh"
#include <math.h>
#include <strings.h>
#include <error.h>
#include <fstream.h>

skeleton ACO
{

    double random_double(double _min, double _max);

    provides class SetUpParams;
    provides class Statistics;
    provides class Colony;
    provides class Trail;
    provides class STrail;
    provides class MTrail;
    provides class Inter_Operator;
    provides class Migration;
    provides class Solver;
    provides class Solver_Seq;
    provides class Solver_Lan;
    provides class Solver_Wan;
    provides class StopCondition;
    provides class Operator_Pool;
    provides class Ant;

    requires class Problem;
    requires class Solution;
    requires class Neighborhood;
    requires class Eta;
    requires class UserStatistics;
    requires class StopCondition_1;
    int test_convergency(const Colony &colony);
    bool terminateQ (const Problem& pbm, const Solver& solver, const SetUpParams& setup);

/************************************************************************************
									     requires
************************************************************************************/
// -+----1----+----2----+----3----+----4----+----5----+----6----+--- (Problem)
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

	int get_node_count() const;
	int max_path_length() const;
	int path_count_from(int node) const;
	int get_destiny(int from, int through) const;

	// Problem-dependent information
  	int Time (int index) const;
  	int Weigth (int index) const;
  	int DeadLine (int index) const;
	int Total_Weigth() const;
    private:	
	// Problem-dependent information
	int _size;
	int _tw;
	int *_t;
	int *_w;
	int *_dl;
    };

// -+----1----+----2----+----3----+----4----+----5----+----6----+-- (Solution)
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
	unsigned int size() const;

	double fitness () const;
	Ant * ant() const;
	void ant(Ant *new_ant); 

    private:
	class Ant * _ant;
	const Problem& _pbm;
	bool &_evaluated;
	int &_fitness;
	char * _string;
	// Problem-dependent information

    };

// -+----1----+----2----+----3----+----4----+----5----+----6----+- (Neighborhood)
    requires class Neighborhood
    {
    public:
	Neighborhood (Ant *ant, const Problem &pbm, const SetUpParams& setup);
	~Neighborhood();
	int size()const;
	void move_to_node (int from, int through);

	int operator[](int pos) const;
    private:
	const Problem& _pbm;
	// Problem-dependent information
	struct nodo{
	    int new_deadline;
	    int freedom;
	    int next;
	    int prev;
	    bool planned;
	    bool feasible;
	};
	struct nodo * _data;
	int _size;
	int _first;
	int _last;
	int *_c_pos;
	int *_c_data;
	void suppress(int);

    };

// -+----1----+----2----+----3----+----4----+----5----+----6----+----7-- (Eta)
    requires class Eta
    {
    public:
	Eta(const Problem& pbm, const SetUpParams& setup);
	~Eta();
	Eta& operator= (const Eta& eta);
	void move_to_node(int from, int through);
	double get_value (int from, int through);
	void reset (Ant *ant, Neighborhood *N);
    private:
	const Problem &_problem;
	// Problem-dependent information
	double * _data;	
	double _norm;
	int _node_count;
    };



// -+----1----+----2----+----3----+----4----+----5----+----6- (UserStatistics)

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
	ofstream TrailFile;

    public:
        UserStatistics ();
        ~UserStatistics();

        friend ostream& operator<< (ostream& os, const UserStatistics& usertats);

        UserStatistics& operator= (const UserStatistics& userstats);
        void update(const Solver& solver);
        void clear();
    };

/************************************************************************************
									     provides
************************************************************************************/

// -+----1----+----2----+----3----+----4----+----5----+----6----+--- (Ant)
    provides class Ant
    {
    public:
	Ant(const Problem& pbm);
	Ant(const class Ant& ant);
	~Ant();

	const class Problem& problem() const;

	int operator[](int pos);
	int size() const;
	
	void move_to_node(int from, int through);

	Ant& operator=  (const Ant& ant);
	bool operator== (const Ant& ant) const;
	bool operator!= (const Ant& ant) const;

	char *to_String() const;
	void to_Ant(char *cadena);
	int string_size() const;

	friend ostream& operator<< (ostream& os, const Ant &ant);
	friend istream& operator>> (istream& is, Ant &ant);

	friend NetStream& operator<< (NetStream& ns, const Ant &ant);
	friend NetStream& operator>> (NetStream& ns, Ant &ant);

    private:
	char *_string;
	int _L_count;
	int *_L;
	const class Problem &_pbm;
    };



// -+----1----+----2----+----3----+----4----+----5----+----6----+---- (Trail)
    provides class Trail
    {
    protected:

	virtual void print(ostream& os) const;
	const Problem &_pbm;
    public:
	Trail(const class Problem& pbm, const class SetUpParams& setup);
	virtual ~Trail();
	virtual void update(Solution * sol, double c1, double c2)=0;
	virtual void update(int from, int to)=0;
	virtual void reset()=0;
	virtual void evaporate(double e)=0;
	virtual double get_value (int from, int to)=0;
	virtual void set_best_fitness(double fitness);
	friend ostream& operator<< (ostream& os, const Trail& trail );
    };


    provides class STrail: public Trail
    {
    private:
	virtual void print(ostream& os) const;
	Rarray<Rarray<double> *>* _pheromonas;
	double _t0;
	double _xi;
	double _min;
	double _max;
	double _a;
	double _rho;
	int _min_max;
    public:
	STrail(const class Problem& pbm, const class SetUpParams& setup );
	STrail(const class Problem& pbm, const class SetUpParams& setup, STrail * src);
	virtual ~STrail();	
	virtual void update(Solution * sol, double c1, double c2);
	virtual void update(int from, int to);
	virtual void reset();
	virtual void evaporate(double e);
	virtual double get_value (int from, int to);
	virtual void set_best_fitness(double fitness);
	double c_inv(double fitness);
    };


    provides class MTrail: public Trail
    {
    private:
	virtual void print(ostream& os) const;
	void _update(Ant * ant, double dir);
	Rarray<Rarray<double> *>* _pheromonas;
	int _max_pop_count;
	Rarray<Solution *>* _sol;
	double _worst_fitness;
	int _pop_count;
	double _p1;
	double _pi;
	int _rm;
	int _first;
	const Colony * _col;
    public:
	MTrail(const class Problem& pbm, const class SetUpParams& setup, const Colony * col);
	MTrail(const class Problem& pbm, const class SetUpParams& setup, const Colony * col, MTrail * src);
	virtual ~MTrail();	
	virtual void update(Solution * sol, double c1, double c2);
	virtual void update(int from, int to);
	virtual void reset();
	virtual void evaporate(double e);
	virtual double get_value (int from, int to);
    };

// -+----1----+----2----+----3----+----4----+----5----+----6----+---- (Colony)
    provides class Colony
    {

    private:

	const Problem &_problem;
	const SetUpParams& _setup;
	unsigned long nb_steps;
	Rarray<Solution*> *_sols;  
	Rarray<double> *_fitness_values;
	Trail *_trail;

	float _start_trial_time;
	float _best_solution_time;


	double _rank;
	bool _elitist;
	double 	 	_rho;
	unsigned int 	_w;

	bool   		_check_convergency;
	double  	_q0;

	double 		_alpha;
	double 		_beta;

	double          _e1;
	double          _e2;

	Eta      *_eta;
	Solution &_best_solution; 
	double   _best_cost;
	double   _worst_cost;

	Solution &_global_best_solution; 
	double   _global_best_cost;
	float _global_best_solution_time;

    public:

	Colony(const Problem& pbm,const SetUpParams& setup);
	~Colony();


	const Problem & problem() const;
	float start_trial_time() const;
	float best_solution_time() const;


	double          rank() const;
	bool            elitist() const;
	double 	 	rho() const;
	unsigned int 	w() const;
	bool   		check_convergency() const;
	double  	q0() const;
	double 		alpha() const;
	double 		beta() const;



	friend ostream& operator<< (ostream& os, const Colony& colony);
	friend istream& operator>> (istream& is, Colony& colony);

	Colony& operator= (const Colony& colony);
	const SetUpParams& setup() const;
	void initialize();
	void advance(); 
	void generate_solution(Solution* sol);
	void daemon();

	// interchange solutions between colony
	void interchange(const unsigned long current_steps, NetStream& channel);
	Trail& trail() const;

	Solution& solution(const unsigned int index) const;
	double fitness(const unsigned int index) const;	

	Solution &best_solution() const;
	Rarray<Solution*> &solutions() const;  
	
	double best_cost() const;
	double worst_cost() const;
    };


// -+----1----+----2----+----3----+----4----+----5----+----6----+ (Statistics)
    provides class Statistics
    {
    private:
        struct stat
        {
            unsigned int trial;
            unsigned long nb_steps;
            double best_cost;
            double global_best_cost;
            double average_cost;
            double standard_deviation;
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

// -+----1----+----2----+----3----+----4----+----5----+----6---- (SetUpParams)
    provides class SetUpParams
    {
    private:
	unsigned int    _independent_runs;
	unsigned long   _nb_evolution_steps;
	unsigned int    _colony_size;
	bool 		_display_state;

	unsigned long   _refresh_global_state;
	bool 	 	_synchronized;
	unsigned int 	_check_asynchronous;

	unsigned int  	_trail;
	unsigned int    _trail_pop_size;
	double          _pi;
	double          _p1;
	double 	 	_rho;
	unsigned int 	_w;
	bool  	 	_elitist;
	double  	_a;
	double          _max;
	unsigned int    _min_max;
	unsigned int    _rm;
	bool   		_check_convergency;
	double  	_q0;
	double  	_xi;
	double  	_t0;
	double          _e1;
	double          _e2;
	double          _rank;

	double 		_alpha;
	double 		_beta;

	Rlist<char > _eta;
	Rlist<char > _neighborhood;
	Rlist<unsigned int> _inter_operators;
	Operator_Pool& _pool;

    public:

	SetUpParams (Operator_Pool& pool);
	Operator_Pool& pool() const;

	friend ostream& operator<< (ostream& os, const SetUpParams& setup);
	friend istream& operator>> (istream& is, SetUpParams& setup);

	const unsigned int    	independent_runs() const;
	const unsigned long   	nb_evolution_steps() const;
	const unsigned int   	colony_size() const;
	const bool 	  	display_state() const;
	const unsigned long   	refresh_global_state() const;
	const bool 		synchronized() const;
	const unsigned int 	check_asynchronous() const;

	const double 		alpha() const;
	const double 		beta() const;
	
	const unsigned int  	trail() const;
	const unsigned int      trail_pop_size() const;
	const double            pi() const;
	const double            p1() const;
	const double 	 	rho() const;
	const double 	 	e1() const;
	const double 	 	e2() const;
	const unsigned int 	w() const;
	const bool  	 	elitist() const;
	const double  		a() const;
	const double            max() const;
	const unsigned int      min_max() const;
	const unsigned int      rm() const;
	const bool   		check_convergency() const;
	const double  		q0() const;
	const double  		xi() const;
	const double  		t0() const;
	const Rlist<char >&      eta() const;
	const Rlist<char >&      neighborhood() const;
	const double            rank() const;
       

	void independent_runs(const unsigned int val);
	void nb_evolution_steps(const unsigned long val);
	void colony_size(const unsigned int val);
	void display_state(const bool val);
	void refresh_global_state(const unsigned long val);
	void synchronized(const bool val);
	void check_asynchronous(const unsigned int val);

	void trail(const unsigned int val);
	void trail_pop_size(const unsigned int val);
	void pi(const double val);
	void p1(const double val);
	void rho(const double val);
	void e1(const double val);
	void e2(const double val);
	void w(const unsigned int val);
	void elitist(const bool val);
	void a(const double val);
	void max(const double val);
	void min_max(const unsigned int val);
	void rm(const unsigned int val);

	void check_convergency(const bool val);
	void q0(const double val);
	void xi(const double val);
	void t0(const double val);
	void eta(char *val);
	void neighborhood(char *val);
	void rank(double val);

	
	void alpha(const double val);
	void beta(const double val);

	const unsigned int inter_operator_index(const unsigned int index) const;
	const unsigned int inter_operators_size() const;


	void RefreshState(const StateCenter& _sc) const;
	void UpdateFromState(const StateCenter& _sc) const;

	~SetUpParams();

    };

// -+----1----+----2----+----3----+----4----+----5----+----6- (Inter_Operator)
    provides class Inter_Operator
    {
    protected:

	unsigned int migration_rate;
	unsigned int migration_size;
	unsigned int _number_operator;
	const Direction direction;

    public:

	Inter_Operator(const unsigned int _number_op, const Direction dir);
	virtual ~Inter_Operator();

	friend ostream& operator<< (ostream& os, const Inter_Operator& inter);

	virtual void execute(Colony& col,const unsigned long current_steps,NetStream& _netstream,const bool synchronized,const unsigned int check_asyncrhonous) const=0;
	virtual void setup(char line[MAX_BUFFER]);
	unsigned int number_operator() const;

	virtual void RefreshState(const StateCenter& _sc) const;
	virtual void UpdateFromState(const StateCenter& _sc);
    };

// -+----1----+----2----+----3----+----4----+----5- (Inter_Operator/Migration)
    provides class Migration: public Inter_Operator
    {
    private:
	int mode;
	char net_file[MAX_BUFFER];
	bool &_initialized;
	Rarray<int> *to;
	Rarray<int> *from;
    public:

	Migration(const Direction dir);
	virtual ~Migration();
	void reciv(int from,Colony& col,NetStream& _netstream,const bool synchronized) const;
	void send(int to,Colony& col,NetStream& _netstream) const;

	virtual void setup(char line[MAX_BUFFER]);

	friend ostream& operator<< (ostream& os, const Migration& migration);

	virtual void execute(Colony& col,const unsigned long current_steps,NetStream& _netstream,const bool synchronized,const unsigned int check_asyncrhonous) const;
	void initialize(NetStream& _netstream) const;
    };

// -+----1----+----2----+----3----+----4----+----5----+----6-- (Operator_Pool)
    provides class Operator_Pool
    {
    private:

	Rlist<Inter_Operator> 	_inter_operators;

    public:

	Operator_Pool(const Problem& pbm);
	~Operator_Pool();
	Inter_Operator& inter_operator(const unsigned int index) const;
	const Rlist<Inter_Operator>& inter_operators() const;

    };


// -+----1----+----2----+----3----+----4----+----5----+----6-- (StopCondition)
    provides class StopCondition
    {
    public:

	StopCondition();
	virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)=0;
	~StopCondition();
    };

// -+----1----+----2----+----3----+----4----+--- (StopCondition/StopCondition)
    requires class StopCondition_1 : public StopCondition
    {
    public:

	StopCondition_1();
	virtual bool EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup);
	~StopCondition_1();
    };

// -+----1----+----2----+----3----+----4----+----5----+----6----+---- (Solver)

    provides class Solver
    {
    protected:
	const Problem&     problem;
	const SetUpParams& params;
	UserStatistics _userstat;
	Statistics     _stat;
	StateCenter _sc;
	Colony current_colony;

	double 	   best_cost;
	double 	   worst_cost;
	float      total_time_spent;
	float      time_spent_in_trial;
	float 	   start_trial;
	float 	   start_global;

	bool 	    _end_trial;

	State_Vble  _independent_runs;
	State_Vble  _current_trial;
	State_Vble  _current_step;

	State_Vble  _current_best_solution;
	State_Vble  _current_best_cost;
	State_Vble  _current_worst_cost;
	State_Vble  _current_time_spent;

	State_Vble  _best_solution_trial;
	State_Vble  _best_cost_trial;
	State_Vble  _worst_cost_trial;
	State_Vble  _step_best_found_in_trial;
	State_Vble  _time_best_found_trial;
	State_Vble  _time_spent_trial;

	State_Vble  _trial_best_found;
	State_Vble  _step_best_found;
	State_Vble  _global_best_solution;
	State_Vble  _global_best_cost;
	State_Vble  _global_worst_cost;
	State_Vble  _time_best_found;


	State_Vble  _trail;
	State_Vble  _trail_pop_size;
	State_Vble  _pi;
	State_Vble  _p1;

	State_Vble  _rho;
	State_Vble  _w;
	State_Vble  _elitist;
	State_Vble  _a;
	State_Vble  _max;
	State_Vble  _min_max;
	State_Vble  _rm;
	State_Vble  _check_convergency;
	State_Vble  _q0;
	State_Vble  _xi;
	State_Vble  _t0;

	State_Vble  _alpha;
	State_Vble  _beta;

  	State_Vble _migration_rate;
  	State_Vble _migration_size;

	State_Vble  _display_state;
	Solution   best_solution;

    public:
	Solver (const Problem& pbm, const SetUpParams& setup);
	virtual ~Solver ();

	virtual int pid() const;
	bool end_trial() const;
	void end_trial(bool et);

	// Execution methods --------------------------------

	// Full execution
	virtual void run () =0;
	virtual void run (const unsigned long int nb_steps) =0;
	virtual void run (const Colony& col,const unsigned long int nb_steps) =0;

	//Partial execution
	virtual void StartUp()=0;
	virtual void StartUp(const Colony& col )=0;

	virtual void DoStep()=0;

	// Statistics handling -------------------------------
	Statistics& statistics();
	UserStatistics& userstatistics ();
	const Colony& colony() const;
	const SetUpParams& setup() const;
	const Problem& pbm() const;

	// State handling ---------------------------------------

	void RefreshState();
	void RefreshCfgState();
	void UpdateFromState();
	void UpdateFromCfgState();
	StateCenter* GetState();

	unsigned int independent_runs () const;
	unsigned int current_trial() const;
	unsigned long current_step() const;
	Solution current_best_solution() const;
	double current_best_cost() const;
	double current_worst_cost() const;
	float  current_time_spent() const;
	Solution  best_solution_trial() const;
	double best_cost_trial() const;
	double worst_cost_trial() const;
	unsigned int step_best_found_in_trial() const;
	float time_best_found_trial() const;
	float time_spent_trial() const;
	unsigned int trial_best_found() const;
	unsigned int step_best_found() const;
	Solution global_best_solution() const;
	double global_best_cost() const;
	double global_worst_cost() const;
	float time_best_found() const;

	unsigned int  	trail() const;
	unsigned int 	trail_pop_size() const;
	double  	pi() const;
	double  	p1() const;
	double 	 	rho() const;
	unsigned int 	w() const;
	bool  	 	elitist() const;
	double  	a() const;
	double  	max() const;
	unsigned int 	rm() const;
	unsigned int 	min_max() const;
	bool   		check_convergency() const;
	double  	q0() const;
	double  	xi() const;
	double  	t0() const;
	double  alpha() const;
	double beta() const;


	int display_state() const;

  	unsigned int migration_rate() const;
  	unsigned int migration_size() const;

	void independent_runs(const unsigned int value);
	void current_trial(const unsigned int value);
	void current_step(const unsigned long value);
	void current_best_solution(const Solution& sol);
	void current_best_cost(const double value);
	void current_worst_cost(const double value);
	void current_time_spent(const float value);
	void best_solution_trial(const Solution& sol);
	void best_cost_trial(const double value);
	void worst_cost_trial(const double value);
	void step_best_found_in_trial(const unsigned int value);
	void time_best_found_trial(const float value);
	void time_spent_trial(const float value);
	void trial_best_found(const unsigned int value);
	void step_best_found(const unsigned int  value);
	void global_best_solution(const Solution& sol);
	void global_best_cost(const double value);
	void global_worst_cost(const double value);
	void time_best_found(const float value);
	void display_state(const int value);

	void trail(const unsigned int value);
	void trail_pop_size(const unsigned int value);
	void pi(const double value);
	void p1(const double value);
	void rho(const double value);
	void w(const unsigned int value);
	void elitist(const bool value);
	void a(const double value);
	void max(const double value);
	void min_max(const unsigned int value);
	void rm(const unsigned int value);
	void check_convergency(const bool value);
	void q0(const double value);
	void xi(const double value);
	void t0(const double value);
	void alpha(const double value);
	void beta(const double valuie);

  	void migration_rate(const unsigned int rate);
  	void migration_size(const unsigned int size);

	void show_state() const;
	void KeepHistory(const Solution& best_sol,const double best_cost,const double worst_cost,const float time_spent_trial,const float total_time_spent);
    };

 


    provides class Solver_Seq: public Solver
    {
    public:
	Solver_Seq ( const Problem& pbm, const SetUpParams& setup);
	virtual ~Solver_Seq ();

	// Execution methods ----------------------------

	// Full execution
	virtual void run ();
	virtual void run (const unsigned long int nb_steps);
	virtual void run (const  Colony& col,const unsigned long int nb_steps);

	//Partial execution
	virtual void StartUp();
	virtual void StartUp(const  Colony& col);

	virtual void DoStep();
    };

    provides class Solver_Lan: public Solver
    {
    private:
  	NetStream _netstream;
  	int mypid;

  	int receive_local_state();

  	unsigned int _current_trial;
  	unsigned long _current_step;
  	double _best_cost_trial;
  	Solution _best_solution_trial;
  	double _worst_cost_trial;
  	float _time_best_found_in_trial;
  	unsigned long _step_best_found_in_trial;


    public:
  	Solver_Lan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
  	virtual ~Solver_Lan ();
  	virtual int pid() const;
	NetStream& netstream();

  	// Execution methods -------------------------------

	// Full execution
	virtual void run ();
	virtual void run (const unsigned long int nb_steps);
	virtual void run (const Colony& col,const unsigned long int nb_steps);

	//Partial execution
	virtual void StartUp();
	virtual void StartUp(const Colony& col);

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

	int receive_local_state();

	unsigned int _current_trial;
	unsigned long _current_step;
	double _best_cost_trial;
	Solution _best_solution_trial;
	double _worst_cost_trial;
	float _time_best_found_in_trial;
	unsigned long _step_best_found_in_trial;


    public:
	Solver_Wan (const Problem& pbm, const SetUpParams& setup,int argc,char **argv);
	virtual ~Solver_Wan ();
	virtual int pid() const;
	NetStream& netstream();

	// Execution methods -------------------------------

	// Full execution
	virtual void run ();
	virtual void run (const unsigned long int nb_steps);
	virtual void run (const Colony& col,const unsigned long int nb_steps);

	//Partial execution
	virtual void StartUp();
	virtual void StartUp(const Colony& col);

	virtual void DoStep();

	//Communication
	void send_local_state_to(int _mypid);
	void check_for_refresh_global_state();
	void reset();
    };

};



