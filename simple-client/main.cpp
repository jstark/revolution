#include "revolution.h"
#include <iostream>

int main(int argc, char *argv[])
{
	std::cout << "API: " 
		  << rv_major_version() << "." 
		  << rv_minor_version() << "." 
		  << rv_patch_version() << std::endl;
	return 0;
}
