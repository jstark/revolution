#ifndef REVOLUTION_UT_BASIC_ES_H_INCLUDED
#define REVOLUTION_UT_BASIC_ES_H_INCLUDED

#include "gtest/gtest.h"
#include "basic_es.h"
#include "objective_function.h"

namespace 
{
    TEST (BasicEsTest, checkInvalidInitialization)
    {
        revolution::BasicEs *bes = 0;
        
        bes = revolution::BasicEs::create(-1, 1, 1, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid μ";        

        bes = revolution::BasicEs::create(0, 1, 1, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid μ";        

        bes = revolution::BasicEs::create(1, -1, 1, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid ρ";        

        bes = revolution::BasicEs::create(1, 0, 1, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid ρ";        

        bes = revolution::BasicEs::create(1, 1, -1, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid λ";

        bes = revolution::BasicEs::create(1, 1, 0, kRVSelectionModeComma, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid λ";

        bes = revolution::BasicEs::create(1, 1, 1, kRVSelectionModeInvalid, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid selection op";

        bes = revolution::BasicEs::create(1, 1, 1, (RVSelectionMode)5, 0);
        ASSERT_EQ(bes, (void *)0) << "Initialization with invalid selection op";
    }
}//~ namespace

#endif /* REVOLUTION_UT_BASIC_ES_H_INCLUDED */
