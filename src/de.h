#ifndef REVOLUTION_DE_H_INCLUDED
#define REVOLUTION_DE_H_INCLUDED

#include "revolution.h"

namespace revolution 
{
    
class DLL_HIDDEN DifferentialEvolution
{
public:
    static DifferentialEvolution *create(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun);
    void setWrapperObject(RVDifferentialEvolution *wrapper);
	void setTermination(RVDifferentialEvolutionShouldTerminate fun, void *data);
	void setOnGenerationFinished(RVDifferentialEvolutionOnGenerationFinished fun, void *data);
	void initializePopulation(RVDifferentialEvolutionPopulationSetInitialValues fun, void *data);
	double getDesignParameter(int agentIndex, int paramIndex) const;
	double getObjective(int agentIndex, int objectiveIndex) const;
	void start();
private:
    DifferentialEvolution(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun);
    struct DEImpl;
    DEImpl *pimpl;
};
    
}

#endif /* REVOLUTION_DE_H_INCLUDED */