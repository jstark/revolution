#include "gtest/gtest.h"
#include "population.h"
#include "atom.h"
#include <vector>

namespace 
{
#define PARENTS 5
#define OFFSPRINGS 3
#define DIM 2
#define OBJ 1

    void init(double *params, double *objectives, void *data)
    {
        for (int i = 0; i < DIM; ++i)
        {
            params[i] = i;
        }
    }

    TEST (PopulationTest, populationInitializesCorrectly)
    {
        revolution::Population<revolution::Atom> p(PARENTS, OFFSPRINGS, DIM, OBJ);

        std::vector<revolution::Atom *> v = p.mem_ref();
        ASSERT_EQ(v.size(), PARENTS+OFFSPRINGS) << "Population size error!";

        for (std::vector<revolution::Atom *>::size_type sz = 0;
                sz != v.size(); ++sz)
        {
            ASSERT_EQ(DIM, v[sz]->dim()) << "Atom dimensionality error!";
            ASSERT_EQ(OBJ, v[sz]->obj()) << "Atom objectives error!";
            for (int i = 0; i < DIM; ++i)
            {
                EXPECT_DOUBLE_EQ(v[sz]->operator[](i), 0.0) << "Atom param error!";
            } 
        }
    
        p.initialize(init, 0);
        for (std::vector<revolution::Atom *>::size_type sz = 0;
                sz != v.size(); ++sz)
        {
            for (int i = 0; i < DIM; ++i)
            {
                EXPECT_DOUBLE_EQ(v[sz]->operator[](i), i) << "Atom param error!";
            }
        }
    }

}//~ namespace
