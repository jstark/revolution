#include "revolution.h"
#include <cstdio>
#include <cmath>
#include <sstream>

#define kDimensionality 2
#define kObjectives 1
#define kNoUserData 0

int kPopulation = 10;
double kFp = 0.2;
double kCR = 0.9;
double kMaxGenerations = 100;

static int ackley2d(const struct RVArray *dvec, struct RVArray *ovec, void *data)
{
	using std::exp;
	double x = RVArrayGetElementAtIndex(dvec, 0);
	double y = RVArrayGetElementAtIndex(dvec, 1);
	double f = 20.0 + exp(1.0) - 
		20.0*exp(-0.5 * (x*x + y*y)) - 
		exp(0.5 * (cos(2 * 3.14159 * x) + cos(2 * 3.14159 * y)));
	if (x < -15 || y < -15 || x > 30 || y > 30) f = 1000;
	RVArraySetElementAtIndex(ovec, 0, f);
	return 0;
}

static void init(struct RVArray *dvec, struct RVArray *ovec, void *data)
{
	RVArraySetElementAtIndex(dvec, 0, -15.0 + 45.0 * (rand() / (1.0 * RAND_MAX)));
	RVArraySetElementAtIndex(dvec, 1, -15.0 + 45.0 * (rand() / (1.0 * RAND_MAX)));
}

static void on_generation(struct RVDifferentialEvolution *de, unsigned int gen, void *d)
{
	printf("%d\t%f\t%f\t%f\n",
		gen,
		RVDifferentialEvolutionGetDesignParameter(de, 0, 0),
		RVDifferentialEvolutionGetDesignParameter(de, 0, 1),
		RVDifferentialEvolutionGetObjective(de, 0, 0));
}

static int should_terminate(struct RVDifferentialEvolution *de, unsigned int gen, void *d)
{
	return gen == kMaxGenerations;
}

int main(int argc, char *argv[])
{
	// parse arguments: "kPopulation kFp kCR kMaxGen"
	// e.g ackley2d_main.exe "10 0.2 0.9 100"
	std::istringstream s(argv[1]);  // could crash
	s >> kPopulation >> kFp >> kCR >> kMaxGenerations;

	struct RVObjectiveFunction *objective = 
		RVObjectiveFunctionCreate(kDimensionality, kObjectives, ackley2d, kNoUserData);

	struct RVDifferentialEvolution *de = 
		RVDifferentialEvolutionCreate(kPopulation, kFp, kCR, objective);
	RVDifferentialEvolutionInitializePopulation(de, init, kNoUserData);
	RVDifferentialEvolutionSetOnGenerationFinishedFun(de, on_generation, kNoUserData);
	RVDifferentialEvolutionSetTerminationFun(de, should_terminate, kNoUserData);
	RVDifferentialEvolutionStart(de);
	RVDifferentialEvolutionDestroy(de);
	RVObjectiveFunctionDestroy(objective);
	::getchar();
	return 0;
}
