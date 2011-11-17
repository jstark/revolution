#include "atom.h"

using revolution::Atom;

/*---------------------------------------------------------------------------*/
Atom::Atom(int dim)
	: params(dim, 0.0) 
{

}

/*---------------------------------------------------------------------------*/
double Atom::operator[](int index) const
{
	return params.at(index);
}

/*---------------------------------------------------------------------------*/
double& Atom::operator[](int index)
{
	return params.at(index);
}
