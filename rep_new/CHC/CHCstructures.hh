#ifndef INC_CHC_mallba_hh
#define INC_CHC_mallba_hh

#include <iostream>
#include <fstream>
#include <math.h>
#include <values.h>
#include <Rlist.h>
#include <Rarray.h>
#include <Messages.h>
#include <mallba.hh>
#include <States.hh>
#include <random.hh>
#include <time.hh>
#include <netstream.hh>
#include <assert.h>

using namespace std;

struct individual // index of a individual in the population and its fitness
{
	int    index;
	double fitness;
	double sel_parameter;
	bool   change;
};	

#endif
