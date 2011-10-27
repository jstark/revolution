#include "revolution.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "API: " 
		  << RVGetMajorVersion() << "." 
		  << RVGetMinorVersion() << "." 
		  << RVGetPatchVersion() << std::endl;

	std::cout << "API: "
			<< RV_API_MAJOR << "." << RV_API_MINOR << "." << RV_API_PATCH << std::endl;

	return 0;
}
