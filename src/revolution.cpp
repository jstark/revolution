#include "revolution.h"
#include "version.h"

using revolution::Version;

extern "C" 
int RVGetMajorVersion(void)
{
	return Version::getMajor();
}

extern "C"
int RVGetMinorVersion(void)
{
	return Version::getMinor();
}

extern "C"
int RVGetPatchVersion(void)
{
	return Version::getPatch();
}
