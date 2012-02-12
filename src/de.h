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
	void setTermination(RV_DE_SHOULD_TERMINATE_FUNCTION fun, void *data);
	void setOnGenerationFinished(RV_DE_GENERATION_FINISHED_FUNCTION fun, void *data);
	void initializePopulation(RV_SET_INITIAL_VALUES_FUNCTION fun, void *data);
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