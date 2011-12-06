#include "revolution.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>

#define EASY_PROBLEM
#undef  EASY_PROBLEM

/*
#define GEN         1000
#define MU          10 
#define LAMBDA      10
#define SELECTION   kRVSelectionModeComma
#define RHO         1 
*/
#define OBJECTIVES  1

#ifndef EASY_PROBLEM
#define DIM         8
#else
#define DIM	    4
#endif

#define POINTS      DIM 

struct Point
{
	double x, y, z, mass;
};

static Point PARTICLES[POINTS];

static void Particles_Initialize()
{
#ifndef EASY_PROBLEM
	PARTICLES[0].x = 2494.589624;
	PARTICLES[0].y = -563.114551;
	PARTICLES[0].z = 1613.935078;
	PARTICLES[0].mass = 3.226921;
	PARTICLES[1].x = 2516.245887;
	PARTICLES[1].y =  578.829788;
	PARTICLES[1].z = 1620.153061;
	PARTICLES[1].mass = 3.226921;
	PARTICLES[2].x = 3839.006113;
	PARTICLES[2].y =  786.027381;
	PARTICLES[2].z =  662.190941;
	PARTICLES[2].mass = 3.784319;
	PARTICLES[3].x = 3983.270739;
	PARTICLES[3].y =  624.978937;
	PARTICLES[3].z = 1600.816521;
	PARTICLES[3].mass = 2.345191;
	PARTICLES[4].x = 1911.002302;
	PARTICLES[4].y = -778.012074;
	PARTICLES[4].z =  612.784773;
	PARTICLES[4].mass = 7.267111;
	PARTICLES[5].x = 4015.657710;
	PARTICLES[5].y = -628.133786;
	PARTICLES[5].z = 1589.003620;
	PARTICLES[5].mass = 2.355893;
	PARTICLES[6].x = 1819.511896;
	PARTICLES[6].y =  768.800124;
	PARTICLES[6].z =  590.885146;
	PARTICLES[6].mass = 2.837796;
	PARTICLES[7].x = 3798.390368;
	PARTICLES[7].y = -758.717933;
	PARTICLES[7].z =  603.615460;
	PARTICLES[7].mass = 5.731926;
#else
	PARTICLES[0].x = 1.;
	PARTICLES[0].y = 1.;
	PARTICLES[0].z = 0.;
	PARTICLES[0].mass = 1.0;
	PARTICLES[1].x = -1.;
	PARTICLES[1].y = -1.;
	PARTICLES[1].z = 0.;
	PARTICLES[1].mass = 1.0;
	PARTICLES[2].x = -1.;
	PARTICLES[2].y = 1.;
	PARTICLES[2].z = 0.;
	PARTICLES[2].mass = 1.0;
	PARTICLES[3].x = 1.;
	PARTICLES[3].y = -1.;
	PARTICLES[3].z = 0.;
	PARTICLES[3].mass = 1.0;
#endif
}

//static Point INITIAL_COG = { 2696.883209, 6.658567, 782.316552, -1};
//static Point DESIRED_COG = { 2700.000000, 7.000000, 783.900000, -1 };
#ifndef EASY_PROBLEM
static Point DESIRED_COG = { 2696.883209, 6.658567, 782.316552, -1};
static Point INITIAL_COG = { 2691.378, 18.831, 779.921, -1 };
static double INITIAL_MASS= 1286.285;
static double DESIRED_MASS= 1304.114579;
#else
static Point DESIRED_COG = { 1.0/3.0, 1.0/3.0, 0, -1};
static Point INITIAL_COG = { 0, 0  , 0, -1};
static double INITIAL_MASS= 4;
static double DESIRED_MASS= 12;
#endif
//static double INITIAL_MASS= 1304.114579;
//static double DESIRED_MASS= 1334.114579;
static double MASS_TO_ADD = DESIRED_MASS - INITIAL_MASS;
static double MT = INITIAL_MASS;
static double MX = INITIAL_COG.x * MT;
static double MY = INITIAL_COG.y * MT;
static double MZ = INITIAL_COG.z * MT;

#if 0
static double MASS_HI_BOUNDS[DIM] = {
	6.458054, 
	4.865722, 
	7.573578, 
	4.693444, 
	14.543709, 
	4.714861, 
	5.679297, 
	11.471335
};
#endif

#ifndef EASY_PROBLEM
static double MASS_HI_BOUNDS[DIM] = {
	3.838,
	2.891,
	8.643,
	3.375,
	4.501,
	2.789,
	2.802,
	6.817
};
#else
static double MASS_HI_BOUNDS[DIM] = {8,8,8,8};
#endif

static void Calculate_Cog(const double *masses, Point* cog)
{
	double sx = 0, sy = 0, sz = 0, sm = 0;
	for (int i = 0; i < DIM; ++i)
	{
		sx += masses[i] * PARTICLES[i].x;
		sy += masses[i] * PARTICLES[i].y;
		sz += masses[i] * PARTICLES[i].z;
		sm += masses[i];
	}
	
	cog->x = (MX+sx)/(MT+sm);
	cog->y = (MY+sy)/(MT+sm);
	cog->z = (MZ+sz)/(MT+sm);
	cog->mass = sm; // only added mass
}

struct InertiaTensor 
{
	double Ixx, Iyy, Izz, Ixy, Ixz, Iyz;
}; //~ InertiaTensor

static InertiaTensor INITIAL_TENSOR = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
static InertiaTensor DESIRED_TENSOR = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

void Calculate_InertiaTensor(const double *masses, InertiaTensor *tensor)
{
	double ixx = 0, iyy = 0, izz = 0, ixy = 0, ixz = 0, iyz = 0;
	for (int i = 0; i < DIM; ++i)
	{
		ixx += masses[i] * (PARTICLES[i].y * PARTICLES[i].y + PARTICLES[i].z * PARTICLES[i].z);
		iyy += masses[i] * (PARTICLES[i].x * PARTICLES[i].x + PARTICLES[i].z * PARTICLES[i].z);
		izz += masses[i] * (PARTICLES[i].x * PARTICLES[i].x + PARTICLES[i].y * PARTICLES[i].y);
		ixy -= masses[i] * (PARTICLES[i].x * PARTICLES[i].y);
		ixz -= masses[i] * (PARTICLES[i].x * PARTICLES[i].z);
		iyz -= masses[i] * (PARTICLES[i].y * PARTICLES[i].z);
	}	
	tensor->Ixx = INITIAL_TENSOR.Ixx + ixx;
	tensor->Iyy = INITIAL_TENSOR.Iyy + iyy;
	tensor->Izz = INITIAL_TENSOR.Izz + izz;
	tensor->Ixy = INITIAL_TENSOR.Ixy + ixy;
	tensor->Ixz = INITIAL_TENSOR.Ixz + ixz;
	tensor->Iyz = INITIAL_TENSOR.Iyz + iyz;
}

void Calculate_InitialInertiaTensor()
{
#if 0
	INITIAL_TENSOR.Ixx = 4.92810E+008;
	INITIAL_TENSOR.Iyy = 2.31120E+009;
	INITIAL_TENSOR.Izz = 2.57308E+009;
	INITIAL_TENSOR.Ixy =-8.60016E+006;
	INITIAL_TENSOR.Ixz =-4.13845E+007;
	INITIAL_TENSOR.Iyz = 3.23983E+006;

	DESIRED_TENSOR.Ixx = 4.77328E+008;
	DESIRED_TENSOR.Iyy = 2.30072E+009;
	DESIRED_TENSOR.Izz = 2.55006E+009;
	DESIRED_TENSOR.Ixy =-1.47839E+007;
	DESIRED_TENSOR.Ixz =-3.96882E+007;
	DESIRED_TENSOR.Iyz = 549598;
#endif

#ifndef EASY_PROBLEM
	DESIRED_TENSOR.Ixx = 4.92810E+008;
	DESIRED_TENSOR.Iyy = 2.31120E+009;
	DESIRED_TENSOR.Izz = 2.57308E+009;
	DESIRED_TENSOR.Ixy =-8.60016E+006;
	DESIRED_TENSOR.Ixz =-4.13845E+007;
	DESIRED_TENSOR.Iyz = 3.23983E+006;

	INITIAL_TENSOR.Ixx = 4.77328E+008;
	INITIAL_TENSOR.Iyy = 2.30072E+009;
	INITIAL_TENSOR.Izz = 2.55006E+009;
	INITIAL_TENSOR.Ixy =-1.47839E+007;
	INITIAL_TENSOR.Ixz =-3.96882E+007;
	INITIAL_TENSOR.Iyz = 549598;
#else
	DESIRED_TENSOR.Ixx = 12;
	DESIRED_TENSOR.Iyy = 12;
	DESIRED_TENSOR.Izz = 24;
	DESIRED_TENSOR.Ixy = -4;
	DESIRED_TENSOR.Ixz =  0;
	DESIRED_TENSOR.Iyz =  0;

	INITIAL_TENSOR.Ixx = 4;
	INITIAL_TENSOR.Iyy = 4;
	INITIAL_TENSOR.Izz = 8;
	INITIAL_TENSOR.Ixy = 0;
	INITIAL_TENSOR.Ixz = 0;
	INITIAL_TENSOR.Iyz = 0;
#endif
}

double InertiaError(const InertiaTensor& t)
{
	double ixxerr = 0, iyyerr = 0, izzerr = 0, ixyerr = 0, ixzerr = 0, iyzerr = 0;

	ixxerr = (t.Ixx - DESIRED_TENSOR.Ixx)/(DESIRED_TENSOR.Ixx);
	iyyerr = (t.Iyy - DESIRED_TENSOR.Iyy)/(DESIRED_TENSOR.Iyy);
	izzerr = (t.Izz - DESIRED_TENSOR.Izz)/(DESIRED_TENSOR.Izz);
	ixyerr = (t.Ixy - DESIRED_TENSOR.Ixy)/(DESIRED_TENSOR.Ixy);
#ifndef EASY_PROBLEM
	ixzerr = (t.Ixz - DESIRED_TENSOR.Ixz)/(DESIRED_TENSOR.Ixz);
	iyzerr = (t.Iyz - DESIRED_TENSOR.Iyz)/(DESIRED_TENSOR.Iyz);
#endif
	return ixxerr*ixxerr + iyyerr*iyyerr + izzerr*izzerr + ixyerr*ixyerr + ixzerr*ixzerr + iyzerr*iyzerr;
}	

double CogError(const Point& current)
{
	double xerr = 0, yerr = 0, zerr = 0;

	xerr = (current.x-DESIRED_COG.x)/DESIRED_COG.x;
	yerr = (current.y-DESIRED_COG.y)/DESIRED_COG.y;
#ifndef EASY_PROBLEM
	zerr = (current.z-DESIRED_COG.z)/DESIRED_COG.z;
#endif

	return xerr*xerr + yerr*yerr + zerr * zerr;
}

double massViolation(const double *masses)
{
	double sm = 0.0;
	for (int i = 0; i < DIM; ++i)
	{
		sm += masses[i];
	}

	double err = 0.0;
	if (sm < 0.99 * MASS_TO_ADD || sm > 1.01 * MASS_TO_ADD)
	{
		err = 1000 * (fabs(sm - MASS_TO_ADD));
	}
	return err;
}

int evalf(const double *dv, double *obj)
{
	Point new_cog = {0, 0, 0, 0};
	obj[0] = 0;
	Calculate_Cog(dv, &new_cog);
	obj[0] = 100 * CogError(new_cog);

	InertiaTensor it = {0, 0, 0, 0, 0, 0};
	Calculate_InertiaTensor(dv, &it);
	obj[0] = obj[0]+InertiaError(it);
	obj[0] += massViolation(dv);
	return 0;
}

void printBest(RVBasicEvolutionStrategy *es, int gen, void *data)
{
#if 1
	double added = 0.0;
	double masses[DIM];
	printf("%d\t", gen);
	for (int i = 0; i < DIM; ++i)
	{
		printf("%f, ", RVBasicEvolutionStrategyGetDesignParameter(es, 0, i));
		masses[i] = RVBasicEvolutionStrategyGetDesignParameter(es, 0, i);
		added += masses[i];
	}
	Point cog = {0, 0, 0, 0};
	Calculate_Cog(masses, &cog);

	InertiaTensor it = {0, 0, 0, 0, 0, 0};
	Calculate_InertiaTensor(masses, &it);
#if 0
	printf(" (X = %f, Y = %f, Z = %f, Added %f mass, I #[ %g %g %g | %g %g %g ] {%f}", 
			cog.x, cog.y, cog.z, added, 
			it.Ixx, it.Iyy, it.Izz, it.Ixy, it.Ixz, it.Iyz, 
			RVBasicEvolutionStrategyGetObjective(es, 0, 0));
#endif
	printf("%f\t%f\t%f\n", 
		fabs(added-MASS_TO_ADD)/MASS_TO_ADD * 100.0, massViolation(masses), RVBasicEvolutionStrategyGetObjective(es, 0, 0));
#endif
}

void init(double *params, double *objectives, void *data)
{
	for (int i = 0; i < DIM; ++i)
	{
		params[i] = MASS_TO_ADD / DIM;
	}
}

unsigned int GEN = 0;

int shouldTerminate(RVBasicEvolutionStrategy *es, unsigned int g, void *data)
{
	if (g == GEN) 
	{
		printf("Solution:\n");
		printf("------------------------------------------------\n");
		double masses[DIM] = {0}, added = 0;
		for (int i = 0; i < DIM; ++i)
		{
			masses[i] = RVBasicEvolutionStrategyGetDesignParameter(es, 0, i);
			printf("%f\t", masses[i]);
			added += masses[i];
		}
		printf("\nMass error = %.2f%%\n", (MASS_TO_ADD-added)/added * 100);
	
		Point c = {0, 0, 0, 0};
		Calculate_Cog(masses, &c);	
		printf("Target errors:\n");
		printf("XG = %f\t\te = %f%%\n", c.x, (c.x-DESIRED_COG.x)/(DESIRED_COG.x)*100);
		printf("YG = %f\t\te = %f%%\n", c.y, (c.y-DESIRED_COG.y)/(DESIRED_COG.y)*100);
#ifndef EASY_PROBLEM
		printf("ZG = %f\t\te = %f%%\n", c.z, (c.z-DESIRED_COG.z)/(DESIRED_COG.z)*100);
#endif
		InertiaTensor t = {0, 0, 0, 0, 0, 0};
		Calculate_InertiaTensor(masses, &t);
		printf("Ixx= %f\t\te = %f%%\n", t.Ixx, (t.Ixx-DESIRED_TENSOR.Ixx)/DESIRED_TENSOR.Ixx * 100);
		printf("Iyy= %f\t\te = %f%%\n", t.Iyy, (t.Iyy-DESIRED_TENSOR.Iyy)/DESIRED_TENSOR.Iyy * 100);
		printf("Izz= %f\t\te = %f%%\n", t.Izz, (t.Izz-DESIRED_TENSOR.Izz)/DESIRED_TENSOR.Izz * 100);
		printf("Ixy= %f\t\te = %f%%\n", t.Ixy, fabs((t.Ixy-DESIRED_TENSOR.Ixy)/DESIRED_TENSOR.Ixy) * 100);
#ifndef EASY_PROBLEM
		printf("Ixz= %f\t\te = %f%%\n", t.Ixz, fabs((t.Ixz-DESIRED_TENSOR.Ixz)/DESIRED_TENSOR.Ixz) * 100);
		printf("Iyz= %f\t\te = %f%%\n", t.Iyz, fabs((t.Iyz-DESIRED_TENSOR.Iyz)/DESIRED_TENSOR.Iyz) * 100);
#endif
		return 1;
	}
	return 0;
}

void constrainMasses(double *dv, void *data)
{
	for (int i = 0; i < DIM; ++i)
	{
		if (dv[i] < 0.0) dv[i] = 0.0;
		if (dv[i] > MASS_HI_BOUNDS[i]) dv[i] = MASS_HI_BOUNDS[i];
	}
}

int main(int argc, char *argv[])
{

	if (argc != 6) 
	{
		printf("usage: main mu lambda gen sel\n");
		return 0;
	}

	int MU = atoi(argv[1]);
	int RHO= atoi(argv[2]);
	int LAMBDA = atoi(argv[3]);
	GEN = atoi(argv[4]);
	int sel = atoi(argv[5]);
	RVSelectionMode SELECTION = sel ? kRVSelectionModeComma : kRVSelectionModePlus;

	Particles_Initialize();
	Calculate_InitialInertiaTensor();

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
