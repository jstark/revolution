#include "revolution.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("API: %d.%d.%d\n", RVGetMajorVersion(), RVGetMinorVersion(), RVGetPatchVersion());
	printf("API: %d.%d.%d\n", RV_API_MAJOR, RV_API_MINOR, RV_API_PATCH);
	return 0;
}
