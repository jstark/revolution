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
		void setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data);
	private:
		BasicEs(int mu, int rho, int lambda, RVSelectionMode mode, ObjectiveFunction *objf);
		BasicEs(const BasicEs& rhs); // not implemented
		BasicEs& operator=(const BasicEs& rhs); // not implemented

		class BasicEsPriv;
		BasicEsPriv *impl;
	};//~ BasicEs
}//~ revolution

#endif /* REVOLUTION_BASIC_ES_HPP_INCLUDED */
