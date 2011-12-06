#include "revolution.h"
#include <stdio.h>

int evalf(const double *dv, double *obj)
{
	return 0;
}

int main(int argc, char *argv[])
{
	RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf);

	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(3, 1, 4, kRVSelectionModeComma, object);
	RVBasicEvolutionStrategyDestroy(es);
    struct RVCmaEvolutionStrategy *cma_es = RVCmaEvolutionStrategyCreate(10, object);
    RVCmaEvolutionStrategyDestroy(cma_es);
	RVObjectiveFunctionDestroy(object);
	return 0;
}
