#include "../include/revolution.h"
#include "version.h"

using revolution::Version;

extern "C" 
int rv_major_version(void)
{
	return Version::getMajor();
}

extern "C"
int rv_minor_version(void)
{
	return Version::getMinor();
}

extern "C"
int rv_patch_version(void)
{
	return Version::getPatch();
}
