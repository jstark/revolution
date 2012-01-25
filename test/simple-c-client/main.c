#include "revolution.h"
#include <stdio.h>

int evalf(const struct RVArray *dv, struct RVArray *obj, void *data)
{
	return 0;
}

int main(int argc, char *argv[])
{
	struct RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf, NULL);
	struct RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(3, 1, 4, kRVSelectionModeComma, object);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);
	return 0;
}
