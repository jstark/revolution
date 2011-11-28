#include "revolution.h"
#include <cstdio>
#include <cmath>

#define MU          1
#define RHO         1
#define LAMBDA      1
#define OBJECTIVES  1
#define DIM         2
#define SELECTION kRVSelectionModePlus

/* f(x) = (x-10)^2 */
int evalf(const double *dv, double *obj)
{
    double x = dv[0];
    double y = dv[1];
    obj[0] = cos(x)*cos(y)*sin(x*y);
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
    printf("%d", gen);
    for (int i = 0; i < 1 /*MU+LAMBDA*/; ++i)
    {
        printf(" | %f %f [%f]", 
                RVBasicEvolutionStrategyGetDesignParameter(es, i, 0),
                RVBasicEvolutionStrategyGetDesignParameter(es, i, 1),
                RVBasicEvolutionStrategyGetObjective(es, i, 0));
    }
    printf("\n");
}

void init(double *params, double *objectives, void *data)
{
    params[0] = params[1] = 10.0;
}

int shouldTerminate(RVBasicEvolutionStrategy *es, unsigned int g, void *data)
{
    if (g == 100) return 1;
    return 0;
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(DIM, OBJECTIVES, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(MU, RHO, LAMBDA, SELECTION, object);
	RVBasicEvolutionStrategyInitializePopulation(es, init, 0);
	RVBasicEvolutionStrategyOnGenerationFinished(es, printBest, 0);
    RVBasicEvolutionStrategySetTerminationCriteria(es, shouldTerminate, 0);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
