#include "basic_es.h"
#include "objective_function.h"
#include "population.h"
#include "atom.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <set>
#include <algorithm>

using revolution::BasicEs;
using revolution::ObjectiveFunction;

/*--------------------------------------------------------------------------*/
namespace 
{

const double PI = 3.14159265;

double normal_dist_num()
{
	static bool cache = false;
	static double cached = 0;

	if (cache)
	{
		cache = false;
		return cached;
	}

	double U = rand() / (1.0 * RAND_MAX);
	double V = rand() / (1.0 * RAND_MAX);
	double X = sqrt(-2.0 * log(U)) * cos(2.0 * PI * V);
	double Y = sqrt(-2.0 * log(U)) * sin(2.0 * PI * V);

	cache = true;
	cached = Y;
	return X;
}//~ normal_dist_num

/*--------------------------------------------------------------------------*/
class Atom : private revolution::Atom
{
public:
	explicit Atom(int n, int m)
		: revolution::Atom(n, m), strParams(n, 1.0)
	{

	}

	using revolution::Atom::operator[];
	using revolution::Atom::f;
	using revolution::Atom::dim;
	using revolution::Atom::obj;
	using revolution::Atom::initialize;
	using revolution::Atom::eval;

	double strategyParam(int index) const 
	{
		return strParams.at(index);
	}

	void setStrategyParam(int index, double value)
	{
		strParams[index] = value;
	}

	void swap(::Atom& rhs)
	{
		revolution::Atom::swap(rhs); //FIXME: slice
		strParams.swap(rhs.strParams);
	}

private:
	std::vector<double> strParams; // endogenous strategy params
};//~Atom for BasicEs

/*--------------------------------------------------------------------------*/
class Population : private revolution::Population<Atom>
{
public:
	Population(int mu, int lambda, int dim, int obj)
			: revolution::Population<Atom>(mu, lambda, dim, obj)
	{

	}

	using revolution::Population<Atom>::initialize;
	using revolution::Population<Atom>::mem_ref;
};//~ BasicEs::Population

/*--------------------------------------------------------------------------*/
void gen_random_indices(int max, std::set<int>& indices)
{
	while (indices.size() != max)
	{
		indices.insert(rand() % max);
	}
}

/*--------------------------------------------------------------------------*/
class Recombination 
{
public:
	Recombination(int r, int dim) : rho(r), n(dim) {}

	void apply(const std::vector<Atom *>& p, Atom& temp) const// intermediate recombination (for the moment)
	{
		std::set<int> random_indices;
		gen_random_indices(rho, random_indices);
		for (int varIndex = 0; varIndex != n; ++varIndex)
		{
			double paramSum = 0.0;
			double strategySum = 0.0;
			Atom *atom = 0;
			for (std::set<int>::const_iterator i = random_indices.begin();
				i != random_indices.end(); ++i)
			{
				atom = p[*i];
				paramSum += atom->operator[](varIndex);
				strategySum += atom->strategyParam(varIndex);
			}
			paramSum /= rho;
			strategySum /= rho;
			temp[varIndex] = paramSum;
			temp.setStrategyParam(varIndex, strategySum);
		}
	}

private:
	const int rho;
	const int n;
	
};//~ Recombination

/*--------------------------------------------------------------------------*/
class Mutation
{
public:
	void apply(Atom& temp) const
	{
		static const int n = temp.dim();
		static const double t0= 1.0/(2.0 * sqrt(1.0*n));
		static const double t = 1.0/(sqrt(2.0 * sqrt(1.0*n)));

		double m0 = exp(t0 * normal_dist_num());
		double mi = 0;
		for (int varIndex = 0; varIndex != n; ++varIndex)
		{
			mi = temp.strategyParam(varIndex);
			mi *= exp(t*normal_dist_num());
			mi *= m0;
			temp.setStrategyParam(varIndex, mi);
			temp[varIndex] += mi * normal_dist_num();
		}
	}

};//~ Mutation

/*--------------------------------------------------------------------------*/
bool cmp_single_obj(const Atom* i, const Atom* j)
{
	return i->f(0) < j->f(0);
}

/*--------------------------------------------------------------------------*/
class Selection
{
public:
	explicit Selection(RVSelectionMode md) 
		: mode(md) {}

	void apply(std::vector<Atom *>& pop, int mu, int lambda) const
	{
		if (mode == kRVSelectionModePlus)
		{
			std::partial_sort(
				pop.begin(), pop.begin()+mu, pop.end(), cmp_single_obj);
		} else if (mode == kRVSelectionModeComma)
		{
            std::partial_sort(pop.begin()+mu, pop.begin()+mu+mu, pop.end(), cmp_single_obj);
            for (std::vector<Atom *>::size_type sz = mu, i = 0;
                sz != 2*mu; ++sz, ++i)
            {
                std::swap(pop[i], pop[sz]);
            }
		}
	}

private:
	RVSelectionMode mode;
};//~ Selection

}//~ unamed

/*--------------------------------------------------------------------------*/
class BasicEs::BasicEsPriv : private ::Population
{
public:
	BasicEsPriv(int m, int r, int l, RVSelectionMode mode, ObjectiveFunction *objf)
		: 
		  ::Population(m, l, objf->dim(), objf->objectives()), 
		  recombination(r, objf->dim()),
		  mutation(),
		  selection(mode), 
		  objectiveFunction(objf), 
		  mu(m), lambda(l),
		  temp(objf->dim(), objf->objectives()),
		  onGenFinished(0),
		  onGenFinishedData(0),
		  wrapperObj(0)
	{
		set_ref();
	}

	void setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
	{
		initialize(fun, data);
	}

	void setOnGenerationFinished(RVGenerationFinished fun, void *data)
	{
		onGenFinished = fun;
		onGenFinishedData = data;
	}

	void start()
	{
		const int MAX_GEN = 1000;
		for (int i = 0; i < MAX_GEN; ++i)
		{
			doEvolutionStep();
			if (onGenFinished) onGenFinished(wrapperObj, i+1, onGenFinishedData);
		}
	}

	double getDesignParameter(int parent, int paramIndex) const
	{
		return atom_ref[parent]->operator[](paramIndex);
	}

	void doEvolutionStep()
	{
		for (int i = 0; i < lambda; ++i)
		{
			recombination.apply(atom_ref, temp);			
			mutation.apply(temp);
			temp.eval(*objectiveFunction);
			atom_ref[mu+i]->swap(temp);
		}
		selection.apply(atom_ref, mu, lambda);
	}

	double getObjective(int parent, int objIndex) const
	{
		return atom_ref[parent]->f(objIndex);
	}

	void setWrapperObject(RVBasicEvolutionStrategy *obj)
	{
		wrapperObj = obj;
	}

	void set_ref()
	{
		atom_ref = mem_ref();
	}

	::Atom temp;
	std::vector< ::Atom *> atom_ref;
	const ::Recombination recombination;
	const ::Selection selection;
	const ::Mutation mutation;
	ObjectiveFunction *objectiveFunction;
	const int mu;
	const int lambda;
	RVGenerationFinished onGenFinished;
	void *onGenFinishedData;
	RVBasicEvolutionStrategy *wrapperObj;
};//~BasicEs::BasicEsPriv

/*---------------------------------------------------------------------------*/
BasicEs::BasicEs(int m, int r, int la, RVSelectionMode sel, ObjectiveFunction *objf)
	: impl(new BasicEs::BasicEsPriv(m, r, la, sel, objf))
{

}

/*---------------------------------------------------------------------------*/
BasicEs::~BasicEs()
{
	delete impl;
	impl = 0;
}

/*--------------------------------------------------------------------------*/
void BasicEs::setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
{
	impl->setPopulationInitialValues(fun, data);
}

/*--------------------------------------------------------------------------*/
void BasicEs::start()
{
	impl->start();
}

/*--------------------------------------------------------------------------*/
void BasicEs::setOnGenerationFinished(RVGenerationFinished fun, void *data)
{
	impl->setOnGenerationFinished(fun, data);
}

/*--------------------------------------------------------------------------*/
double BasicEs::getDesignParameter(int parent, int paramIndex) const
{
	return impl->getDesignParameter(parent, paramIndex);
}

/*--------------------------------------------------------------------------*/
double BasicEs::getObjective(int parent, int objIndex) const
{
	return impl->getObjective(parent, objIndex);
}

/*--------------------------------------------------------------------------*/
void BasicEs::setWrapperObject(RVBasicEvolutionStrategy *obj)
{
	impl->setWrapperObject(obj);
}

/*--------------------------------------------------------------------------*/
BasicEs* BasicEs::create(int mu, int rho, int lambda,RVSelectionMode mode, ObjectiveFunction *objf)
{
	bool invalidPopulation = mu < 0 || lambda < 0;
	bool invalidRecombinationConstant = rho < 0 || rho > mu;
	bool invalidSelectionMode = mode == kRVSelectionModeComma ? (lambda >= mu ? false : true) : false;

	if (invalidPopulation || invalidRecombinationConstant || invalidSelectionMode)
	{
		return 0;
	}
	return new BasicEs(mu, rho, lambda, mode, objf);
}
