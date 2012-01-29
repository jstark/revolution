#include "revolution.h"
#include <stdio.h>

int sphere2(const struct RVArray *dv, struct RVArray *obj, void *data)
{
	double x = RVArrayGetElementAtIndex(dv, 0);
	double y = RVArrayGetElementAtIndex(dv, 1);
	RVArraySetElementAtIndex(obj, 0, x*x + y*y);
	return 0;
}

void initPopulation(struct RVArray *dv, struct RVArray *obj, void *data)
{
	RVArraySetElementAtIndex(dv, 0, 10.0 * rand() / (double)RAND_MAX);
	RVArraySetElementAtIndex(dv, 1, 10.0 * rand() / (double)RAND_MAX);
}

int main(int argc, char *argv[])
{    
	struct RVObjectiveFunction *object = RVObjectiveFunctionCreate(2, 1, sphere2, NULL);
    struct RVDifferentialEvolution *de = RVDifferentialEvolutionCreate(10, 0.8, 0.2, object);
	RVDifferentialEvolutionInitializePopulation(de, initPopulation, NULL);
	RVDifferentialEvolutionStart(de);
	printf("X = %f, Y = %f\tF: %f\n",
		   RVDifferentialEvolutionGetDesignParameter(de, 0, 0),
		   RVDifferentialEvolutionGetDesignParameter(de, 0, 1),
		   RVDifferentialEvolutionGetObjective(de, 0, 0));
	RVDifferentialEvolutionDestroy(de);
    RVObjectiveFunctionDestroy(object);
	return 0;
}
