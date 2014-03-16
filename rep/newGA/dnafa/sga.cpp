/***********************************************************************
 *  File:        sga.cpp
 *  Author:      Lishan Li                                        
 *  Date:        May, 2003                                                 
 *
 *  Description: It provides implementation of all the functions declared   
 *               in the header file sga.h.
 ***********************************************************************/

#include "sga.h"

/*********************************************
 * Purpose: allocate memory and initialization
 *********************************************/

SGA::SGA(const string &is, const string &it)
{
	g = -2;       // gap penalty = -2
	s = is;
	t = it;
	m = s.length();
	n = t.length();	
	bestScore = 0;

	score = new int *[m+1];
	for(int i = 0; i <= m; i++)
		score[i] = new int[n+1];	
}

/*************************
 * Purpose: release memory
 *************************/

SGA::~SGA()
{
	for(int i = 0; i <= m; i++)
			delete [] score[i];	
	
	delete [] score;
}


/*******************************************************
 * Purpose: Build a score matrix to store the scores of
 *          pairwise semiglobal alignment
 *******************************************************/

void SGA::buildMatrix()
{
    // does not penalize the starting & ending gaps

	for (int i = 0; i <= m; i++)
	{
		score[i][0] = 0;         
	}
	for (int j = 0; j <= n; j++)
	{
		score[0][j] = 0;
	}

	// calculate score from row 1 to m, column 1 to n

	for (int i = 1; i <= m; i++)
	{
		for (int j = 1; j <= n; j++)
		{

			if(p(i,j) == 1)
				score[i][j] = score[i-1][j-1] + 1;
			else
				score[i][j] = 0;
		}
	}
}

/***************************
 * match score = 1
 * mismatch score = -1
 ***************************/

int SGA::p(int i, int j)
{
	if (s[i-1] == t[j-1])  // match
		return 1;
	else
		return -1;
}

/****************************************************
 * Purpose: find the maximum number of x, y, and z
 ****************************************************/

int SGA::max(int x, int y, int z)
{
	int temp = x;
	if (x < y)
		temp = y;
	if (temp < z)
		temp = z;
	return temp;
}

/********************************************************************
 * Purpose: find the best score for the pairwise semiglobal alignment
 ********************************************************************/

void SGA::findBestScore()
{	
   for(int i = 1; i <= m; i++)
   {
	   if(score[i][n] == n && score[i][n] > bestScore)		//  -------------
	   {													//		-----
		  bestScore = score[i][n];
		  overlapType = 1;
	   }
	   else if(score[i][n] == i && score[i][n] > bestScore) 
	   {
		   bestScore = score[i][n];							//       ----------
		   overlapType = 2;									//  -----------
	   } 
   }														

   for(int j = 1; j <= n; j++)
   {
	   if(score[m][j] == m && score[m][j] > bestScore)		//  	------
	   {													//	-------------
		   bestScore = score[m][j];
		   overlapType = 3;
	   }
	   else if(score[m][j] == j && score[m][j] > bestScore) 
	   {
			bestScore = score[m][j];	 			    	//  ----------
		   	overlapType = 4;								//       ----------
	   }
   }
}


/**********************************************************************
 * Purpose: Align the two sequence starting from the the row and column
 *          with the best score
 **********************************************************************/

void SGA::align()
{
	int i;
	int start;
	string preGaps = "";
	string postGaps = "";
	if((start = s.find(t)) != string::npos)	// t is substring of s
	{
		align_s = s;
		for(i = 0; i < start; i++)
			preGaps += "-";
		align_t = preGaps + t;

		for(i = start+bestScore; i < m; i++)
			postGaps += "-";
		align_t += postGaps;
	}
	else if((start = t.find(s)) != string::npos) // s is substring of t
	{
		align_t = t;
		for(i = 0; i < start; i++)
			preGaps += "-";
		align_s = preGaps + s;

		for(i = start+bestScore; i < n; i++)
			postGaps += "-";
		align_s += postGaps;
	}
	else if(overlapType == 4)
	{
		int preNumGaps = m - bestScore;
		for(int i = 0; i < preNumGaps; i++)
			preGaps += "-";
		align_t = preGaps + t;

		int postNumGaps = align_t.length() - m;
		for(int j = 0; j < postNumGaps; j++)
			postGaps += "-";
		align_s = s + postGaps;
	}
	else
	{
		int preNumGaps = n - bestScore;
		for(int i = 0; i < preNumGaps; i++)
			preGaps += "-";
		align_s = preGaps + s;

		int postNumGaps = align_s.length() - n;
		for(int j = 0; j < postNumGaps; j++)
			postGaps += "-";
		align_t = t + postGaps;
	}
}

/************************************************************
 * Purpose: Print the score matrix for the pairwise alignment
 ************************************************************/

void SGA::printMatrix()
{
	for (int i = 0; i <= m; i++)
	{
		cout << endl;
		for (int j = 0; j <= n; j++)
		{
			printf("%3d", score[i][j]);
		}
	}
	cout << endl << endl;
	cout << "The best score is: " << bestScore << endl << endl;
}

/***************************************
 * Purpose: Print the pairwise alignment
 ***************************************/

void SGA::printAlign()
{
	cout << align_s << endl;
	cout << align_t << endl << endl;
}

/*******************************
 * Purpose: Print Info for debug
 *******************************/
void SGA::printInfo()
{
	cout << "m = " << m << " n = " << n << " g = " << g
		 << " bestRow = " << bestRow << " bestCol = " << bestCol << " bestscore = " << bestScore
		 << endl;
	cout << "s = " << align_s << " t = " << align_t << endl;

//	printMatrix();
}

/************************************ End of File **************************************/

