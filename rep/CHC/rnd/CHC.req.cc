#ifndef INC_REQ_CHC
#define INC_REQ_CHC
#include "CHC.hh"
#include <math.h>

//Parámetros para RND -------------------------------------------------------------

#define GRID_SIZE_X  	287    	//Artificial grid horizontal size.
#define GRID_SIZE_Y  	287    	//Artificial grid vertical size.
#define GRID_SIZE     82369		//Total grid size.
#define TRANS_NUMB   	59  		//Number of transmiters used.
#define TRANS_TOTAL   349     //Number of total transmiters.
			  			//49 transmiters distributed regulary...
						//... the rest is distributed randomly.

#define RANGE	      20   		//Transmiter cover range (square area)

//-------------------------------------------------------------------------------------------------------
#define TARGET_FITNESS	204.08   //Fitness to be achieved in current trial
//-------------------------------------------------------------------------------------------------------


#define MAX_FIT		204.08	//Fitness for optimum solution.
#define AVG_B		-4.878	//Coef. for linear penalty of fitness value ...
#define AVG_A		 4.878	// ... depending on avg. and dev. overlapping.
#define DEV_B		 0.0
#define DEV_A		 2.404

//---------------------------------------------------------------------------------

skeleton CHC
{


	//Array ------------------------------------------------------------------

	static short int trans_location[TRANS_TOTAL*2]=
	{20,20,   61,20,   102,20,   143,20,   184,20,   225,20,   266,20,
	 20,61,   61,61,   102,61,   143,61,   184,61,   225,61,   266,61,
	 20,102,  61,102,  102,102,  143,102,  184,102,  225,102,  266,102,
	 20,143,  61,143,  102,143,  143,143,  184,143,  225,143,  266,143,
	 20,184,  61,184,  102,184,  143,184,  184,184,  225,184,  266,184,
	 20,225,  61,225,  102,225,  143,225,  184,225,  225,225,  266,225,
	 20,266,  61,266,  102,266,  143,266,  184,266,  225,266,  266,266,

	169,180, 180,161, 160,233,    57,156,  158,145,  138,151,  160,32,
	165,36,  228,111, 251,181,   110,130,  286,19,    96,183,   91,133,
	 88,74,   93,56,   35,273,   152,198,  142,181,   37,117,  271,193,
	 49,109, 104,119, 110,54,     58,160,  135,204,  220,172,    4,141,
	160,244, 210,14,   36,37,     98,3,    134,226,  197,109,  101,17,
	112,230, 169,126, 215,44,    206,27,    18,90,    14,272,   40,134,
	160,150,  58,216, 170,37,    252,185,  246,142,  154,247,  180,128,
	188,55,  207,201, 134,15,     31,111,  166,34,   206,116,  223,261,
	 94,48,  227,179,  24,250,    46,26,   159,245,  279,56,   235,43,
	195,166, 165,241, 203,9,     190,73,    20,91,    25,200,  211,255,
	260,199, 262,66,  283,120,    16,76,    38,112,  201,172,  144,1,
	273,282, 230,285, 222,240,    70,132,   240,40,  202,147,  35,175,
	 24,41,    4,120,  88,114,    23,104,   274,142,  83,230, 281,265,
	248,58,  140,42,  136,185,    17,2,       9,42,  156,115, 216,32,
	242,104, 221,224,
241,113,224,229,261,56,96,220,79,158,137,180,104,147,273,262,182,205,40,174,
4,69,39,230,44,115,37,31,286,62,147,240,175,84,182,150,141,279,83,221,
151,220,114,255,81,101,231,263,20,272,150,24,55,190,255,100,18,5,131,18,
68,278,258,244,76,154,107,218,147,191,152,11,125,267,267,206,81,211,183,101,
197,47,126,252,237,94,65,256,100,197,274,168,188,246,126,265,114,233,196,261,
138,61,272,264,42,252,183,123,177,80,225,88,128,64,53,79,159,119,48,260,
29,36,142,218,282,268,196,109,215,105,84,66,167,70,43,210,36,227,47,213,
21,272,15,149,50,68,228,210,188,277,183,218,26,38,149,22,20,58,132,235,
164,216,14,45,286,58,255,36,286,15,249,20,1,264,170,51,46,112,262,235,
103,158,166,129,197,28,152,217,87,284,165,251,214,180,10,214,239,265,250,238,
281,213,259,282,191,142,47,238,255,22,186,71,180,65,201,90,94,66,21,181,
64,186,146,278,80,156,206,32,135,170,271,129,96,243,124,0,98,171,239,67,
193,138,138,87,204,52,178,11,118,199,193,183,99,52,174,179,209,94,212,58,
264,196,187,73,152,25,74,251,196,26,31,103,165,170,191,82,222,82,94,54,
282,1,237,95,54,125,275,263,219,200,34,196,110,222,270,262,247,58,227,157,
85,259,261,250,142,165,46,78,248,141,133,243,142,83,51,196,208,39,173,141,
240,207,51,63,143,34,39,103,93,267,260,178,240,234,142,96,113,189,174,74,
43,20,30,185,104,82,95,26,122,268,167,76,189,218,139,45,253,179,148,59,
160,122,238,113,70,93,209,183,282,97,257,39,117,1,224,222,84,32,248,206,
14,128,283,203,60,136,248,26,28,109,86,188,232,37,14,15,131,224,198,127};
 //Transmiters location array

 static short int grid[GRID_SIZE];

	// Problem ---------------------------------------------------------------

	Problem::Problem ():_dimension(0)
	{}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{
		os << endl << endl << "Number of Variables " << pbm._dimension
		   << endl;
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int i;

		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%d",&pbm._dimension);

		return is;
	}

	Problem& Problem::operator=  (const Problem& pbm)
	{
		return *this;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		if (_dimension!=pbm.dimension()) return false;	
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
		return maximize;
		//return minimize;
	}

	int Problem::dimension() const
	{
		return _dimension;
	}

	Problem::~Problem()
	{}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),_var(pbm.dimension())
	{}


	const Problem& Solution::pbm() const
	{
		return _pbm;
	}

	Solution::Solution(const Solution& sol):_pbm(sol.pbm())
	{
		*this=sol;
	}


	istream& operator>> (istream& is, Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			is >> sol._var[i];
		
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().dimension();i++)
			os << " " << sol._var[i];
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns << sol._var[i];
		return ns;
	}


	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol._var.size();i++)
			ns >> sol._var[i];
		return ns;
	}


 	Solution& Solution::operator= (const Solution &sol)
	{
		_var=sol._var;
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

	void Solution::initialize()
	{
		for (int i=0;i<_pbm.dimension();i++)
			_var[i]=rand_int(0,1);
	}


	/*double Solution::fitness () const
	{
        double fitness = 0.0;

		if(_var[0] == 2) return 0.0;

		for (int i=0;i<_var.size();i++)
			fitness += _var[i];

		return fitness;
	}*/

	double Solution::fitness()   const // EVALUATE1 modificado porque SINO NO FUNCIONA (violación de segmento)
	{
	register int x,y,x1,y1,k;
	double covered_points=0.0, cover_rate=0.0, fitness=0.0,alfa=2.0;
	int used_trans;

		if(_var[0] == 2) return 0.0;

		for (k=0;k<GRID_SIZE;k++)	// Initializing  the grid
 			grid[k]=0;


		used_trans=0;			// Updating covered points in the grid according ...
	 	for (k=0;k<_var.size();k++)	// with transmiter locations and calculating ...
 			{				// covered points.
	 	 	if (_var[k]!=0.0)
 	 			{
 		 		used_trans++;
	 	  		x=trans_location[k*2];
 	  			y=trans_location[k*2+1];
	  			for (x1=x-RANGE;x1<=x+RANGE;x1++)
					for (y1=y-RANGE;y1<=y+RANGE;y1++)
						if ((x1>=0)&(y1>=0)&(x1<GRID_SIZE_X)&(y1<GRID_SIZE_Y))
			 				{
			 				grid[x1*GRID_SIZE_X+y1]++;
			 				if (grid[x1*GRID_SIZE_X+y1]==1) covered_points++;
			 				}
   	 			}
			}

		cover_rate =(double) (100.0 * covered_points) / (GRID_SIZE);
		fitness = (cover_rate * cover_rate )/used_trans;

	 	return fitness;

	}


	/*double Solution::fitness()   const // MI VERSION GUARREADA DE FITNESS: AÑADE PENALIZACION POR INTERFERENCIA
	{
	register int x,y,x1,y1,k;
	double covered_points=0.0, interferenced_points=0.0, cover_rate=0.0, interference_rate=0.0, effective_cover_rate=0.0, fitness=0.0,alfa=2.0;
	int used_trans;


		for (k=0;k<GRID_SIZE;k++)	// Initializing  the grid
 			grid[k]=0;


		used_trans=0;			// Updating covered points in the grid according ...
	 	for (k=0;k<_var.size();k++)	// with transmiter locations and calculating ...
 			{				// covered points.
	 	 	if (_var[k]!=0.0)
 	 			{
 		 		used_trans++;
	 	  		x=trans_location[k*2];
 	  			y=trans_location[k*2+1];
	  			for (x1=x-RANGE;x1<=x+RANGE;x1++)
					for (y1=y-RANGE;y1<=y+RANGE;y1++)
						if ((x1>=0)&(y1>=0)&(x1<GRID_SIZE_X)&(y1<GRID_SIZE_Y))
			 				{
			 				grid[x1*GRID_SIZE_X+y1]++;
			 				if (grid[x1*GRID_SIZE_X+y1]==1) covered_points++;
							if (grid[x1*GRID_SIZE_X+y1]==2) interferenced_points++;
			 				}
   	 			}
			}

		cover_rate =(double) (100.0 * covered_points) / (GRID_SIZE);
		interference_rate =(double)(100.0*interferenced_points)/(GRID_SIZE);

		effective_cover_rate=cover_rate - interference_rate;
		fitness = (effective_cover_rate * effective_cover_rate )/used_trans;

	 	return fitness;

	}*/






	char *Solution::to_String() const
	{
		return (char *)_var.get_first();
	}


	void Solution::to_Solution(char *_string_)
	{
		int *ptr=(int *)_string_;
		for (int i=0;i<_pbm.dimension();i++)
		{
			_var[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return (_pbm.dimension() * sizeof(int));
	}

	int Solution::lengthInBits() const
	{
		return _pbm.dimension();
	}

	void Solution::flip(const int index)
	{
			_var[index] = 1 - _var[index]; 
	}

	bool Solution::equalb(const int index,Solution &s)
	{
		return _var[index] == s._var[index];
	}

	void Solution::swap(const int index, Solution &s)
	{
		int aux = s._var[index];
		s._var[index] = _var[index];
		_var[index] = aux;		
	}

	void Solution::invalid()
	{
			_var[0] = 2;
	}

	int& Solution::var(const int index)
	{
		return _var[index];
	}


	Rarray<int>& Solution::array_var()
	{
		return _var;
	}

	Solution::~Solution()
	{}

	// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{







		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;
		os << "	Trial	best cost	iteration best cost	Initial T	T best cost		t best cost		t spent" << endl;

		double best_cost=0.0;
		int evaluation_best_found=0;
		int iteration_best_found=0;
		double time_best_found=0.0;


		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << "\t" <<  userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t" << userstat.result_trials[i].worst_cost_trial
			   << "\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t" << userstat.result_trials[i].nb_iteration_best_found_trial
			   
			   
			   << "    \t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;

			   best_cost = best_cost + userstat.result_trials[i].best_cost_trial;
			   evaluation_best_found = evaluation_best_found + userstat.result_trials[i].nb_evaluation_best_found_trial;
			   iteration_best_found = iteration_best_found + userstat.result_trials[i].nb_iteration_best_found_trial;
			   time_best_found = time_best_found + userstat.result_trials[i].time_best_found_trial;
		}
		best_cost = best_cost / userstat.result_trials.size();
		evaluation_best_found = (int) (evaluation_best_found / userstat.result_trials.size());
		iteration_best_found = (int) iteration_best_found / userstat.result_trials.size();
		time_best_found = time_best_found / userstat.result_trials.size();

		os << endl << "------------------------------------------------------------------" << endl;
		os << endl << "\tMEDIA\t" << best_cost << "\t\t\t\t" << evaluation_best_found << "\t\t\t" << iteration_best_found << "\t\t\t" << time_best_found << endl;
		return os;

			
			
			
			
			
			
			
			
		
		

	}

	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		result_trials=userstats.result_trials;
		return (*this);
	}

	void UserStatistics::update(const Solver& solver)
	{
		if( (solver.pid()!=0) || (solver.end_trial()!=true)
		  || ((solver.current_iteration()!=solver.setup().nb_evolution_steps())
		       && !terminateQ(solver.pbm(),solver,solver.setup())))
			return;

		struct user_stat *new_stat;

		if ((new_stat=(struct user_stat *)malloc(sizeof(struct user_stat)))==NULL)
			show_message(7);
		new_stat->trial         				 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     			 = solver.worst_cost_trial();
		new_stat->best_cost_trial     			 = solver.best_cost_trial();
		new_stat->time_best_found_trial			 = solver.time_best_found_trial();
		new_stat->time_spent_trial 				 = solver.time_spent_trial();

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

	//  User_Operator:Intra_operator ---------------------------------------------------------

	User_Operator::User_Operator(const unsigned int _number_op):Intra_Operator(_number_op)
	{}

	void User_Operator::execute(Rarray<Solution*>& sols) const
	{}

	void User_Operator::setup(char line[MAX_BUFFER])
	{}

	Intra_Operator *User_Operator::create(const unsigned int _number_op)
	{
		return new User_Operator(_number_op);
	}

	ostream& operator<< (ostream& os, const User_Operator&  u_op)
	{
		 os << "User Operator.";
		 return os;
	}

	void User_Operator::RefreshState(const StateCenter& _sc) const
	{}

	void User_Operator::UpdateFromState(const StateCenter& _sc)
	{}

	User_Operator::~User_Operator()
	{}


// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
		return (solver.best_cost_trial() >= TARGET_FITNESS);
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


