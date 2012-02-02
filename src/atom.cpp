#include "atom.h"
#include "objective_function.h"
#include "array.h"
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
    struct RVArray dv = { &params[0], params.size() };
    struct RVArray ov = { &objectives[0], params.size() };
	fun.eval(&dv, &ov);
}

/*---------------------------------------------------------------------------*/
void Atom::eval(struct RVObjectiveFunction *obj)
{
    struct RVArray dv = { &params[0], params.size() };
    struct RVArray ov = { &objectives[0], params.size() };
	void *data = RVObjectiveFunctionGetUserData(obj);
	RVObjectiveEvaluationFun fun = RVObjectiveFunctionGetEvalFun(obj);
	fun(&dv, &ov, data);
}

/*---------------------------------------------------------------------------*/
double Atom::f(int index) const
{
	return objectives.at(index);
}

/*---------------------------------------------------------------------------*/
int Atom::dim() const
{
	return static_cast<int>(params.size());
}

/*---------------------------------------------------------------------------*/
int Atom::obj() const
{
	return static_cast<int>(objectives.size());
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
        struct RVArray dv = { &params[0], params.size() };
        struct RVArray ov = { &objectives[0], params.size() };
		fun(&dv, &ov, data);
	}
}

/*---------------------------------------------------------------------------*/
void Atom::constrain(RVConstrainParam fun, void *data)
{
    if (fun)
    {
        struct RVArray dv = { &params[0], params.size() };
        fun(&dv, data);
    }
}
