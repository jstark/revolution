#include "revolution.h"
#include <cstdio>
#include <cmath>

#define MU          100 
#define RHO         1 
#define LAMBDA      100
#define OBJECTIVES  1
#define DIM         9
#define SELECTION   kRVSelectionModeComma
#define GEN         100

#define POINTS      DIM 

struct Point
{
    double x, y, mass;
};

static Point PARTICLES[POINTS];

static void Particles_Initialize()
{
    PARTICLES[0].x = -2.0;
    PARTICLES[0].y = -2.0;
    PARTICLES[0].mass = 1;
    PARTICLES[1].x = -1.0;
    PARTICLES[1].y = -1.0;
    PARTICLES[1].mass = 1;
    PARTICLES[2].x =  0.0;
    PARTICLES[2].y =  0.0;
    PARTICLES[2].mass = 1;
    PARTICLES[3].x =  1.0;
    PARTICLES[3].y =  1.0;
    PARTICLES[3].mass = 1;
    PARTICLES[4].x =  2.0;
    PARTICLES[4].y =  2.0;
    PARTICLES[4].mass = 1;
    PARTICLES[5].x = -2.0;
    PARTICLES[5].y =  2.0;
    PARTICLES[5].mass = 1;
    PARTICLES[6].x = -1.0;
    PARTICLES[6].y =  1.0;
    PARTICLES[6].mass = 1;
    PARTICLES[7].x =  1.0;
    PARTICLES[7].y = -1.0;
    PARTICLES[7].mass = 1;
    PARTICLES[8].x =  2.0;
    PARTICLES[8].y = -2.0;
    PARTICLES[8].mass = 1;
}

static Point INITIAL_COG;

static void Initialize_Cog()
{
    double Mtotal = 0.0;
    double M_times_X = 0.0;
    double M_times_Y = 0.0;
    for (int i = 0; i < DIM; ++i)
    {
        Mtotal += PARTICLES[i].mass;
        M_times_X += PARTICLES[i].x * PARTICLES[i].mass;
        M_times_Y += PARTICLES[i].y * PARTICLES[i].mass;
    }

    INITIAL_COG.x = M_times_X / Mtotal;
    INITIAL_COG.y = M_times_Y / Mtotal;
    INITIAL_COG.mass = Mtotal;
}

static Point Calculate_Cog(const double *masses)
{
    Point cog = {0.0, 0.0, 0.0};

    double Mtotal = 0.0;
    double M_times_X = 0.0;
    double M_times_Y = 0.0;
    for (int i = 0; i < DIM; ++i)
    {
        Mtotal += masses[i] + PARTICLES[i].mass;
        M_times_X += PARTICLES[i].x * (masses[i] + PARTICLES[i].mass);
        M_times_Y += PARTICLES[i].y * (masses[i] + PARTICLES[i].mass);
    }

    cog.x = M_times_X / Mtotal;
    cog.y = M_times_Y / Mtotal;
    cog.mass = Mtotal;
    return cog;
}

static Point NEW_COG = { 0.5, 0.5, 20.0 };
static double MASS_TO_ADD = 0;

void Calculate_Mass_to_Add()
{
    MASS_TO_ADD = NEW_COG.mass - 9.0;
}

double CogError(const Point& current)
{
    return (current.x-NEW_COG.x)*(current.x-NEW_COG.x)+(current.y-NEW_COG.y)*(current.y-NEW_COG.y);
}

double PENALTY = 1000;

int evalf(const double *dv, double *obj)
{
    Point current_cog = Calculate_Cog(dv);
    double actualDistance = 0.5*CogError(current_cog);
    if (current_cog.mass > 1.01 * NEW_COG.mass) actualDistance += PENALTY + current_cog.mass * current_cog.mass;
    if (current_cog.mass < 0.99 * NEW_COG.mass) actualDistance += PENALTY + current_cog.mass * current_cog.mass;
    obj[0] = actualDistance;
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
#if 1
    double added = 0.0;
    printf("%d\t", gen);
    for (int i = 0; i < DIM; ++i)
    {
        printf("%f ", RVBasicEvolutionStrategyGetDesignParameter(es, 0, i));
        added += RVBasicEvolutionStrategyGetDesignParameter(es, 0, i);
    }
    printf(" (%f)\t%f", added, RVBasicEvolutionStrategyGetObjective(es, 0, 0));
    printf("\n");
#endif
}

void init(double *params, double *objectives, void *data)
{
    for (int i = 0; i < DIM; ++i)
    {
        params[i] = MASS_TO_ADD / DIM;
    }
}

int shouldTerminate(RVBasicEvolutionStrategy *es, unsigned int g, void *data)
{
    if (g == GEN) return 1;
    return 0;
}

void constrainMasses(double *dv, void *data)
{
    for (int i = 0; i < DIM; ++i)
    {
        if (dv[i] < 0.0) dv[i] = 0.0;
        if (dv[i] > 100.0) dv[i] = 100.0;
    }
}

int main(int argc, char *argv[])
{
    Particles_Initialize();
    Initialize_Cog();
    Calculate_Mass_to_Add();

	RVObjectiveFunction *object = RVObjectiveFunctionCreate(DIM, OBJECTIVES, evalf);
	RVBasicEvolutionStrategy *es = RVBasicEvolutionStrategyCreate(MU, RHO, LAMBDA, SELECTION, object);
	RVBasicEvolutionStrategyInitializePopulation(es, init, 0);
    RVBasicEvolutionStrategySetParameterConstraints(es, constrainMasses, 0);
	RVBasicEvolutionStrategyOnGenerationFinished(es, printBest, 0);
    RVBasicEvolutionStrategySetTerminationCriteria(es, shouldTerminate, 0);
	RVBasicEvolutionStrategyStart(es);
	RVBasicEvolutionStrategyDestroy(es);
	RVObjectiveFunctionDestroy(object);

	return 0;
}
