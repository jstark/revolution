#ifndef REVOLUTION_UT_DE_H_INCLUDED
#define REVOLUTION_UT_DE_H_INCLUDED

#include "gtest/gtest.h"
#include "de.h"
#include "revolution.h"

namespace 
{
	TEST (DETest, checkInvalidInitialization)
	{
		revolution::DifferentialEvolution *de = 0;
		
		de = revolution::DifferentialEvolution::create(0, 0.5, 0.5, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid population number (>= 4)";
		
		de = revolution::DifferentialEvolution::create(2, 0.5, 0.5, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid population number (>= 4)";
		
		de = revolution::DifferentialEvolution::create(4, -1.2, 0.5, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid F (0 <= F <= 2)";
		
		de = revolution::DifferentialEvolution::create(4, 6.1, 0.5, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid F (0 <= F <= 2)";
		
		de = revolution::DifferentialEvolution::create(4, 1.1, -4.0, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid CR (0 <= CR <= 1)";
		
		de = revolution::DifferentialEvolution::create(4, 1.1, 6.0, (RVObjectiveFunction *)0xFF);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid CR (0 <= CR <= 1)";
		
		de = revolution::DifferentialEvolution::create(4, 1.1, 0.5, 0);
		ASSERT_EQ(de, (void *)0) << "Initialization with invalid function (!= 0)";
	}
}
#endif /* REVOLUTION_UT_DE_H_INCLUDED */