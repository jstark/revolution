#include "revolution.h"
#include <iostream>

/* f(x) = (x-10)^2 */
int evalf(const double *dv, double *obj)
{
	obj[0] = (dv[0]-10.0)*(dv[0]-10);
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
	std::cout << "[Generation: " << gen << "]\n";
	std::cout << "\tBest  F: " << RVBasicEvolutionStrategyGetObjective(es, 0, 0) << std::endl;
	std::cout << "\tParam X: " << RVBasicEvolutionStrategyGetDesignParameter(es, 0, 0) << std::endl;
}

void init(double *params, double *objectives, void *data)
{
	/* just calculate f */
	evalf(params, objectives);
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(1, 1, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(1, 1, 1, kRVSelectionModePlus, object);
	RVBasicEvolutionStrategyInitializePopulation(es, init, 0);
	RVBasicEvolutionStrategyOnGenerationFinished(es, printBest, 0);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
