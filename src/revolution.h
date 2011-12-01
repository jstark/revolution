/*! \file revolution.h
 *  \brief The library's header file
 */
#ifndef REVOLUTION_H_INCLUDED
#define REVOLUTION_H_INCLUDED

/* From API C++ */
#if defined _WIN32 || defined __CYGWIN__
	#ifdef revolution_EXPORTS // define this when generating DLL
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllexport))
		#else
			#define DLL_PUBLIC __declspec(dllexport)
		#endif 
	#else
		#ifdef __GNUC__
			#define DLL_PUBLIC __attribute__((dllimport))
		#else
			#define DLL_PUBLIC __declspec(dllimport)
		#endif
	#endif
	#define DLL_HIDDEN 
#else
	#if __GNUC__ >= 4
		#define DLL_PUBLIC __attribute__ ((visibility("default"))) 
		#define DLL_HIDDEN __attribute__ ((visibility("hidden")))
	#else
		#define DLL_PUBLIC 
		#define DLL_HIDDEN
	#endif
#endif

#define RV_API_MAJOR	0
#define RV_API_MINOR	0
#define RV_API_PATCH	0

#ifdef __cplusplus
extern "C" {
#endif
		
DLL_PUBLIC int RVGetMajorVersion(void);
DLL_PUBLIC int RVGetMinorVersion(void);
DLL_PUBLIC int RVGetPatchVersion(void);

struct RVObjectiveFunction;

/*! \var RVObjectiveFunction
 *  \brief The type that represents an optimization problem.
 */
typedef struct RVObjectiveFunction RVObjectiveFunction;

/*! \var RVObjectiveEvalFun
 *  \brief A function that evaluates a design vector.
 */
typedef int (*RVObjectiveEvalFun)(const double *designVector, double *objectivesEvaluation);

/*! \fn RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvalFun fun);
 *  \brief Creates an object that defines an optimization problem.
 */
DLL_PUBLIC RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvalFun fun);

/*! \fn void RVObjectiveFunctionDestroy(RVObjectiveFunction *object);
 *  \brief Destroys an RVObjectiveFunction object.
 */
DLL_PUBLIC void RVObjectiveFunctionDestroy(RVObjectiveFunction *object);

/*! \var RVBasicEvolutionStrategy
 *  \brief The type that represents a basic evolution strategy.
 */
typedef struct RVBasicEvolutionStrategy RVBasicEvolutionStrategy;

/*! \var RVSelectionMode
 *	\brief The selection operator's selection mode.
 */
typedef enum RVSelectionMode
{
	kRVSelectionModeInvalid,
	kRVSelectionModePlus,
	kRVSelectionModeComma
} RVSelectionMode;

/*! \fn RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, RVObjectiveFunction *fun);
 *  \brief Creates an object that represents an evolution strategy.
 */
DLL_PUBLIC RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, RVSelectionMode mode, RVObjectiveFunction *fun);

/*! \fn RVPopulationSetInitialValues
 *  \brief A function that calculates the initial population values. 
 */
typedef void (*RVPopulationSetInitialValues)(double *params, double *objectives, void *data);

/* \fn void RVBasicEvolutionStrategyPopulationSetInitialValues(RVBasicEvolutionStrategy *es, RVSetPopulationInitialValues fun);
 * \brief Sets a function that will be called to calculate the initial values of the population. 
 */
DLL_PUBLIC void RVBasicEvolutionStrategyInitializePopulation(RVBasicEvolutionStrategy *es, RVPopulationSetInitialValues fun, void *data);

typedef void (*RVConstrainParam)(double *designParam, void *data);

DLL_PUBLIC void RVBasicEvolutionStrategySetParameterConstraints(RVBasicEvolutionStrategy *es, RVConstrainParam fun, void *data);

/* \fn void RVBasicEvolutionStrategyStart(RVBasicEvolutionStrategy *es);
 * \brief Starts an evolution strategy
 */
DLL_PUBLIC void RVBasicEvolutionStrategyStart(RVBasicEvolutionStrategy *es);

typedef void (*RVGenerationFinished)(RVBasicEvolutionStrategy *es, int gen, void *data);

/* \fn void RVBasicEvolutionOnGenerationFinished(RVBasicEvolutionStrategy *es, RVGenerationFinished fun, void *data);
 * \brief Starts an evolution strategy
 */
DLL_PUBLIC void RVBasicEvolutionStrategyOnGenerationFinished(RVBasicEvolutionStrategy *es, RVGenerationFinished fun, void *data);

typedef int (*RVEvolutionShouldTerminate)(RVBasicEvolutionStrategy *es, unsigned int g, void *data);

/* \fn void RVBasicEvolutionStrategySetTerminationCriteria(RVBasicEvolutionStrategy *es, RVEvolutionShouldTerminate fun, void *data);
 * \brief Checks if an evolution should terminate.
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetTerminationCriteria(RVBasicEvolutionStrategy *es, RVEvolutionShouldTerminate fun, void *data);

DLL_PUBLIC double RVBasicEvolutionStrategyGetDesignParameter(RVBasicEvolutionStrategy *es, int parent, int paramIndex);
DLL_PUBLIC double RVBasicEvolutionStrategyGetObjective(RVBasicEvolutionStrategy *es, int parent, int objIndex);

typedef double (*RVRandom)(void *data);

DLL_PUBLIC void RVBasicEvolutionStrategySetRNG(RVBasicEvolutionStrategy *es, RVRandom fun, void *data);

/*! \fn void RVBasicEvolutionStrategyDestroy(RVBasicEvolutionStrategy *es);
 *  \brief Destroys an RVBasicEvolutionStrategy objects and frees memory.
 */
DLL_PUBLIC void RVBasicEvolutionStrategyDestroy(RVBasicEvolutionStrategy *es);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_H_INCLUDED
