#include "revolution.h"
#include <iostream>

int evalf(const double *dv, double *obj)
{
	return 0;
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(3, 1, 4, kRVSelectionModeComma, object);
	std::cout << " --START-- \n";
	RVBasicEvolutionStrategyStart(es);
	std::cout << " -FINISHED- \n";
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
