#ifndef REVOLUTION_CMA_ES_H_INCLUDED
#define REVOLUTION_CMA_ES_H_INCLUDED

#include "revolution.h"

namespace revolution
{

class DLL_HIDDEN CmaEs
{
public:
    static CmaEs* create(unsigned int lambda, struct RVObjectiveFunction *fun);
	~CmaEs();
	void setWrapperObject(RVCmaEvolutionStrategy *w);
    void setTermination(RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data);
private:
    CmaEs(unsigned int lambda, struct RVObjectiveFunction *fun);
	
	class CmaEsPriv;
	CmaEsPriv *impl;
private:

};//~ CmaEs

}//~ revolution

#endif /* REVOLUTION_CMA_ES_H_INCLUDED */
