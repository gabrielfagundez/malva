/************************************************ 
***				  	      *** 
***  Cooperative Local Search Skeleton  v1.0  *** 
***  User-required classes and methods        ***
***  Developed by: Carlos Cotta Porras        *** 
***                                           *** 
************************************************/ 


#include <iostream.h>
#include "CLS.hh"
#include "Mallba/random.hh"


skeleton CLS {




// UserStatistics -------------------------------------------------------


	UserStatistics::UserStatistics ()
	{
	}


	ostream& operator<< (ostream& os, const UserStatistics& usertat)
	{
		 return os;
	}


	/* friend NetStream& operator << (NetStream& ns, const UserStatistics& usertats)
		{
		 return ns;
		}

	 friend NetStream& operator >> (NetStream& ns, UserStatistics& usertats)
		{
		 return ns;
		}
	*/

	UserStatistics& UserStatistics::operator= (const UserStatistics& userstats)
	{
		 return (*this);
	}


	void UserStatistics::update(const Solver_Seq& solver)
	{
	}


	UserStatistics::~UserStatistics()
	{
	}

	void UserStatistics::clear()
	{
	}



	bool TerminateQ (const Problem& pbm, const Solver_Seq& solver,
			 const SetUpParams& setup)
	{
		 return false;
	}



}

