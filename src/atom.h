#ifndef REVOLUTION_ATOM_H_INCLUDED
#define REVOLUTION_ATOM_H_INCLUDED

#include <vector>
#include "revolution.h"

namespace revolution
{
class DLL_HIDDEN Atom
{
public:
	explicit Atom(int dim);

	double operator[](int index) const;
	double& operator[](int index);
private:
	std::vector<double> params;
};//~

}//~ revolution

#endif /* REVOLUTION_ATOM_H_INCLUDED */
