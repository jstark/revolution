#ifndef REVOLUTION_ATOM_H_INCLUDED
#define REVOLUTION_ATOM_H_INCLUDED

#include <vector>
#include "revolution.h"

namespace revolution
{
class DLL_HIDDEN Atom
{
public:
	Atom(int dim, int m);

	double operator[](int index) const;
	double& operator[](int index);

	void eval(RVObjectiveEvalFun fun);
	double f(int index) const;

	int dim() const;
	int obj() const;

private:
	std::vector<double> params;
	std::vector<double> objectives;
};//~

}//~ revolution

#endif /* REVOLUTION_ATOM_H_INCLUDED */
