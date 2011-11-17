#include "basic_es.h"
#include "objective_function.h"
#include "population.h"
#include "atom.h"
#include <vector>

using revolution::BasicEs;
using revolution::ObjectiveFunction;

/*--------------------------------------------------------------------------*/
namespace 
{

class Atom : private revolution::Atom
{
public:
	explicit Atom(int n)
		: revolution::Atom(n), strParams(n, 0)
	{

	}

	double strategyParam(int index) const 
	{
		return strParams.at(index);
	}

	void setStrategyParam(int index, double value)
	{
		strParams[index] = value;
	}

private:
	std::vector<double> strParams; // endogenous strategy params
};//~Atom for BasicEs


/*--------------------------------------------------------------------------*/
class Population : private revolution::Population<Atom>
{
public:
	Population(int mu, int lambda, int dim)
			: revolution::Population<Atom>(mu, lambda, dim)
	{

	}
};//~ BasicEs::Population

}//~ unamed

/*--------------------------------------------------------------------------*/
class BasicEs::BasicEsPriv
{
public:
	BasicEsPriv(int m, int r, int l, RVSelectionMode mode, ObjectiveFunction *objf)
		: atoms(new ::Population(m, l, objf->dim())), selectionMode(mode), objectiveFunction(objf)
	{

	}

 	~BasicEsPriv()
	{
		delete atoms;
		atoms = 0;
	}

	void setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
	{
			initialValuesFun = fun;
			initialValuesFunData = data;
	}

	::Population *atoms;	
	RVSelectionMode selectionMode;
	ObjectiveFunction *objectiveFunction;
	RVPopulationSetInitialValues initialValuesFun;
	void *initialValuesFunData;
	
};//~BasicEs::BasicEsPriv


/*---------------------------------------------------------------------------*/
BasicEs::BasicEs(int m, int r, int la, RVSelectionMode sel, ObjectiveFunction *objf)
	: impl(new BasicEs::BasicEsPriv(m, r, la, sel, objf))
{

}

/*---------------------------------------------------------------------------*/
BasicEs::~BasicEs()
{
	delete impl;
	impl = 0;
}

/*--------------------------------------------------------------------------*/
void BasicEs::setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
{
	impl->setPopulationInitialValues(fun, data);
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
