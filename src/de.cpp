#include "de.h"
#include "population.h"
#include "atom.h"
#include <vector>
#include <set>
#include <algorithm>

using revolution::DifferentialEvolution;
using revolution::Population;
using revolution::Atom;

static bool compare_single_obj(const Atom* a1, const Atom* a2)
{
	return a1->f(0) < a2->f(0);
}

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
	
	double getDesignParameter(int agentIndex, int paramIndex) const
	{
		return atom_ref[agentIndex]->operator[](paramIndex);
	}
	
	double getObjective(int agentIndex, int objectiveIndex) const
	{
		return atom_ref[agentIndex]->f(objectiveIndex);
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
		
		// after evolution finishes, we need to sort
		std::sort(atom_ref.begin(), atom_ref.end(), compare_single_obj);
	}
	
	void doEvolutionStep()
	{
		const int populationSize = atom_ref.size();
		const int dimensionality = RVObjectiveFunctionGetDimensionality(objectiveFunction_);
		for (std::vector<Atom *>::size_type sz = 0; sz != populationSize; ++sz)
		{
			std::set<Atom *> dinstictAgents;
			while (dinstictAgents.size() != 3)
			{
				int randomAgentIndex = rand() % populationSize;
				if (randomAgentIndex != sz)
				{
					dinstictAgents.insert(atom_ref[randomAgentIndex]);
				}
			}
			
			Atom clonedAgent = *atom_ref[sz];
			for (int i = 0; i < dimensionality; ++i)
			{
				int randomIndex = rand() % dimensionality;
				double randomUnifr = rand() / RAND_MAX;
				if (randomUnifr < crossover_ || i == randomIndex-1)
				{
					// FIXME:
					std::vector<Atom *> dAgents;
					std::copy(dinstictAgents.begin(), dinstictAgents.end(), back_inserter(dAgents));
					clonedAgent[i] = (*dAgents[0])[i] + factor_ * ((*dAgents[1])[i] - (*dAgents[2])[i]);
				} else
				{
					clonedAgent[i] = (*atom_ref[sz])[i];
				}
			}
			
			clonedAgent.eval(objectiveFunction_);
			if (clonedAgent.f(0) < atom_ref[sz]->f(0)) // FIXME: hardcoded ONE objective ONLY
			{
				atom_ref[sz]->swap(clonedAgent);
			}
			
		}
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
void DifferentialEvolution::start()
{
	pimpl->start();
}

/*---------------------------------------------------------------------------*/
double DifferentialEvolution::getDesignParameter(int agentIndex, int paramIndex) const
{
	return pimpl->getDesignParameter(agentIndex, paramIndex);
}

/*---------------------------------------------------------------------------*/
double DifferentialEvolution::getObjective(int agentIndex, int objectiveIndex) const
{
	return pimpl->getObjective(agentIndex, objectiveIndex);
}

/*---------------------------------------------------------------------------*/
DifferentialEvolution::DifferentialEvolution(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun)
: pimpl(new DifferentialEvolution::DEImpl(p, Fp, CRp, fun))
{
    
}