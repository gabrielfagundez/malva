#ifndef INC_REQ_newGA
#define INC_REQ_newGA
#include "newGA.hh"
#include <math.h>
#include "Util.h"
#include "AdjKeys.h"

skeleton newGA
{

	// Problem ---------------------------------------------------------------

	Problem::Problem ():numOfFragments(0),avgLength(0.0),cutoff(0),
					    fragmentLength(NULL),fragments(NULL),rcFragments(NULL),
						description(NULL)
	{
	}

	ostream& operator<< (ostream& os, const Problem& pbm)
	{		
		return os;
	}

	istream& operator>> (istream& is, Problem& pbm)
	{
		char buffer[MAX_BUFFER];
		int i;		

		
		is.getline(buffer,MAX_BUFFER,'\n');
		sscanf(buffer,"%d",&pbm.cutoff);

		is.getline(buffer,MAX_BUFFER,'\n');		
		ifstream f((string(getenv("HOME")) + buffer).c_str());
		
		int seqCount = -1;
		pbm.fragments = new vector<string>();
		pbm.description = new vector<string>();

		/* read fragments from input file and store its description in descr[]
		   and its sequence in sequence[] */

		char oneLine[2000];
		string sequence;
		string upperSeq;
		while(f.getline(oneLine, 2000))
		{
//cout << oneLine;
			string temp = string(oneLine);
			int rightArrow = temp.find_first_of(">");
			if(rightArrow != -1) //rightArrow is found
			{
				seqCount++;
				if(seqCount != 0)
					pbm.fragments->push_back(sequence);
				int space = temp.find(" ", rightArrow); 
				pbm.description->push_back(temp.substr(rightArrow+1, space));
				sequence = "";
			}
			else //line doesn't start with rightArrow
			{
				upperSeq = "";
				for(int i = 0; i < temp.length(); i++)
					upperSeq += toupper(temp.at(i));

				sequence.append(upperSeq);
			}
		}
		pbm.fragments->push_back(sequence);
		f.close();    
		
		pbm.numOfFragments = pbm.fragments->size();
		pbm.fragmentLength = new int[pbm.numOfFragments];

		pbm.rcFragments = new vector<string>();

		for(int j = 0; j < pbm.numOfFragments;  j++)
		{
			pbm.rcFragments->push_back(Util::rcSequence(pbm.fragments->at(j)));
//cout << pbm.description->at(j) << " " << pbm.fragments->at(j) << " " << pbm.rcFragments->at(j) << endl;
		}

		int sumLength = 0;
		for(int i = 0; i < pbm.numOfFragments;  i++)
		{
			pbm.fragmentLength[i] = pbm.fragments->at(i).length();
			sumLength += pbm.fragmentLength[i];
		}
		pbm.avgLength = (double)sumLength / (double)pbm.numOfFragments;		
		
		pbm.detector = new OverlapDetector(pbm.numOfFragments);
//		pbm.detector->buildScoresTable(pbm.fragments, pbm.rcFragments);
		pbm.detector->readScoreTable();
//		pbm.detector->printScoreTable();
		
		return is;
	}

	bool Problem::operator== (const Problem& pbm) const
	{
		// Falta terminar
		return true;
	}

	bool Problem::operator!= (const Problem& pbm) const
	{
		return !(*this == pbm);
	}

	Direction Problem::direction() const
	{
	 	return maximize; // F1
//		return minimize; // F2
	}

	Problem::~Problem()
	{
		delete detector;
		delete fragments;
		delete rcFragments;
		delete description;
		delete [] fragmentLength;		
	}

	// Solution --------------------------------------------------------------

	Solution::Solution (const Problem& pbm):_pbm(pbm),fragOrder(new int[pbm.numOfFragments])
	{}

	const Problem& Solution::pbm() const
	{
		return _pbm;
	}

	Solution::Solution(const Solution& sol):_pbm(sol.pbm())
	{
		if(fragOrder != NULL) delete [] fragOrder;
		
		fragOrder = new int [_pbm.numOfFragments];
		
		for(int i = 0; i < _pbm.numOfFragments; i++)
			fragOrder[i] = sol.fragOrder[i];
	}

	istream& operator>> (istream& is, Solution& sol)
	{
		for (int i=0;i<sol.pbm().numOfFragments;i++)
			is >> sol.fragOrder[i];
		return is;
	}

	ostream& operator<< (ostream& os, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().numOfFragments;i++)
			os << " " << sol.fragOrder[i];
		return os;
	}

	NetStream& operator << (NetStream& ns, const Solution& sol)
	{
		for (int i=0;i<sol.pbm().numOfFragments;i++)
			ns << sol.fragOrder[i];
		return ns;
	}

	NetStream& operator >> (NetStream& ns, Solution& sol)
	{
		for (int i=0;i<sol.pbm().numOfFragments;i++)
			ns >> sol.fragOrder[i];
		return ns;
	}

 	Solution& Solution::operator= (const Solution &sol)
	{
		if(fragOrder != NULL) delete [] fragOrder;
		
		fragOrder = new int [_pbm.numOfFragments];
		
		for(int i = 0; i < _pbm.numOfFragments; i++)
			fragOrder[i] = sol.fragOrder[i];
		
		return *this;
	}

	bool Solution::operator== (const Solution& sol) const
	{
		if (sol.pbm() != _pbm) return false;
		for(int i = 0; i < _pbm.numOfFragments; i++)
			if(fragOrder[i] != sol.fragOrder[i]) return false;
		return true;
	}

	bool Solution::operator!= (const Solution& sol) const
	{
		return !(*this == sol);
	}

	void Solution::initialize()
	{
		int * randNum = new int[_pbm.numOfFragments];
		
		for(int k = 0; k < _pbm.numOfFragments; k++)
		{
			int num = rand_int(0, _pbm.numOfFragments * 2);
			randNum[k] = num;
			fragOrder[k] = k;
		}

		// sort value and store index as fragment order
		for(int i = 0; i < _pbm.numOfFragments-1; i++)
		{
			for(int j = i+1; j < _pbm.numOfFragments; j++)
			{
				if( randNum[i] > randNum[j])
				{
					int temp = randNum[i];
					randNum[i] = randNum[j];
					randNum[j] = temp;

					temp = fragOrder[i];
					fragOrder[i] = fragOrder[j];
					fragOrder[j] = temp;
				}
			}
		}
		delete [] randNum;
	}

	double Solution::fitness ()
	{
		int ** score = _pbm.detector->getScoreTable();
		// F1 maximization
		int fit = 0;

		for(int k = 0; k < _pbm.numOfFragments-1; k++)
		{
			int i = fragOrder[k];
			int j = fragOrder[k+1];

			fit += abs(score[i][j]);
		}
		return (double) fit; 
		// F2 minimization
	/*	int fit = 0;
		int nof = _pbm.numOfFragments;

		for(int i = 0; i < nof; i++)
		{
			int m = fragOrder[i];
			for(int j = 0; j < nof; j++)
			{
				if(i != j)
				{
					int n = fragOrder[j];
					if((nof<m) || (nof<n) || (m<0) || (n<0))
					{
						cout << "Error en indices" << endl;
						return infinity();
					}
					fit += abs(i-j) * abs(score[m][n]);
				}
			}
		}

		return (double)fit;
*/
	}

	char *Solution::to_String() const
	{
		return (char *)fragOrder;
	}

	void Solution::to_Solution(char *_string_)
	{
		int *ptr=(int *)_string_;
		
		for (int i=0;i<_pbm.numOfFragments;i++)
		{
			fragOrder[i]=*ptr;
			ptr++;
		}
	}

	unsigned int Solution::size() const
	{
		return (_pbm.numOfFragments * sizeof(int));
	}


	int& Solution::fragment(const int index)
	{
		return fragOrder[index];
	}


	int* Solution::fragments()
	{
		return fragOrder;
	}

	Solution::~Solution()
	{
		delete [] fragOrder;		
	}

	// UserStatistics -------------------------------------------------------

	UserStatistics::UserStatistics ()
	{}

	ostream& operator<< (ostream& os, const UserStatistics& userstat)
	{
		os << "\n---------------------------------------------------------------" << endl;
		os << "                   STATISTICS OF TRIALS                   	 " << endl;
		os << "------------------------------------------------------------------" << endl;

		for (int i=0;i< userstat.result_trials.size();i++)
		{
			os << endl
			   << userstat.result_trials[i].trial
			   << "\t" << userstat.result_trials[i].best_cost_trial
			   << "\t\t" << userstat.result_trials[i].worst_cost_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_evaluation_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].nb_iteration_best_found_trial
			   << "\t\t\t" << userstat.result_trials[i].time_best_found_trial
			   << "\t\t" << userstat.result_trials[i].time_spent_trial;
		}
		os << endl << "------------------------------------------------------------------" << endl;
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
		new_stat->trial         		 		 = solver.current_trial();
		new_stat->nb_evaluation_best_found_trial = solver.evaluations_best_found_in_trial();
		new_stat->nb_iteration_best_found_trial  = solver.iteration_best_found_in_trial();
		new_stat->worst_cost_trial     		 	 = solver.worst_cost_trial();
		new_stat->best_cost_trial     		 	 = solver.best_cost_trial();
		new_stat->time_best_found_trial		 	 = solver.time_best_found_trial();
		new_stat->time_spent_trial 		 		 = solver.time_spent_trial();

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

// Intra_operator  --------------------------------------------------------------

	Intra_Operator::Intra_Operator(const unsigned int _number_op):_number_operator(_number_op),probability(NULL)
	{}

	unsigned int Intra_Operator::number_operator() const
	{
		return _number_operator;
	}

	Intra_Operator *Intra_Operator::create(const unsigned int _number_op)
	{
		switch (_number_op)
		{
			case 0: return new Crossover();break;
			case 1: return new Mutation();break;
		}
	}

	ostream& operator<< (ostream& os, const Intra_Operator& intra)
	{
		switch (intra.number_operator())
		{
			case 0: os << (Crossover&)intra;break;
			case 1: os << (Mutation&)intra;break;
		}
		return os;
	}

	Intra_Operator::~Intra_Operator()
	{}

//  Crossover:Intra_operator -------------------------------------------------------------

	Crossover::Crossover():Intra_Operator(0)
	{
		probability = new float[1];
	}

	void Crossover::cross(Solution& sol1,Solution& sol2) const // dadas dos soluciones de la poblacion, las cruza
	{
		// ER
		int indsize = sol1.pbm().numOfFragments;
/*
		AdjKeys * adjKeys = new AdjKeys(sol1.fragments(), sol2.fragments(),indsize);

		adjKeys->recombine();

		for(int i = 0; i < sol1.pbm().numOfFragments; i++)
		{
			sol1.fragment(i) = adjKeys->offSpring[i];
		}
		delete adjKeys;*/
		// ORDERED BASED
		 if (indsize > 1)
		 {
			// random generate two crossover points
			int crosspoint1 = rand_int(1,indsize-1);
			int crosspoint2 = rand_int(1,indsize-1);

			// if two points are the same, the second point is regenerated
			while(crosspoint2 == crosspoint1)
				crosspoint2 = rand_int(1,indsize-1);

			if(crosspoint1 > crosspoint2)
			{
				int t = crosspoint1;
				crosspoint1 = crosspoint2;
				crosspoint2 = t;
			}

			int * offSpring = new int[indsize];
			for(int i = crosspoint1; i <= crosspoint2; i++)
				offSpring[i] = sol1.fragment(i);

			int m = 0;
			for(int j = 0; j < indsize; j++)
			{
				bool exist = false;
				int temp = sol2.fragment(j);
				for(int k = crosspoint1; k <= crosspoint2; k++)
				{
					if(temp == offSpring[k])
					{
						exist = true;
						break;
					}
				}
				if(!exist)
				{
					if(m == crosspoint1)
						m = crosspoint2 + 1;

					offSpring[m++] = temp;
				}
			}

			for(int k = 0; k < indsize; k++)
			{
				sol2.fragment(k) = offSpring[k];
			}
			delete [] offSpring;
		  }
	}

	void Crossover::execute(Rarray<Solution*>& sols) const
	{
		int popsize = sols.size();
		
		for(int i = 0; i < popsize; i++)	  
		{
			if (rand01()<=probability[0]) 
			{
				  // random generate two parents
				  int parent1 = rand_int(0, popsize-1);
				  int parent2 = rand_int(0, popsize-1);
	
				  // if both parents are the same, the second parent is regenerated
				  while(parent2 == parent1)
					parent2 = rand_int(0, popsize-1);
		
				  cross(*sols[parent1], *sols[parent2]);
			  }
		  }
	}

	ostream& operator<< (ostream& os, const Crossover&  cross)
	{
		 os << "Crossover." << " Probability: "
                    << cross.probability[0]
		    << endl;
		 return os;
	}

	void Crossover::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_crossover_probability",(char *)probability,1,sizeof(float));
	}

	void Crossover::UpdateFromState(const StateCenter& _sc)
	{
		 unsigned long nbytes,length;
		 _sc.get_contents_state_variable("_crossover_probability",(char *)probability,nbytes,length);
	}

	void Crossover::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
	}

	Crossover::~Crossover()
	{
		delete [] probability;
	}

	//  Mutation: Sub_operator -------------------------------------------------------------

	Mutation::Mutation():Intra_Operator(1)
	{
		probability = new float[1];
	}

	void Mutation::mutate(Solution& sol) const
	{
		int indsize = sol.pbm().numOfFragments;
		int r1 = rand_int(0, indsize-1);
		int r2 = rand_int(0, indsize-1);

		while(r2 == r1)
		{
			if(r1 == indsize-1) r2 = rand_int(0, indsize-2);
			else r2 = rand_int(r1, indsize-1);
		}

		int temp = sol.fragment(r1);
		sol.fragment(r1) = sol.fragment(r2);
		sol.fragment(r2) = temp;
	}

	void Mutation::execute(Rarray<Solution*>& sols) const
	{
		for (int i=0;i<sols.size();i++)
		{
			if(rand01() <= probability[0])	mutate(*sols[i]);
		}
	}

	ostream& operator<< (ostream& os, const Mutation&  mutation)
	{
		os << "Mutation." << " Probability: " << mutation.probability[0]
//		   << " Probability1: " << mutation.probability[1]
		   << endl;
		return os;
	}

	void Mutation::setup(char line[MAX_BUFFER])
	{
		int op;
		sscanf(line," %d %f ",&op,&probability[0]);
		assert(probability[0]>=0);
//		assert(probability[1]>=0);
	}

	void Mutation::RefreshState(const StateCenter& _sc) const
	{
		_sc.set_contents_state_variable("_mutation_probability",(char *)probability,1,sizeof(probability));
	}

	void Mutation::UpdateFromState(const StateCenter& _sc)
	{
		unsigned long nbytes,length;
		_sc.get_contents_state_variable("_mutation_probability",(char *)probability,nbytes,length);
	}

	Mutation::~Mutation()
	{
		delete [] probability;
	}

// StopCondition_1 -------------------------------------------------------------------------------------

	StopCondition_1::StopCondition_1():StopCondition()
	{}

	bool StopCondition_1::EvaluateCondition(const Problem& pbm,const Solver& solver,const SetUpParams& setup)
	{
//		return ((int)solver.best_cost_trial() == 0);
		return false;
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

