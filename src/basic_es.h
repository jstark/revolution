#ifndef REVOLUTION_BASIC_ES_HPP_INCLUDED
#define REVOLUTION_BASIC_ES_HPP_INCLUDED

#include "revolution.h"

namespace revolution
{
	class ObjectiveFunction;

	class DLL_HIDDEN BasicEs
	{
	public:
		static BasicEs* create(int mu, int rho, int lambda, RVSelectionMode mode, ObjectiveFunction *objf);

		~BasicEs();
		void setPopulationInitialValues(RV_SET_INITIAL_VALUES_FUNCTION fun, void *data);
		void setOnGenerationFinished(RV_BASIC_GENERATION_FINISHED_FUNCTION fun, void *data);
		void start();
		double getDesignParameter(int parent, int paramIndex) const;
		double getObjective(int parent, int objIndex) const;
		void setWrapperObject(RVBasicEvolutionStrategy *obj);
       	void setTerminationCriteria(RV_BASIC_SHOULD_TERMINATE_FUNCTION fun, void *data);
		void setRNG(RV_RANDOM_FUNCTION fun, void *data);
        void setParamConstraints(RV_CONSTRAIN_PARAMS_FUNCTION fun, void *data);
	private:
		BasicEs(int mu, int rho, int lambda, RVSelectionMode mode, ObjectiveFunction *objf);
		BasicEs(const BasicEs& rhs); // not implemented
		BasicEs& operator=(const BasicEs& rhs); // not implemented

		class BasicEsPriv;
		BasicEsPriv *impl;
	};//~ BasicEs
}//~ revolution

#endif /* REVOLUTION_BASIC_ES_HPP_INCLUDED */
