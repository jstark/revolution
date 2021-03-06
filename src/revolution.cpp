#include "array.h"
#include "revolution.h"
#include "version.h"
#include "objective_function.h"
#include "basic_es.h"
#include "cma_es.h"
#include "de.h"
#include <cstdio>
#include <cstdlib>
#include <limits>

using revolution::Version;
using revolution::ObjectiveFunction;
using revolution::BasicEs;
using revolution::CmaEs;
using revolution::DifferentialEvolution;

/*---------------------------------------------------------------------------*/

#define DEFINE_POD_WRAPPER_STRUCT(ClassName, WrappedClass) \
	struct ClassName { \
		WrappedClass *impl; \
	};

#define SET_WRAPPED_OBJECT(WrapperObject, WrappedObject) (WrapperObject)->impl = WrappedObject
#define GET_WRAPPED_OBJECT(WrapperObject) ((WrapperObject)->impl)

DEFINE_POD_WRAPPER_STRUCT(RVObjectiveFunction, ObjectiveFunction)
DEFINE_POD_WRAPPER_STRUCT(RVBasicEvolutionStrategy, BasicEs)
DEFINE_POD_WRAPPER_STRUCT(RVCmaEvolutionStrategy, CmaEs)
DEFINE_POD_WRAPPER_STRUCT(RVDifferentialEvolution, DifferentialEvolution)

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
size_t RVArrayGetSize(struct RVArray const *array)
{
    if (array)
    {
        return array->size;
    }
    return -1;
}

/*---------------------------------------------------------------------------*/
double RVArrayGetElementAtIndex(struct RVArray const *array, size_t index)
{
    if (array)
    {
        return array->data[index];
    }
    return std::numeric_limits<double>::quiet_NaN();
}

/*---------------------------------------------------------------------------*/
void RVArraySetElementAtIndex(struct RVArray *array, size_t index, double value)
{
    if (array)
    {
        array->data[index] = value;
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
struct RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objs, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data)
{
	ObjectiveFunction *objfun = ObjectiveFunction::create(dim, objs, fun, data);
	struct RVObjectiveFunction *wrapper = 0;
	if (objfun)
	{
		wrapper = CONSTRUCT_POD_OBJECT(RVObjectiveFunction);
		SET_WRAPPED_OBJECT(wrapper, objfun);
	} 
	return wrapper;
}

/*---------------------------------------------------------------------------*/
extern "C"
int RVObjectiveFunctionGetDimensionality(struct RVObjectiveFunction *f)
{
    if (f)
    {
        ObjectiveFunction *obj = GET_WRAPPED_OBJECT(f);
        return obj->dim();
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
extern "C"
int RVObjectiveFunctionGetNumberOfObjectives(struct RVObjectiveFunction *f)
{
    if (f)
    {
        ObjectiveFunction *obj = GET_WRAPPED_OBJECT(f);
        return obj->objectives();
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
extern "C"
RV_OBJECTIVE_EVALUATION_FUNCTION RVObjectiveFunctionGetEvalFun(struct RVObjectiveFunction *f)
{
    if (f)
    {
        ObjectiveFunction *obj = GET_WRAPPED_OBJECT(f);
        return obj->evalFun();
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
extern "C"
void * RVObjectiveFunctionGetUserData(struct RVObjectiveFunction *f)
{
    if (f)
    {
        ObjectiveFunction *obj = GET_WRAPPED_OBJECT(f);
        return obj->data();
    }
    return 0;
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
struct RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, enum RVSelectionMode mode, struct RVObjectiveFunction *fun)
{
	BasicEs *es = BasicEs::create(mu, rho, lambda, mode, GET_WRAPPED_OBJECT(fun));
	struct RVBasicEvolutionStrategy *wrapper = 0;
	if (es)
	{
		wrapper = CONSTRUCT_POD_OBJECT(RVBasicEvolutionStrategy);
		SET_WRAPPED_OBJECT(wrapper, es);
		es->setWrapperObject(wrapper);
	}
	return wrapper;
}

/*---------------------------------------------------------------------------*/
void RVBasicEvolutionStrategyInitializePopulation(struct RVBasicEvolutionStrategy *es, RV_SET_INITIAL_VALUES_FUNCTION fun, void *data)
{
	if (es)
	{
		BasicEs *basic = GET_WRAPPED_OBJECT(es);
		basic->setPopulationInitialValues(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategySetParameterConstraints(struct RVBasicEvolutionStrategy *es, RV_CONSTRAIN_PARAMS_FUNCTION fun, void *data)
{
    if (es)
    {
        BasicEs *basic = GET_WRAPPED_OBJECT(es);
        basic->setParamConstraints(fun, data);
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyStart(struct RVBasicEvolutionStrategy *es)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		b->start();
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyOnGenerationFinished(struct RVBasicEvolutionStrategy *es, RV_BASIC_GENERATION_FINISHED_FUNCTION fun, void *data)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		b->setOnGenerationFinished(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategySetTerminationCriteria(struct RVBasicEvolutionStrategy *es, RV_BASIC_SHOULD_TERMINATE_FUNCTION fun, void *data)
{
    if (es)
    {
        BasicEs *b = GET_WRAPPED_OBJECT(es);
        b->setTerminationCriteria(fun, data);
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
double RVBasicEvolutionStrategyGetDesignParameter(struct RVBasicEvolutionStrategy *es, int parent, int paramIndex)
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
double RVBasicEvolutionStrategyGetObjective(struct RVBasicEvolutionStrategy *es, int parent, int objIndex)
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
void RVBasicEvolutionStrategySetRNG(struct RVBasicEvolutionStrategy *es, RV_RANDOM_FUNCTION fun, void *data)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		b->setRNG(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVBasicEvolutionStrategyDestroy(struct RVBasicEvolutionStrategy *es)
{
	if (es)
	{
		BasicEs *b = GET_WRAPPED_OBJECT(es);
		delete b;
		FREE_POD_OBJECT(es);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
struct RVDifferentialEvolution *RVDifferentialEvolutionCreate(unsigned int pnum, double Fp, double CRp, struct RVObjectiveFunction *fun)
{
    DifferentialEvolution *de = DifferentialEvolution::create(pnum, Fp, CRp, fun);
    if (de) {
        RVDifferentialEvolution *wrapper = CONSTRUCT_POD_OBJECT(RVDifferentialEvolution);
        SET_WRAPPED_OBJECT(wrapper, de);
        de->setWrapperObject(wrapper);
        return wrapper;
    }
    return 0;
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVDifferentialEvolutionInitializePopulation(struct RVDifferentialEvolution *de, RV_SET_INITIAL_VALUES_FUNCTION fun, void *data)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		dev->initializePopulation(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVDifferentialEvolutionSetOnGenerationFinishedFun(struct RVDifferentialEvolution *de, RV_DE_GENERATION_FINISHED_FUNCTION fun, void *data)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		dev->setOnGenerationFinished(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVDifferentialEvolutionSetTerminationFun(struct RVDifferentialEvolution *de, RV_DE_SHOULD_TERMINATE_FUNCTION fun, void *data)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		dev->setTermination(fun, data);
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
double RVDifferentialEvolutionGetDesignParameter(struct RVDifferentialEvolution *de, int agentIndex, int paramIndex)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		return dev->getDesignParameter(agentIndex, paramIndex);
	}
	return std::numeric_limits<double>::quiet_NaN();
}

/*---------------------------------------------------------------------------*/
extern "C"
double RVDifferentialEvolutionGetObjective(struct RVDifferentialEvolution *de, int agentIndex, int objectiveIndex)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		return dev->getObjective(agentIndex, objectiveIndex); // FIXME: should be 0 
	}
	return std::numeric_limits<double>::quiet_NaN();
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVDifferentialEvolutionStart(struct RVDifferentialEvolution *de)
{
	if (de)
	{
		DifferentialEvolution *dev = GET_WRAPPED_OBJECT(de);
		dev->start();
	}
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVDifferentialEvolutionDestroy(struct RVDifferentialEvolution *de)
{
    if (de)
    {
        DifferentialEvolution *in = GET_WRAPPED_OBJECT(de);
        delete in;
        FREE_POD_OBJECT(de);
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
RVCmaEvolutionStrategy* RVCmaEvolutionStrategyCreate(unsigned int lambda, RVObjectiveFunction *fun)
{
    CmaEs *es = CmaEs::create(lambda, fun);
    RVCmaEvolutionStrategy *wrapper = 0;
    if (es)
    {
        wrapper = CONSTRUCT_POD_OBJECT(RVCmaEvolutionStrategy);
        SET_WRAPPED_OBJECT(wrapper, es);
        es->setWrapperObject(wrapper);
    }
    return wrapper;
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVCmaEvolutionSetTerminationFun(struct RVCmaEvolutionStrategy *cma, RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data)
{
    if (cma)
    {
        CmaEs *in = GET_WRAPPED_OBJECT(cma);
        in->setTermination(fun, data);
    }
}

/*---------------------------------------------------------------------------*/
extern "C"
void RVCmaEvolutionStrategyDestroy(RVCmaEvolutionStrategy *es)
{
    if (es)
    {
        CmaEs *cma_es = GET_WRAPPED_OBJECT(es);
        delete cma_es;
        FREE_POD_OBJECT(es);
    }
}
