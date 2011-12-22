#include "basic_es.h"
#include "objective_function.h"
#include "population.h"
#include "atom.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
#include <functional>
#include <ctime>

using revolution::BasicEs;
using revolution::ObjectiveFunction;

/*--------------------------------------------------------------------------*/
namespace 
{

const double PI = 3.14159265;

double normal_dist_num(void *data)
{
	static bool cache = false;
	static double cached = 0;
    static bool seed = true;

    if (seed)
    {
        seed = false;
        srand(time(NULL));
    }   

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
    using revolution::Atom::constrain;

	double strategyParam(int index) const 
	{
		return strParams.at(index);
	}

	void setStrategyParam(int index, double value)
	{
		strParams[index] = value;
	}

	void swap(Atom& rhs)
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
void gen_random_indices(unsigned int max, std::vector<int>& indices)
{
    for (std::vector<int>::size_type sz = 0; sz != max; ++sz)
    {
        indices[sz] = rand() % max;
    }
}

/*--------------------------------------------------------------------------*/
class Recombination 
{
public:
	Recombination(int r, int dim) : rho(r), n(dim) {}

	void apply(const std::vector<Atom *>& p, Atom& temp) const// intermediate recombination (for the moment)
	{
		std::vector<int> random_indices(rho, 0);
		gen_random_indices(rho, random_indices);
		for (int varIndex = 0; varIndex != n; ++varIndex)
		{
			double paramSum = 0.0;
			double strategySum = 0.0;
			Atom *atom = 0;
			for (std::vector<int>::size_type i = 0; i != random_indices.size(); ++i)
			{
				atom = p[i];
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
	Mutation()
	{
		setRNG(0, 0);
	}

	void setRNG(RVRandom f, void *d)
	{
		if (f)
		{
			rng = f;
			data = d;
		} else
		{
			rng = normal_dist_num;
			data = 0;
		}
	}

	void apply(Atom& temp) const
	{
		static const int n = temp.dim();
		static const double t0= 1.0/(2.0 * sqrt(1.0*n));
		static const double t = 1.0/(sqrt(2.0 * sqrt(1.0*n)));

		double m0 = exp(t0 * rng(data));
		double mi = 0;
		for (int varIndex = 0; varIndex != n; ++varIndex)
		{
			mi = temp.strategyParam(varIndex);
			mi *= exp(t*rng(data));
			mi *= m0;
			temp.setStrategyParam(varIndex, mi);
			temp[varIndex] += mi * rng(data);
		}
	}

	RVRandom rng;
	void *data;

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

	void apply(std::vector<Atom *>& pop, unsigned int mu, unsigned int lambda) const
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
	BasicEsPriv(unsigned int m, unsigned int r, unsigned int l, RVSelectionMode mode, ObjectiveFunction *objf)
		: 
		  ::Population(m, l, objf->dim(), objf->objectives()), 
		  temp(objf->dim(), objf->objectives()),
		  recombination(r, objf->dim()),
		  selection(mode), 
		  objectiveFunction(objf), 
		  mu(m), lambda(l),
		  onGenFinished(0),
		  onGenFinishedData(0),
          evolutionShouldTerminate(0),
          evolutionShouldTerminateData(0),
		  wrapperObj(0),
          varConstrain(0), 
          varConstrainData(0)
	{
		set_ref();
	}

	void setPopulationInitialValues(RVPopulationSetInitialValues fun, void *data)
	{
		initialize(fun, data);
        for (std::vector< ::Atom *>::size_type sz = 0; sz != mu; ++sz)
        {
            atom_ref[sz]->constrain(varConstrain, varConstrainData);
            atom_ref[sz]->eval(*objectiveFunction);
        }
	}

	void setOnGenerationFinished(RVGenerationFinished fun, void *data)
	{
		onGenFinished = fun;
		onGenFinishedData = data;
	}

	void setEvolutionShouldTerminate(RVEvolutionShouldTerminate fun, void *data)
	{
		evolutionShouldTerminate = fun;
		evolutionShouldTerminateData = data;
	}

	void start()
	{
        static const unsigned int MAX_GEN = 1000;

        unsigned int g = 0;
        while (true)
		{
			doEvolutionStep();
            ++g;

			if (onGenFinished) 
            {
                onGenFinished(wrapperObj, g, onGenFinishedData);
            }

            if (evolutionShouldTerminate) 
            {
                int terminate = evolutionShouldTerminate(wrapperObj, g, evolutionShouldTerminateData);
                if (terminate)
                {
                    break;
                }
            } else if (g == MAX_GEN)
            {
                break;
            }
		}
	}

	double getDesignParameter(int parent, int paramIndex) const
	{
		return atom_ref[parent]->operator[](paramIndex);
	}

	void doEvolutionStep()
	{
		for (unsigned int i = 0; i < lambda; ++i)
		{
			recombination.apply(atom_ref, temp);			
			mutation.apply(temp);
            temp.constrain(varConstrain, varConstrainData);
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

	void setRNG(RVRandom fun, void *data)
	{
		mutation.setRNG(fun, data);
	}

	void setParamConstraints(RVConstrainParam fun, void *data)
	{
        varConstrain = fun;
        varConstrainData = data;
	}

	void set_ref()
	{
		atom_ref = mem_ref();
	}

	::Atom temp;
	std::vector< ::Atom *> atom_ref;
	const ::Recombination recombination;
	const ::Selection selection;
	Mutation mutation;
	ObjectiveFunction *objectiveFunction;
	const unsigned int mu;
	const unsigned int lambda;
	RVGenerationFinished onGenFinished;
	void *onGenFinishedData;
	RVEvolutionShouldTerminate evolutionShouldTerminate;
	void *evolutionShouldTerminateData;
	RVBasicEvolutionStrategy *wrapperObj;
    RVConstrainParam varConstrain;
    void *varConstrainData;
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
void BasicEs::setTerminationCriteria(RVEvolutionShouldTerminate fun, void *data)
{
	impl->setEvolutionShouldTerminate(fun, data);
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
void BasicEs::setRNG(RVRandom fun, void *data)
{
	impl->setRNG(fun, data);
}

/*--------------------------------------------------------------------------*/
void BasicEs::setParamConstraints(RVConstrainParam fun, void *data)
{
    impl->setParamConstraints(fun, data);
}

/*--------------------------------------------------------------------------*/
static bool isValidMode(enum RVSelectionMode mode)
{
    return (mode != kRVSelectionModeComma) && (mode != kRVSelectionModePlus);
}

/*--------------------------------------------------------------------------*/
BasicEs* BasicEs::create(int mu, int rho, int lambda, enum RVSelectionMode mode, ObjectiveFunction *objf)
{
	bool invalidPopulation = (mu <= 0) || (lambda <= 0);
	bool invalidRecombinationConstant = (rho <= 0) || rho > mu;
	bool invalidSelectionMode = isValidMode(mode);

	if (invalidPopulation || invalidRecombinationConstant || invalidSelectionMode)
	{
		return 0;
	}
	return new BasicEs(mu, rho, lambda, mode, objf);
}
