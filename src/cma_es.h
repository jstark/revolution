#ifndef REVOLUTION_CMA_ES_H_INCLUDED
#define REVOLUTION_CMA_ES_H_INCLUDED

namespace revolution
{

class ObjectiveFunction; 

class CmaEs
{
public:
	static CmaEs* create(ObjectiveFunction *fun);
private:

};//~ CmaEs

}//~ revolution

#endif /* REVOLUTION_CMA_ES_H_INCLUDED */
