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
		RV_OBJECTIVE_EVALUATION_FUNCTION evalFun() const { return evalf; }
		void *data() { return userData; }
		void eval(struct RVArray *const dv, struct RVArray *obj) const;

		static ObjectiveFunction* create(unsigned int dim, unsigned int objs, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data);
	private:
		ObjectiveFunction(unsigned int dim, unsigned int obj, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data);
		ObjectiveFunction(const ObjectiveFunction& rhs); // not implemented
		ObjectiveFunction& operator=(const ObjectiveFunction& rhs); // not implemented
		unsigned int dimensionality;
		unsigned int objectivesNumber;
		RV_OBJECTIVE_EVALUATION_FUNCTION evalf;
		void *userData;
	};//~ ObjectiveFunction
}//~revolution

#endif /* OBJECTIVE_FUNCTION_H_INCLUDED */
