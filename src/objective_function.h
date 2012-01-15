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

			void eval(struct RVArray *const dv, struct RVArray *obj) const;

			static ObjectiveFunction* create(unsigned int dim, unsigned int objs, RVObjectiveEvaluationFun fun, void *data);
	private:
			ObjectiveFunction(unsigned int dim, unsigned int obj, RVObjectiveEvaluationFun fun, void *data);
			ObjectiveFunction(const ObjectiveFunction& rhs); // not implemented
			ObjectiveFunction& operator=(const ObjectiveFunction& rhs); // not implemented
			unsigned int dimensionality;
			unsigned int objectivesNumber;
			RVObjectiveEvaluationFun evalf;
            void *userData;
	};//~ ObjectiveFunction
}//~revolution

#endif /* OBJECTIVE_FUNCTION_H_INCLUDED */
