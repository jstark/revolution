#include "de.h"
#include "population.h"
#include "atom.h"

using revolution::DifferentialEvolution;
using revolution::Population;
using revolution::Atom;

struct DifferentialEvolution::DEImpl : private Population<Atom>
{
    DEImpl(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
    : Population<Atom>(p, 0, RVObjectiveFunctionGetDimensionality(fun), RVObjectiveFunctionGetNumberOfObjectives(fun)), 
      factor_(Fp), crossover_(CRp), objectiveFunction_(fun)
    {
        
    }
    
    void setWrapperObject(struct RVDifferentialEvolution *wrapper)
    {
        wrapper_ = wrapper;
    }
    
    double factor_;
    double crossover_;
    struct RVObjectiveFunction *objectiveFunction_;
    struct RVDifferentialEvolution *wrapper_;
};

DifferentialEvolution * DifferentialEvolution::create(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
{
    // sanity checks:
    bool fpValid = Fp > 0;
    bool crpValid = CRp > 0;
    bool funValid = fun ? true : false;
    
    return (fpValid && crpValid && funValid) ? new DifferentialEvolution(p, Fp, CRp, fun) : 0;
}

void DifferentialEvolution::setWrapperObject(struct RVDifferentialEvolution *wrapper)
{
    pimpl->setWrapperObject(wrapper);
}

DifferentialEvolution::DifferentialEvolution(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
: pimpl(new DifferentialEvolution::DEImpl(p, Fp, CRp, fun))
{
    
}