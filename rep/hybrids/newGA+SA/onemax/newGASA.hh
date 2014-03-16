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
  requires class Crossover;
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
		void to_Solution(char *_vertex_);
		unsigned int size() const;

		void initialize();
		double fitness ();

		int& var(const int index);
		Rarray<int>& array_var();

	private:
		Rarray<int> _var;
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

// Crossover ----------------------------------------------------------------------------------

  requires class Crossover: public Intra_Operator
  {
	public:
		Crossover();
		virtual ~Crossover();

		friend ostream& operator << (ostream& os, const Crossover&  cross);

		void cross(Solution &sol1,Solution &sol2) const;
		virtual void execute(Rarray<Solution*>& sols) const;
		virtual void setup(char line[MAX_BUFFER]);

		virtual void RefreshState(const StateCenter& _sc) const;
		virtual void UpdateFromState(const StateCenter& _sc);
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
