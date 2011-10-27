//
//  version.cpp
//  revolution
//
//  Created by John Fourkiotis on 10/15/11.
//  Copyright 2011  n/a. All rights reserved.
//

#include "version.h"
#include <cstdio>

using revolution::Version;

/*---------------------------------------------------------------------------*/
int Version::getMajor()
{
	return RV_API_MAJOR;
}

/*---------------------------------------------------------------------------*/
int Version::getMinor()
{
	return RV_API_MINOR;
}

/*---------------------------------------------------------------------------*/
int Version::getPatch()
{
	return RV_API_PATCH;
}

/*---------------------------------------------------------------------------*/
std::string Version::getVersion()
{
	static const int vbuffer_sz = 16;
	static char buffer[vbuffer_sz] = {0};
	
	// create a string representation lazily
	if (buffer[0] == '\0')
	{
		sprintf(buffer, "%d.%d.%d", getMajor(), getMinor(), getPatch());
	}
	return buffer;
}

/*---------------------------------------------------------------------------*/
bool Version::isAtLeast(int major, int minor, int patch)
{
	return (getMajor() >= major) && (getMinor() >= minor) && (getPatch() >= patch); 
}

/*---------------------------------------------------------------------------*/
bool Version::hasFeature(const std::string& feature_name)
{
	// TODO
	return false;
}


