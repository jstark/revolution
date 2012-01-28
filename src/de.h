#ifndef REVOLUTION_DE_H_INCLUDED
#define REVOLUTION_DE_H_INCLUDED

#include "revolution.h"

namespace revolution 
{
    
class DLL_HIDDEN DifferentialEvolution
{
public:
    static DifferentialEvolution *create(unsigned int p, double Fp, double CRp, struct RVObjectiveFunction *fun);
private:
    struct DEImpl;
    DEImpl *pimpl;
};
    
}

#endif /* REVOLUTION_DE_H_INCLUDED */