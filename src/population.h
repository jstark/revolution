#ifndef REVOLUTION_POPULATION_H_INCLUDED
#define REVOLUTION_POPULATION_H_INCLUDED

#include "atom.h"
#include "revolution.h"
#include <vector>

namespace revolution
{

template<typename AType>
class DLL_HIDDEN Population
{
public:
	Population(int parents, int offsprings, int dim);

	double designVariable(int atom, int var) const;
	void setDesignVariable(int atom, int var, double value);
private:
	std::vector<AType> atoms;
	const int mu;
	const int lambda;
	const int n; // dimensionality
};//~ Population

/*---------------------------------------------------------------------------*/
template<typename AType>
Population<AType>::Population(int parents, int offsprings, int dim)
	: atoms(parents+offsprings, AType(dim)), mu(parents), lambda(offsprings), n(dim)
{

}

/*---------------------------------------------------------------------------*/
template<typename AType>
double Population<AType>::designVariable(int atom_index, int var_index) const
{
	const AType& ref = atoms[atom_index];
	return ref[var_index];
}

/*---------------------------------------------------------------------------*/
template<typename AType>
void Population<AType>::setDesignVariable(int atom_index, int var_index, double value)
{
	AType& ref = atoms[atom_index];
	ref[var_index] = value;
}

}//~ revolution

#endif /* REVOLUTION_POPULATION_H_INCLUDED */
