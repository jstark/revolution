//
//  version.h
//  revolution
//
//  Created by John Fourkiotis on 10/15/11.
//  Copyright 2011  n/a. All rights reserved.
//

#ifndef revolution_version_h
#define revolution_version_h

#include <string>

namespace revolution {
	/*
	 * API Version based on API Design for C++, MK, Martin Reddy, 2011
	 *
	 */
	
	#define API_MAJOR	0
	#define API_MINOR	0
	#define API_PATCH	0
	
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
