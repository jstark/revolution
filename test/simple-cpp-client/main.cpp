#include "revolution.h"
#include <cstdio>

#define MU          1
#define RHO         1
#define LAMBDA      1
#define OBJECTIVES  1
#define DIM         2
#define SELECTION kRVSelectionModeComma

/* f(x) = (x-10)^2 */
int evalf(const double *dv, double *obj)
{
    double x = dv[0];
    double y = dv[1];
    obj[0] = (1.0-x)*(1.0-x)+100.0*(y-x*x)*(y-x*x);
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
    printf("%d", gen);
    for (int i = 0; i < MU+LAMBDA; ++i)
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
	/* just calculate f */
    params[0] = params[1] = 10.0;
	evalf(params, objectives);
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(DIM, OBJECTIVES, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(MU, RHO, LAMBDA, SELECTION, object);
	RVBasicEvolutionStrategyInitializePopulation(es, init, 0);
	RVBasicEvolutionStrategyOnGenerationFinished(es, printBest, 0);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
