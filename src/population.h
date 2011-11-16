#ifndef REVOLUTION_POPULATION_H_INCLUDED
#define REVOLUTION_POPULATION_H_INCLUDED

#include "atom.h"
#include <vector>

namespace revolution
{

class Population
{
public:
	Population(int parents, int offsprings, int dim);

	double designVariable(int atom, int var) const;
	void setDesignVariable(int atom, int var, double value);
private:
	std::vector<Atom> atoms;
	const int mu;
	const int lambda;
	const int n; // dimensionality
};//~ Population

}//~ revolution

#endif /* REVOLUTION_POPULATION_H_INCLUDED */