#ifndef OBJECTIVE_FUNCTION_H_INCLUDED
#define OBJECTIVE_FUNCTION_H_INCLUDED

#include "revolution.h"

namespace revolution
{
	class DLL_HIDDEN ObjectiveFunction
	{
	public:
        const unsigned int dim() const;
        const unsigned int objectives() const;
        RVObjectiveEvalFun evalFun() const { return evalf; }
        void *data() { return userData; }
        void eval(const double *dv, double *obj) const;

        static ObjectiveFunction* create(unsigned int dim, unsigned int objs, RVObjectiveEvalFun fun, void *data);
	private:
        ObjectiveFunction(unsigned int dim, unsigned int obj, RVObjectiveEvalFun fun, void *data);
        ObjectiveFunction(const ObjectiveFunction& rhs); // not implemented
        ObjectiveFunction& operator=(const ObjectiveFunction& rhs); // not implemented
        unsigned int dimensionality;
        unsigned int objectivesNumber;
        RVObjectiveEvalFun evalf;
        void *userData;
	};//~ ObjectiveFunction
}//~revolution

#endif /* OBJECTIVE_FUNCTION_H_INCLUDED */
