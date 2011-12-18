#ifndef REVOLUTION_UT_VERSION_H_INCLUDED
#define REVOLUTION_UT_VERSION_H_INCLUDED

#include "gtest/gtest.h"
#include "version.h"

namespace 
{
    TEST (VersionTest, checkMajorVersion)
    {
        ASSERT_EQ(revolution::Version::getMajor(), RV_API_MAJOR) << "Major version error !";
    }

    TEST (VersionTest, checkMinorVersion)
    {
        ASSERT_EQ(revolution::Version::getMinor(), RV_API_MINOR) << "Minor version error !";
    }

    TEST (VersionTest, checkPatchVersion)
    {
        ASSERT_EQ(revolution::Version::getPatch(), RV_API_PATCH) << "Patch version error !";
    }
}//~ namespace 

#endif /* REVOLUTION_UT_VERSION_H_INCLUDED */
