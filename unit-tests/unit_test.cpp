#include "ut_atom.h"
#include "ut_population.h"
#include "ut_objective_function.h"
#include "ut_version.h"
#include "ut_basic_es.h"
#include "gtest/gtest.h"

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
