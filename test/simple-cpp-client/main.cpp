#include "revolution.h"
#include <iostream>

int evalf(const double *dv, double *obj)
{
	return 0;
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(3, 1, 4, kRVSelectionModePlus, object);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
