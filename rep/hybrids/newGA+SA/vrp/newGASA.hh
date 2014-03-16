#ifndef INC_newGA_SA
#define INC_newGA_SA

#include "Mallba/mallba.hh"
#include "Mallba/States.hh"
#include "Mallba/Rarray.h"
#include "Mallba/time.hh"
#include "Mallba/netstream.hh"
#include <math.h>
#include <string.h>
#include <fstream.h>

skeleton newGASA
{
  // Generales
  requires class Problem;
  requires class Solution;

  // Especificos de GA
  requires class Intra_Operator;
  requires class Crossover_PMX1;
  requires class Crossover_PMX2;
  requires class Crossover_OX;
  requires class Crossover_CX;
  requires class Crossover_ER;
  requires class Mutation;

  // Especifico de SA
  requires class Movement;

  provides class SetUpParam;
  provides class Improve;
  provides class Solver;
  provides class Solver_Seq_v1;
  provides class Solver_Lan_v1;
  provides class Solver_Wan_v1;
  provides class Solver_Seq_v2;
  provides class Solver_Lan_v2;
  provides class Solver_Wan_v2;
  provides class Solver_Seq_v3;
  provides class Solver_Lan_v3;
  provides class Solver_Wan_v3;

  // Problem ----------------------------------------------------------------------------

  requires class Problem
  {
	public:
		Problem ();
		~Problem ();

		friend ostream& operator<< (ostream& os, const Problem& pbm);
		friend istream& operator>> (istream& is, Problem& pbm);

		Problem& operator=  (const Problem& pbm);
		bool     operator== (const Problem& pbm) const;
		bool     operator!= (const Problem& pbm) const;

 		Direction direction () const;

		int nCustomers() const;
		int capacity() const;
		double maxRouteTime() const;
		double distance(const int i, const int j) const;
		int demand(const int i) const;
		int service_time() const;

	private:
		void genDistances(int *x,int *y);

		int _nCustomers;
		int _capacity;
		double **_distance;
		int _service_time;
		double _maxRouteTime;
		int *_demand;
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
		void to_Solution(char *_vertex_);
		unsigned int size() const;

		void initialize();
		double fitness () const;

		int & pos(const int index);
		Rarray<int> & routes();
		void print();

	private:
		Rarray<int> _routes;
		const Problem& _pbm;
 };

// Intra_Operator ( clase abstracta ) --------------------------------------------------------------

  requires class Intra_Operator
  {
	protected:
		unsigned int _number_operator;
		float *probability;

	public:
		Intra_Operator(const unsigned int _number_op);
		virtual ~Intra_Operator();

		static Intra_Operator *create(const unsigned int _number_op);
		friend ostream& operator<< (ostream& os, const Intra_Operator& intra);

		virtual void execute(Rarray<Solution*>& sols) const=0;
		virtual void setup(char line[MAX_BUFFER]) = 0;
		unsigned int number_operator() const;

		virtual void RefreshState(const StateCenter& _sc) const=0;
		virtual void UpdateFromState(const StateCenter& _sc)=0;
  };

// Crossover_PMX1 ----------------------------------------------------------------------------------

  requires class Crossover_PMX1: public Intra_Operator
  {
	public:
		Crossover_PMX1();
		virtual ~Crossover_PMX1();

		friend ostream& operator << (ostream& os, const Crossover_PMX1&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

	private:
		int newValue(const int oldValue,const int l1,const int l2, Rarray<int> & s1, Rarray<int> & s2) const;
		void complete(Solution &s) const;
  };

// Crossover_PMX2 ----------------------------------------------------------------------------------

  requires class Crossover_PMX2: public Intra_Operator
  {
	public:
		Crossover_PMX2();
		virtual ~Crossover_PMX2();

		friend ostream& operator << (ostream& os, const Crossover_PMX2&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

	private:
		int newValue(const int oldValue,const Rarray<int> & n) const;
		void complete(Solution &s) const;
  };

// Crossover_OX ----------------------------------------------------------------------------------

  requires class Crossover_OX: public Intra_Operator
  {
	public:
		Crossover_OX();
		virtual ~Crossover_OX();

		friend ostream& operator << (ostream& os, const Crossover_OX&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
  };

// Crossover_CX ----------------------------------------------------------------------------------

  requires class Crossover_CX: public Intra_Operator
  {
	public:
		Crossover_CX();
		virtual ~Crossover_CX();

		friend ostream& operator << (ostream& os, const Crossover_CX&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

	private:
		Solution *crossAux(Solution &sol1,Solution &sol2) const;
		void create_map(int **m,int *c_m,int **p,int *c_p,const Solution &s1,const Solution s2) const;
		void remove_of_map(int c, int **m,int *c_m, int **p, int *c_p) const;
		int choose_random(int *c,int size) const;
		int choose_minimun(int p,int *m,int *c) const;
		bool is_in(int p,int *m,int size) const;
		void remove_pos(int e,int l,int **m,int *c_m) const;
  };

// Crossover_ER ----------------------------------------------------------------------------------

  requires class Crossover_ER: public Intra_Operator
  {
	public:
		Crossover_ER();
		virtual ~Crossover_ER();

		friend ostream& operator << (ostream& os, const Crossover_ER&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

	private:
		Solution *crossAux(Solution &sol1,Solution &sol2) const;
		void create_map(int **m,int *c_m,int **p,int *c_p,Solution &s1,Solution s2) const;
		void remove_of_map(int c, int **m,int *c_m, int **p, int *c_p) const;
		int choose_random(int *c,int size) const;
		int choose_minimum(int p,int *m,int *c) const;
		bool is_in(int p,int *m,int size) const;
		void remove_pos(int e,int l,int **m,int *c_m) const;
  };

// Mutation ----------------------------------------------------------------------------------

  requires class Mutation: public Intra_Operator
  {
	public:
		Mutation();
		virtual ~Mutation();

		friend ostream& operator<< (ostream& os, const Mutation&  mutation);

		void mutate(Solution& sol) const;
		// applies mutation over all solutions in array sols
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);

  };

// Movement for SA ----------------------------------------------------------------------------------

  requires class Movement
  {
	public:
		Movement();
		~Movement();

		void Apply(Solution& sol) const;
  };


// SetUpParam -------------------------------------------------------------------------------

  provides class SetUpParams
  {
	private:
		unsigned int _select_to_SA;
		unsigned int _parameter_select_to_SA;
		unsigned int _number_solutions_SA;

		char *_nGA_Cfg;
		char *_SA_Cfg;
		char *_pbm;
		char *_res;
		char *_path;


	public:
		SetUpParams (char *path);
		SetUpParams ();

		friend ostream& operator<< (ostream& os, const SetUpParams& setup);
		friend istream& operator>> (istream& is, SetUpParams& setup);

		const unsigned int  select_to_SA() const;
		const unsigned int  parameter_select_to_SA() const;
		const unsigned int  number_solutions_SA() const;
		const char*	      	nGA_Cfg() const;
		const char*	      	SA_Cfg() const;
		const char*	      	pbm() const;
		const char*	      	res() const;
		const char*			path() const;

		void select_to_SA(const unsigned int val);
		void parameter_select_to_SA(const unsigned int val);
		void number_solutions_SA(const unsigned int val);
		void nGA_Cfg(char *val);
		void SA_Cfg(char *val);
		void pbm(char *val);
		void res(char *val);
		void path(char *val);

		~SetUpParams();
  };

// Solver ------------------------------------------------------------------------
  class Solver
  {
   	public:

		Solver();
		virtual void run(SetUpParams& params,int argc,char** argv)=0;
		virtual ~Solver();
  };


// Solver_Seq_v1 ------------------------------------------------------------------------

class Solver_Seq_v1: public Solver
{

	public:
		Solver_Seq_v1();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Seq_v1();
};

// Solver_Lan_v1 ------------------------------------------------------------------------

class Solver_Lan_v1: public Solver
{
 	public:
		Solver_Lan_v1();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Lan_v1();
};

// Solver_Wan_v1 ------------------------------------------------------------------------

class Solver_Wan_v1: public Solver
{
 	public:
		Solver_Wan_v1();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Wan_v1();
};

// Solver_Seq_v2 ------------------------------------------------------------------------

class Solver_Seq_v2: public Solver
{
	public:

		Solver_Seq_v2();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Seq_v2();
};

// Solver_Lan_v2 ------------------------------------------------------------------------

class Solver_Lan_v2: public Solver
{

	public:
		Solver_Lan_v2();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Lan_v2();
};

// Solver_Wan_v2 ------------------------------------------------------------------------

class Solver_Wan_v2: public Solver
{
 	public:
		Solver_Wan_v2();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Wan_v2();
};

// Solver_Seq_v3 ------------------------------------------------------------------------

class Solver_Seq_v3: public Solver
{
 	public:
		Solver_Seq_v3();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Seq_v3();
};

// Solver_Lan_v3 ------------------------------------------------------------------------

class Solver_Lan_v3: public Solver
{
	public:
		Solver_Lan_v3();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Lan_v3();
};

// Solver_Wan_v3 ------------------------------------------------------------------------

class Solver_Wan_v3: public Solver
{
 	public:
		Solver_Wan_v3();
		virtual void run(SetUpParams& params,int argc,char** argv);
		virtual ~Solver_Wan_v3();
};

}

#endif

