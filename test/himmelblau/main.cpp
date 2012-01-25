#include "revolution.h"
#include <cstdio>
#include <cmath>

#define MU          200
#define RHO         1 
#define LAMBDA      200
#define OBJECTIVES  1
#define DIM         2
#define SELECTION kRVSelectionModePlus

int himmelblau(struct RVArray const *dv, struct RVArray *obj, void *data)
{
    double x = RVArrayGetElementAtIndex(dv, 0);
    double y = RVArrayGetElementAtIndex(dv, 1);
    double f = (x*x + y - 11) * (x*x + y - 11) + (x + y*y - 7)*(x + y*y - 7);
    RVArraySetElementAtIndex(obj, 0, f);
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
    /*
    printf("%d\t", gen);
    printf("%f\t%f\t%f", 
            RVBasicEvolutionStrategyGetDesignParameter(es, 0, 0),
            RVBasicEvolutionStrategyGetDesignParameter(es, 0, 1),
            RVBasicEvolutionStrategyGetObjective(es, 0, 0));
    printf("\n");
    */
}

void init(struct RVArray *params, struct RVArray *objectives, void *data)
{
    RVArraySetElementAtIndex(params, 0, 0.0);
    RVArraySetElementAtIndex(params, 1, 0.0);
}

int shouldTerminate(RVBasicEvolutionStrategy *es, unsigned int g, void *data)
{
    if (g == 1000) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(DIM, OBJECTIVES, himmelblau, NULL);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(MU, RHO, LAMBDA, SELECTION, object);
	RVBasicEvolutionStrategyInitializePopulation(es, init, 0);
	RVBasicEvolutionStrategyOnGenerationFinished(es, printBest, 0);
    RVBasicEvolutionStrategySetTerminationCriteria(es, shouldTerminate, 0);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
