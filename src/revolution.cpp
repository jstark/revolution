#include "revolution.h"
#include "version.h"
#include "objective_function.h"
#include "basic_es.h"
#include <cstdio>
#include <cstdlib>
#include <limits>

using revolution::Version;
using revolution::ObjectiveFunction;
using revolution::BasicEs;

/*---------------------------------------------------------------------------*/

#define DEFINE_POD_WRAPPER_STRUCT(ClassName, WrappedClass) \
	struct ClassName { \
		WrappedClass *impl; \
	};

#define SET_WRAPPED_OBJECT(WrapperObject, WrappedObject) (WrapperObject)->impl = WrappedObject
#define GET_WRAPPED_OBJECT(WrapperObject) ((WrapperObject)->impl)

DEFINE_POD_WRAPPER_STRUCT(RVObjectiveFunction, ObjectiveFunction)
DEFINE_POD_WRAPPER_STRUCT(RVBasicEvolutionStrategy, BasicEs)

#define CONSTRUCT_POD_OBJECT(ClassName) \
	static_cast<ClassName *>(std::calloc(1, sizeof(ClassName)))

#define FREE_POD_OBJECT(Object) \
	free(Object)


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
struct RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objs, RVObjectiveEvalFun fun)
{
	ObjectiveFunction *objfun = ObjectiveFunction::create(dim, objs, fun);
	RVObjectiveFunction *wrapper = 0;
	if (objfun)
	{
		wrapper = CONSTRUCT_POD_OBJECT(RVObjectiveFunction);
		SET_WRAPPED_OBJECT(wrapper, objfun);
	} 
	return wrapper;
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVObjectiveFunctionDestroy(struct RVObjectiveFunction* object)
{
	if (object)
	{
		ObjectiveFunction *of = GET_WRAPPED_OBJECT(object);
		delete of;
		FREE_POD_OBJECT(object);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, RVSelectionMode mode, RVObjectiveFunction *fun)
{
	BasicEs *es = BasicEs::create(mu, rho, lambda, mode, GET_WRAPPED_OBJECT(fun));
	RVBasicEvolutionStrategy *wrapper = 0;
	if (es)
	{
		wrapper = CONSTRUCT_POD_OBJECT(RVBasicEvolutionStrategy);
		SET_WRAPPED_OBJECT(wrapper, es);
		es->setWrapperObject(wrapper);
	}
	return wrapper;
}

/*---------------------------------------------------------------------------*/
void RVBasicEvolutionStrategyInitializePopulation(RVBasicEvolutionStrategy *es, RVPopulationSetInitialValues fun, void *data)
{
	if (es)
	{
		BasicEs *basic = GET_WRAPPED_OBJECT(es);
		basic->setPopulationInitialValues(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyStart(RVBasicEvolutionStrategy *es)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		b->start();
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyOnGenerationFinished(RVBasicEvolutionStrategy *es, RVGenerationFinished fun, void *data)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		b->setOnGenerationFinished(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategySetTerminationCriteria(RVBasicEvolutionStrategy *es, RVEvolutionShouldTerminate fun, void *data)
{
    if (es)
    {
        BasicEs *b = GET_WRAPPED_OBJECT(es);
        b->setTerminationCriteria(fun, data);
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
double RVBasicEvolutionStrategyGetDesignParameter(RVBasicEvolutionStrategy *es, int parent, int paramIndex)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		return b->getDesignParameter(parent, paramIndex);
	}
	return std::numeric_limits<double>::max();
}

/*---------------------------------------------------------------------------*/
extern "C"
double RVBasicEvolutionStrategyGetObjective(RVBasicEvolutionStrategy *es, int parent, int objIndex)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		return b->getObjective(parent, objIndex);
	}
	return std::numeric_limits<double>::max();
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyDestroy(RVBasicEvolutionStrategy *es)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		delete b;
		FREE_POD_OBJECT(es);
	}
}

