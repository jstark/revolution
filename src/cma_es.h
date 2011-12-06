#ifndef REVOLUTION_CMA_ES_H_INCLUDED
#define REVOLUTION_CMA_ES_H_INCLUDED

#include "revolution.h"

namespace revolution
{

class ObjectiveFunction; 

class DLL_HIDDEN CmaEs
{
public:
	static CmaEs* create(unsigned int lambda, ObjectiveFunction *fun);
	~CmaEs();
	void setWrapperObject(RVCmaEvolutionStrategy *w);
private:
	CmaEs(unsigned int lambda, ObjectiveFunction *fun);
	
	class CmaEsPriv;
	CmaEsPriv *impl;
private:

};//~ CmaEs

}//~ revolution

#endif /* REVOLUTION_CMA_ES_H_INCLUDED */
