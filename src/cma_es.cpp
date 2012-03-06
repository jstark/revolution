#include "cma_es.h"
#include "objective_function.h"

using revolution::CmaEs;
using revolution::ObjectiveFunction;

/*---------------------------------------------------------------------------*/
class CmaEs::CmaEsPriv
{
public:
    CmaEsPriv(unsigned int l, struct RVObjectiveFunction *fun)
        : lambda_(l), objectiveFunction_(fun), wrapper_(0)
	{
		
	}
	
	void setWrapperObject(RVCmaEvolutionStrategy *w)
	{
        wrapper_ = w;
	}

    void setTermination(RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data)
    {
        evolutionShouldTerminate_ = fun;
        evolutionShouldTerminateData_ = data;
    }
	
    unsigned int lambda_;
    struct RVObjectiveFunction *objectiveFunction_;
    struct RVCmaEvolutionStrategy *wrapper_;
    RV_CMA_SHOULD_TERMINATE_FUNCTION evolutionShouldTerminate_;
    void *evolutionShouldTerminateData_;
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
void CmaEs::setTermination(RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data)
{
    impl->setTermination(fun, data);
}

/*---------------------------------------------------------------------------*/
CmaEs* CmaEs::create(unsigned int lambda, RVObjectiveFunction *fun)
{
	return new CmaEs(lambda, fun);
}
