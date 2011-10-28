#include "objective_function.h"

using revolution::ObjectiveFunction;


/*---------------------------------------------------------------------------*/
ObjectiveFunction::ObjectiveFunction(int dim, int obj, RVObjectiveEvalFun fun)
		: dimensionality(dim), objectivesNumber(obj), evalf(fun)
{

}

/*---------------------------------------------------------------------------*/
const int ObjectiveFunction::dim() const
{
	return dimensionality;
}

/*---------------------------------------------------------------------------*/
const int ObjectiveFunction::objectives() const
{
	return objectivesNumber;
}

/*---------------------------------------------------------------------------*/
void ObjectiveFunction::eval(const double *dv, double *obj) const
{
	evalf(dv, obj);
}

/*---------------------------------------------------------------------------*/
ObjectiveFunction* ObjectiveFunction::create(int dim, int objs, RVObjectiveEvalFun fun)
{
	ObjectiveFunction* objfun = 0;

	if (dim > 0 && objs > 0 && fun)
	{
		objfun = new ObjectiveFunction(dim, objs, fun);
	}

	return objfun;
}

