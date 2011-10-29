#include "revolution.h"
#include <iostream>

int evalf(const double *dv, double *obj)
{
	return 0;
}

int main(int argc, char *argv[])
{
	std::cout << "API: " 
		  << RVGetMajorVersion() << "." 
		  << RVGetMinorVersion() << "." 
		  << RVGetPatchVersion() << std::endl;

	std::cout << "API: "
			<< RV_API_MAJOR << "." << RV_API_MINOR << "." << RV_API_PATCH << std::endl;

	RVObjectiveFunction *object = RVObjectiveFunctionCreate(3, 2, evalf);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
