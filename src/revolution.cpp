#include "revolution.h"
#include "version.h"
#include "objective_function.h"

using revolution::Version;
using revolution::ObjectiveFunction;

/*---------------------------------------------------------------------------*/
struct RVObjectiveFunction
{
		RVObjectiveFunction(ObjectiveFunction *objfun) : impl(objfun) {}
		~RVObjectiveFunction() { delete static_cast<ObjectiveFunction *>(impl);}
		void *impl;
};//~ RVObjectiveFunction

/*---------------------------------------------------------------------------*/
extern "C" 
int RVGetMajorVersion(void)
{
	return Version::getMajor();
}

/*---------------------------------------------------------------------------*/
extern "C"
int RVGetMinorVersion(void)
{
	return Version::getMinor();
}

/*---------------------------------------------------------------------------*/
extern "C"
int RVGetPatchVersion(void)
{
	return Version::getPatch();
}

/*---------------------------------------------------------------------------*/
extern "C"
RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objs, RVObjectiveEvalFun fun)
{
	ObjectiveFunction *objfun = ObjectiveFunction::create(dim, objs, fun);
	RVObjectiveFunction *objfunWrapper = 0;
	if (objfun)
	{
		objfunWrapper = new RVObjectiveFunction(objfun);
	} 
	return objfunWrapper;
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVObjectiveFunctionDestroy(RVObjectiveFunction* object)
{
	if (object)
	{
		delete object;
	}
}
