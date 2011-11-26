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
	Population(int parents, int offsprings, int dim, int obj);

	void initialize(RVPopulationSetInitialValues fun, void *data);
	std::vector<AType *> mem_ref();
private:
	std::vector<AType> atoms;
};//~ Population

/*---------------------------------------------------------------------------*/
template<typename AType>
Population<AType>::Population(int parents, int offsprings, int dim, int obj)
	: atoms(parents+offsprings, AType(dim, obj))
{

}

/*---------------------------------------------------------------------------*/
template<typename AType>
void Population<AType>::initialize(RVPopulationSetInitialValues fun, void *data)
{
	for (typename std::vector<AType>::size_type sz = 0;
		sz != atoms.size(); ++sz)
	{
		atoms[sz].initialize(fun, data); // FIXME: will init children also
	}
}

/*---------------------------------------------------------------------------*/
template<typename AType>
std::vector<AType *> Population<AType>::mem_ref()
{
	std::vector<AType *> ref;
	for (typename std::vector<AType>::size_type sz = 0;
		sz != atoms.size(); ++sz)
	{
		ref.push_back(&atoms[sz]);
	}
	return ref;
}

}//~ revolution

#endif /* REVOLUTION_POPULATION_H_INCLUDED */
