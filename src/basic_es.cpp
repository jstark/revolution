#include "basic_es.h"
#include "objective_function.h"
#include "population.h"

using revolution::BasicEs;
using revolution::ObjectiveFunction;

/*--------------------------------------------------------------------------*/
class BasicEs::Population : private revolution::Population
{
public:
	Population(int mu, int lambda, int dim);
};//~ BasicEs::Population

/*--------------------------------------------------------------------------*/
BasicEs::Population::Population(int mu, int lambda, int dim)
	: revolution::Population(mu, lambda, dim)
{

}

/*---------------------------------------------------------------------------*/
BasicEs::BasicEs(int m, int r, int la, RVSelectionMode sel, ObjectiveFunction *objf)
	: mu(m), rho(r), lambda(la), mode(sel), objfun(objf), initialValuesFun(0)
{
	population.reset(new Population(m, la, objf->dim()));
}

/*---------------------------------------------------------------------------*/
void BasicEs::setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
{
	initialValuesFun = fun;
	initialValuesFunData = data;
}

/*--------------------------------------------------------------------------*/
BasicEs* BasicEs::create(int mu, int rho, int lambda,RVSelectionMode mode, ObjectiveFunction *objf)
{
	bool invalidPopulation = mu < 0 || lambda < 0;
	bool invalidRecombinationConstant = rho < 0 || rho > mu;
	bool invalidSelectionMode = mode == kRVSelectionModeComma ? lambda >= mu : false;

	if (invalidPopulation || invalidRecombinationConstant || invalidSelectionMode)
	{
		return 0;
	}
	return new BasicEs(mu, rho, lambda, mode, objf);
}