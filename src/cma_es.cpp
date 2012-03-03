#include "cma_es.h"
#include "objective_function.h"

using revolution::CmaEs;
using revolution::ObjectiveFunction;

/*---------------------------------------------------------------------------*/
class CmaEs::CmaEsPriv
{
public:
    CmaEsPriv(unsigned int l, struct RVObjectiveFunction *fun)
		: lambda(l), objFun(fun), wrapper(0) 
	{
		
	}
	
	void setWrapperObject(RVCmaEvolutionStrategy *w)
	{
		wrapper = w;
	}
	
	unsigned int lambda;
    struct RVObjectiveFunction *objFun;
    struct RVCmaEvolutionStrategy *wrapper;
};

/*---------------------------------------------------------------------------*/
CmaEs::CmaEs(unsigned int lambda, struct RVObjectiveFunction *fun)
	: impl(new CmaEsPriv(lambda, fun))
{
	
}

/*---------------------------------------------------------------------------*/
CmaEs::~CmaEs()
{
	delete impl;
}

/*---------------------------------------------------------------------------*/
void CmaEs::setWrapperObject(RVCmaEvolutionStrategy *w)
{
	impl->setWrapperObject(w);	
}

/*---------------------------------------------------------------------------*/
CmaEs* CmaEs::create(unsigned int lambda, RVObjectiveFunction *fun)
{
	return new CmaEs(lambda, fun);
}
