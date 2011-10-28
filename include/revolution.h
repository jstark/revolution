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

struct RVEvolutionStrategy;

/*! \var RVEvolutionStrategy
 *  \brief The library's main object type.
 */
typedef struct RVEvolutionStrategy RVEvolutionStrategy;

struct RVObjectiveFunction;

/*! \var RVObjectiveFunction
 *  \brief The object that defines an optimization problem.
 */
typedef struct RVObjectiveFunction RVObjectiveFunction;

/*! \var RVObjectiveEvalFun
 *  \brief A pointer to a function that evaluates a design vector.
 */
typedef int (*RVObjectiveEvalFun)(const double *designVector, double *objectivesEvaluation);

/*! \fn RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvalFun fun);
 *  \brief Creates an object that defines an optimization problem.
 */
DLL_PUBLIC struct RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvalFun fun);

/*! \fn void RVObjectiveFunctionDestroy(RVObjectiveFunction *object);
 *  \brief Destroys an RVObjectiveFunction object.
 */
DLL_PUBLIC void RVObjectiveFunctionDestroy(struct RVObjectiveFunction *object);

/*! \fn RVEvolutionStrategy* RVEvolutionStrategyCreate(RVObjectiveFunction *fun);
 *  \brief Creates an object that represents an evolution strategy.
 */
DLL_PUBLIC RVEvolutionStrategy* RVEvolutionStrategyCreate(RVObjectiveFunction *fun);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_H_INCLUDED
