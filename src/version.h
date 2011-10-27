//
//  version.h
//  revolution
//
//  Created by John Fourkiotis on 10/15/11.
//  Copyright 2011  n/a. All rights reserved.
//

#ifndef REVOLUTION_VERSION_H_INCLUDED
#define REVOLUTION_VERSION_H_INCLUDED

#include "revolution.h"
#include <string>

namespace revolution {
	class Version 
	{
	public:
		static int getMajor();
		static int getMinor();
		static int getPatch();
		static std::string getVersion();
		static bool isAtLeast(int major, int minor, int patch);
		static bool hasFeature(const std::string& feature_name);
	};//~ Version
}//~ revolution

#endif
