#ifndef REVOLUTION_ATOM_H_INCLUDED
#define REVOLUTION_ATOM_H_INCLUDED

#include <vector>
#include "revolution.h"


namespace revolution
{

class ObjectiveFunction;

class DLL_HIDDEN Atom
{
public:
	Atom(int dim, int m);

	double operator[](int index) const;
	double& operator[](int index);

	void eval(const ObjectiveFunction& fun);
	void eval(struct RVObjectiveFunction *obj);
	double f(int index) const;
    void constrain(RV_CONSTRAIN_PARAMS_FUNCTION fun, void *data);

	int dim() const;
	int obj() const;

	void swap(Atom& rhs);
	void initialize(RV_SET_INITIAL_VALUES_FUNCTION fun, void *data);

private:
	std::vector<double> params;
	std::vector<double> objectives;
};//~

}//~ revolution

#endif /* REVOLUTION_ATOM_H_INCLUDED */
