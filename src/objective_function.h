#ifndef OBJECTIVE_FUNCTION_H_INCLUDED
#define OBJECTIVE_FUNCTION_H_INCLUDED

#include "revolution.h"

namespace revolution
{
	class ObjectiveFunction
	{
	public:
			const int dim() const;
			const int objectives() const;
			void eval(const double *dv, double *obj) const;

			static ObjectiveFunction* create(int dim, int objs, RVObjectiveEvalFun fun);
	private:
			ObjectiveFunction(int dim, int obj, RVObjectiveEvalFun fun);
			ObjectiveFunction(const ObjectiveFunction& rhs); // not implemented
			ObjectiveFunction& operator=(const ObjectiveFunction& rhs); // not implemented
			int dimensionality;
			int objectivesNumber;
			RVObjectiveEvalFun evalf;
	};//~ ObjectiveFunction
}//~revolution

#endif /* OBJECTIVE_FUNCTION_H_INCLUDED */
