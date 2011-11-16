#include "population.h"

using revolution::Population;

/*---------------------------------------------------------------------------*/
Population::Population(int parents, int offsprings, int dim)
	: atoms(parents+offsprings, Atom(dim)), mu(parents), lambda(offsprings), n(dim)
{

}

/*---------------------------------------------------------------------------*/
double Population::designVariable(int atom, int var) const
{
	const Atom& ref = atoms[atom];
	return ref[var];
}

/*---------------------------------------------------------------------------*/
void Population::setDesignVariable(int atom, int var, double value)
{
	Atom& ref = atoms[atom];
	ref[var] = value;
}