#include "atom.h"
#include "objective_function.h"
#include <limits>

using revolution::Atom;

namespace {
	const double MAX_VAL = std::numeric_limits<double>::max();
}

/*---------------------------------------------------------------------------*/
Atom::Atom(int dim, int obj)
	: params(dim, 0.0), objectives(obj, MAX_VAL) 
{

}

/*---------------------------------------------------------------------------*/
double Atom::operator[](int index) const
{
	return params.at(index);
}

/*---------------------------------------------------------------------------*/
double& Atom::operator[](int index)
{
	return params.at(index);
}

/*---------------------------------------------------------------------------*/
void Atom::eval(const revolution::ObjectiveFunction& fun)
{
	fun.eval(&params[0], &objectives[0]);
}

/*---------------------------------------------------------------------------*/
double Atom::f(int index) const
{
	return objectives.at(index);
}

/*---------------------------------------------------------------------------*/
int Atom::dim() const
{
	return params.size();
}

/*---------------------------------------------------------------------------*/
int Atom::obj() const
{
	return objectives.size();
}

/*---------------------------------------------------------------------------*/
void Atom::swap(Atom& rhs)
{
	params.swap(rhs.params);
	objectives.swap(rhs.objectives);
}

/*---------------------------------------------------------------------------*/
void Atom::initialize(RVPopulationSetInitialValues fun, void *data)
{
	if (fun)
	{
		fun(&params[0], &objectives[0], data);
	}
}

/*---------------------------------------------------------------------------*/
void Atom::constrain(RVConstrainParam fun, void *data)
{
    if (fun)
    {
        fun(&params[0], data);
    }
}
