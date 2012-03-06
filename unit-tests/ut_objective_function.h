#ifndef REVOLUTION_UT_OBJECTIVE_FUNCTION_H_INCLUDED
#define REVOLUTION_UT_OBJECTIVE_FUNCTION_H_INCLUDED

#include "gtest/gtest.h"
#include "objective_function.h"
#include "atom.h"
#include "array.h"

namespace 
{
    int eval(const struct RVArray *params, struct RVArray *obj, void *data)
    {
        RVArraySetElementAtIndex(obj, 0, 1.0);
        RVArraySetElementAtIndex(obj, 1, 2.0);
        return 0;
    }

    TEST (ObjectiveFunction, testInvalidData)
    {
        revolution::ObjectiveFunction *ref = 0;
        
        ref = revolution::ObjectiveFunction::create(5, 2, 0, 0);
        ASSERT_EQ(ref, (void *)0) << "Initialized objective function with invalid EVALFUN!";

        ref = revolution::ObjectiveFunction::create(0, 2, eval, 0);
        ASSERT_EQ(ref, (void *)0) << "Initialized objective function with invalid n!";

        ref = revolution::ObjectiveFunction::create(5, 0, eval, 0);
        ASSERT_EQ(ref, (void *)0) << "Initialized objective function with invalid m!";

        delete ref;
    }

    TEST (ObjectiveFunction, testInitialization)
    {
        revolution::ObjectiveFunction *ref = 0;

        ref = revolution::ObjectiveFunction::create(5, 2, eval, 0);
        ASSERT_NE(ref, (void *)0) << "Did not initialize objective function with valid data";
        ASSERT_TRUE(ref->dim() == 5);
        ASSERT_TRUE(ref->objectives() == 2);

        delete ref;
    }

    TEST (ObjectiveFunction, testEval)
    {
        revolution::ObjectiveFunction *ref = 0;
        ref = revolution::ObjectiveFunction::create(3, 2, eval, 0);

        revolution::Atom a(3, 2);
        a.eval(*ref);

        ASSERT_DOUBLE_EQ(a.f(0), 1.0) << "Eval fun did not calculate f(0)";
        ASSERT_DOUBLE_EQ(a.f(1), 2.0) << "Eval fun did not calculate f(1)";

        delete ref;
    }

}//~ namespace

#endif 
