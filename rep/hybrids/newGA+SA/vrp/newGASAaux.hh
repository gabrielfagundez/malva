#ifndef AUX_newGASA
#define AUX_newGASA

#include "SA.hh"
#include "newGASA.hh"

skeleton newGASA
{

//  Improve: Intra_operator -------------------------------------------------------------

  provides class Improve: public Intra_Operator
  {
		private:

			SA::Solver* my_sa;
			mutable unsigned long _evaluations;

    	public:

			Improve();
			virtual ~Improve();
	
			friend ostream& operator<< (ostream& os, const Improve&  improve);

			void do_improve(Solution& sol) const;
			// apply mutation over all solutions in array sols
			virtual void execute(Rarray<Solution*>& sols) const;

			virtual void setup(char line[MAX_BUFFER]);

			virtual void RefreshState(const StateCenter& _sc) const;
			virtual void UpdateFromState(const StateCenter& _sc);

			void set_sa(SA::Solver *sa);
			SA::Solver *get_sa() const;
 };
}

#endif
