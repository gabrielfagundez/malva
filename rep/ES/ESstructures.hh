#ifndef INC_ES_mallba_hh
#define INC_ES_mallba_hh


#include <iostream>
#include <fstream>
#include <cmath>
#include <values.h>
#include <Mallba/Rlist.h>
#include <Mallba/Rarray.h>
#include <Mallba/Messages.h>
#include <Mallba/Matrix.hh>
#include <Mallba/mallba.hh>
#include <Mallba/States.hh>
#include <Mallba/random.hh>
#include <Mallba/time.hh>
#include <Mallba/netstream.hh>
#include <assert.h>

using namespace std;

struct individual // index of a individual in the population and its fitness
{
	int index;
	double fitness;
	double sel_parameter;
	bool change;
};


#endif
