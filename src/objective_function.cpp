#include "objective_function.h"

using revolution::ObjectiveFunction;


/*---------------------------------------------------------------------------*/
ObjectiveFunction::ObjectiveFunction(unsigned int dim, unsigned int obj, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data)
		: dimensionality(dim), objectivesNumber(obj), evalf(fun), userData(data)
{

}

/*---------------------------------------------------------------------------*/
const unsigned int ObjectiveFunction::dim() const
{
	return dimensionality;
}

/*---------------------------------------------------------------------------*/
const unsigned int ObjectiveFunction::objectives() const
{
	return objectivesNumber;
}

/*---------------------------------------------------------------------------*/
void ObjectiveFunction::eval(struct RVArray *const dv, struct RVArray *obj) const
{
	evalf(dv, obj, userData);
}

/*---------------------------------------------------------------------------*/
ObjectiveFunction* ObjectiveFunction::create(unsigned int dim, unsigned int objs, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data)
{
	ObjectiveFunction* objfun = 0;

	if (dim > 0 && objs > 0 && fun)
	{
		objfun = new ObjectiveFunction(dim, objs, fun, data);
	}

	return objfun;
}

