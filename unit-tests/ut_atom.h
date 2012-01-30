#ifndef REVOLUTION_UT_ATOM_H_INCLUDED
#define REVOLUTION_UT_ATOM_H_INCLUDED

#include "gtest/gtest.h"
#include "atom.h"
#include "objective_function.h"
#include "array.h"
#include <limits>

namespace 
{
    const double MAX_F_VAL = std::numeric_limits<double>::max();

    TEST (AtomTest, atomInitializesCorrectly)
    {
        revolution::Atom a(5,2);
        ASSERT_EQ(5, a.dim()) << "Dim error!";
        ASSERT_EQ(2, a.obj()) << "Obj error!";

        for (int i = 0; i < 5; ++i)
            EXPECT_DOUBLE_EQ(a[i], 0.0)  << "a[" << i << "] != 0.0";

        EXPECT_DOUBLE_EQ(a.f(0), MAX_F_VAL) << "a.f(0) != " << MAX_F_VAL;
        EXPECT_DOUBLE_EQ(a.f(1), MAX_F_VAL) << "a.f(1) != " << MAX_F_VAL;
    }

    TEST (AtomTest, setDesignVariables)
    {
        revolution::Atom a(5,2);

        for (int i = 0; i < 5; ++i)
        {
            a[i] = i;
            EXPECT_DOUBLE_EQ(a[i], i);
        }
    }

    class AtomFixture : public ::testing::Test 
    {
        protected:
            AtomFixture() 
            {
                sphere3 = revolution::ObjectiveFunction::create(3, 1, sphere3_eval, 0);
            }

            virtual ~AtomFixture()
            {
                delete sphere3;
            }  

            revolution::ObjectiveFunction *sphere3;

            static int sphere3_eval(const struct RVArray *params, struct RVArray *obj, void *data)
            {
                double x = RVArrayGetElementAtIndex(params, 0);
				double y = RVArrayGetElementAtIndex(params, 1);
				double z = RVArrayGetElementAtIndex(params, 2);
                RVArraySetElementAtIndex(obj, 0, x*x + y*y + z*z);
                return 0;
            }

            static void init(struct RVArray *params, struct RVArray *obj, void *data)
            {
                RVArraySetElementAtIndex(params, 0, 1.0);
                RVArraySetElementAtIndex(params, 1, 2.0);
                RVArraySetElementAtIndex(params, 2, 3.0);
            }
    };

    TEST_F (AtomFixture, checkObjectiveValues)
    {
        revolution::Atom a(3,1);

        a[0] = 1;
        a[1] = 2;
        a[2] = 3;

        a.eval(*sphere3);
        EXPECT_DOUBLE_EQ(a.f(0), 14.0) << "a.f(0) != 14";
    }

    TEST_F (AtomFixture, checkInitialization)
    {
        revolution::Atom a(3,1);

        a.initialize(init, 0);

        EXPECT_DOUBLE_EQ(a[0], 1.0) << "a[0] not initialized correctly!";
        EXPECT_DOUBLE_EQ(a[1], 2.0) << "a[1] not initialized correctly!";
        EXPECT_DOUBLE_EQ(a[2], 3.0) << "a[2] not initialized correctly!";
    }

    TEST_F (AtomFixture, checkSwap)
    {
        revolution::Atom a1(3, 1), a2(3,1);
        a1[0] = 2.0, a1[1] = 3.0, a1[2] = 4.0;
        a1.swap(a2);

        ASSERT_DOUBLE_EQ(a1[0], 0.0) << "a[0] is not 0!";
        ASSERT_DOUBLE_EQ(a1[1], 0.0) << "a[1] is not 0!";
        ASSERT_DOUBLE_EQ(a1[2], 0.0) << "a[2] is not 0!";
        ASSERT_DOUBLE_EQ(a2[0], 2.0) << "a[0] is not 2!";
        ASSERT_DOUBLE_EQ(a2[1], 3.0) << "a[1] is not 3!";
        ASSERT_DOUBLE_EQ(a2[2], 4.0) << "a[2] is not 4!";
    }

}//~ namespace

#endif
