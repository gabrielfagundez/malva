#ifndef INC_newGA_mallba_hh
#define INC_newGA_mallba_hh


#include <iostream.h>
#include <fstream.h>
#include <math.h>
#include <values.h>
#include <Mallba/Rlist.h>
#include <Mallba/Rarray.h>
#include <Mallba/Messages.h>
#include <Mallba/mallba.hh>
#include <Mallba/States.hh>
#include <Mallba/random.hh>
#include <Mallba/time.hh>
#include <Mallba/netstream.hh>
#include <assert.h>

struct individual // index of a individual in the population and its fitness
{
	int index;
	double fitness;
	double sel_parameter;
	bool   change;

};

#endif
