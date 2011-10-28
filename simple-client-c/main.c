#include "revolution.h"
#include <stdio.h>

int evalf(const double *dv, double *obj)
{
	return 0;
}

int main(int argc, char *argv[])
{
	printf("API: %d.%d.%d\n", RVGetMajorVersion(), RVGetMinorVersion(), RVGetPatchVersion());
	printf("API: %d.%d.%d\n", RV_API_MAJOR, RV_API_MINOR, RV_API_PATCH);

	RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf);
	RVObjectiveFunctionDestroy(object);
	return 0;
}
