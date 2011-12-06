#include "cma_es.h"
#include "objective_function.h"

using revolution::CmaEs;

class CmaEs::CmaEsPriv
{
public:
	CmaEsPriv(unsigned int l, ObjectiveFunction *fun)
		: lambda(l), objFun(fun), wrapper(0) 
	{
		
	}
	
	void setWrapperObject(RVCmaEvolutionStrategy *w)
	{
		wrapper = w;
	}
	
	unsigned int lambda;
	ObjectiveFunction *objFun;
	RVCmaEvolutionStrategy *wrapper;
};

CmaEs::CmaEs(unsigned int lambda, ObjectiveFunction *fun)
	: impl(new CmaEsPriv(lambda, fun))
{
	
}

CmaEs::~CmaEs()
{
	delete impl;
}

void CmaEs::setWrapperObject(RVCmaEvolutionStrategy *w)
{
	impl->setWrapperObject(w);	
}

CmaEs* CmaEs::create(unsigned int lambda, ObjectiveFunction *fun)
{
	return new CmaEs(lambda, fun);
}
