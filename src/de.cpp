#include "de.h"
#include "population.h"
#include "atom.h"
#include <vector>

using revolution::DifferentialEvolution;
using revolution::Population;
using revolution::Atom;

/*---------------------------------------------------------------------------*/
struct DifferentialEvolution::DEImpl : private Population<Atom>
{
    DEImpl(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
    : Population<Atom>(p, 0, RVObjectiveFunctionGetDimensionality(fun), RVObjectiveFunctionGetNumberOfObjectives(fun)), 
      factor_(Fp), crossover_(CRp), objectiveFunction_(fun)
    {
        
    }
	
	void initializePopulation(RVDifferentialEvolutionPopulationSetInitialValues fun, void *data)
	{
		initialize(fun, data);
        for (std::vector<Atom *>::size_type sz = 0; sz != atom_ref.size(); ++sz)
        {
            //atom_ref[sz]->constrain(varConstrain, varConstrainData);
            atom_ref[sz]->eval(objectiveFunction_);
        }
	}
    
    void setWrapperObject(struct RVDifferentialEvolution *wrapper)
    {
        wrapper_ = wrapper;
    }
	
	void setOnGenerationFinished(RVDifferentialEvolutionOnGenerationFinished fun, void *data)
	{
		onGenFinished_ = fun;
		onGenFinishedData_ = data;
	}
	
	void setTermination(RVDifferentialEvolutionShouldTerminate fun, void *data)
	{
		evolutionShouldTerminate_ = fun;
		evolutionShouldTerminateData_ = data;
	}
	
	void start()
	{
        static const unsigned int MAX_GEN = 1000;
		
        unsigned int g = 0;
        while (true)
		{
			doEvolutionStep();
            ++g;
			
			if (onGenFinished_) 
            {
                onGenFinished_(wrapper_, g, onGenFinishedData_);
            }
			
            if (evolutionShouldTerminate_) 
            {
                int terminate = evolutionShouldTerminate_(wrapper_, g, evolutionShouldTerminateData_);
                if (terminate)
                {
                    break;
                }
            } else if (g == MAX_GEN)
            {
                break;
            }
		}
	}
	
	void doEvolutionStep()
	{
		
	}
    
    double factor_;
    double crossover_;
    struct RVObjectiveFunction *objectiveFunction_;
    struct RVDifferentialEvolution *wrapper_;
	RVDifferentialEvolutionShouldTerminate evolutionShouldTerminate_;
	void *onGenFinishedData_;
	RVDifferentialEvolutionOnGenerationFinished onGenFinished_;
	void *evolutionShouldTerminateData_;
	std::vector<Atom *> atom_ref;
};

/*---------------------------------------------------------------------------*/
DifferentialEvolution * DifferentialEvolution::create(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
{
    // sanity checks:
    bool fpValid = Fp > 0;
    bool crpValid = CRp > 0;
    bool funValid = fun ? true : false;
    
    return (fpValid && crpValid && funValid) ? new DifferentialEvolution(p, Fp, CRp, fun) : 0;
}

/*---------------------------------------------------------------------------*/
void DifferentialEvolution::setWrapperObject(struct RVDifferentialEvolution *wrapper)
{
    pimpl->setWrapperObject(wrapper);
}

/*---------------------------------------------------------------------------*/
void DifferentialEvolution::setTermination(RVDifferentialEvolutionShouldTerminate fun, void *data)
{
	pimpl->setTermination(fun, data);
}

/*---------------------------------------------------------------------------*/
void DifferentialEvolution::setOnGenerationFinished(RVDifferentialEvolutionOnGenerationFinished fun, void *data)
{
	pimpl->setOnGenerationFinished(fun, data);
}

/*---------------------------------------------------------------------------*/
void DifferentialEvolution::initializePopulation(RVDifferentialEvolutionPopulationSetInitialValues fun, void *data)
{
	pimpl->initializePopulation(fun, data);
}

/*---------------------------------------------------------------------------*/
DifferentialEvolution::DifferentialEvolution(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
: pimpl(new DifferentialEvolution::DEImpl(p, Fp, CRp, fun))
{
    
}