/*! \file revolution.h
 *  \brief The library's header file
 */
#ifndef REVOLUTION_H_INCLUDED
#define REVOLUTION_H_INCLUDED

#include <stdlib.h>

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

/*! \def RV_API_MAJOR
 *  \brief The library's major version.
 */
#define RV_API_MAJOR	1
/*! \def RV_API_MINOR
 *  \brief The library's major version.
 */
#define RV_API_MINOR	0
/*! \def RV_API_PATCH
 *  \brief The library's major version.
 */
#define RV_API_PATCH	1

#ifdef __cplusplus
extern "C" {
#endif
	
/*! \fn void RVGetMajorVersion(void);
 *  \brief Gets the library's major version.
 *  \return returns the \ref RV_API_MAJOR value.
 *  @see RVGetMinorVersion(), RVGetPatchVersion()
 */
DLL_PUBLIC int RVGetMajorVersion(void);
/*! \fn void RVGetMinorVersion(void);
 *  \brief Gets the library's minor version.
 *  \return Returns the RV_API_MINOR value
 *  @see RVGetMajorVersion(), RVGetPatchVersion()
 */
DLL_PUBLIC int RVGetMinorVersion(void);
/*! \fn void RVGetPatchVersion(void);
 *  \brief Gets the library's patch version.
 *  \return Returns the RV_API_PATCH value.
 *  @see RVGetMajorVersion(), RVGetMinorVersion()
 */
DLL_PUBLIC int RVGetPatchVersion(void);

/*! \struct RVArray;
 *  \brief Represents a numeric vector.
 *  You cannot create or destroy a RVArray object, but there are many functions that accept
 *  one as argument. 
 *  @see RVArrayGetSize() RVArrayGetElementAtIndex(), RVArraySetElementAtIndex().
 */
struct RVArray;

/*! \fn size_t RVArrayGetSize(struct RVArray const *array);
 *  \brief Find the size of an array.
 *  \param array an RVArray object.
 *  @see RVArrayGetElementAtIndex(), RVArraySetElementAtIndex()
 *  \returns the size of the array, or -1 if array was NULL.
 */
DLL_PUBLIC size_t RVArrayGetSize(const struct RVArray *array);

/*! \fn double RVArrayGetElementAtIndex(struct RVArray const *array, size_t index);
 *  \brief Get the value at a specified index
 *  This function queries an array's elements. Be careful, however, for if you supply 
 *  an index that is out of bounds (greater than the size of the array), the result is 
 *  undefined. There is no bound checking.
 *  \param array an RVArray object
 *  \param index the index of the element
 *  \returns the element at the specified index
 *  @see RVArraySetElementAtIndex(), RVArrayGetSize()
 */
DLL_PUBLIC double RVArrayGetElementAtIndex(const struct RVArray *array, size_t index);

/*! \fn void RVArraySetElementAtIndex(struct RVArray *array, size_t index, double value);
 *  \brief Sets a value at the specified index.
 *  This function changes the value of an array at the specified index. Have in mind, that
 *  there is no bound checking. 
 *  \param array an RVArray object.
 *  \param index the element's index
 *  \param value the new value at the specified index
 *  @see RVArrayGetElementAtIndex(), RVArrayGetSize()
 */
DLL_PUBLIC void RVArraySetElementAtIndex(struct RVArray *array, size_t index, double val);

/*! \struct RVObjectiveFunction;
 *  \brief Represents an objective function to be optimized.
 *  An optimization problem's objective function is modeled by an object of 
 *  type RVObjectiveFunction. 
 *  @see RVObjectiveFunctionCreate()
 */
struct RVObjectiveFunction;

/*! \typedef int (*RVObjectiveEvaluationFun(struct RVArray *const dv, struct RVArray *obj, void *data);
 *  \brief A function signature that evaluates a design vector.
 *  This function is called in every generation to evaluate each population atom.
 *  \param dv a pointer to an RVArray of the design parameters.
 *  \param obj a pointer to an RVArray of the objectives, to be calculated
 *  \param data the user-supplied data, if any, used for the creation of 
 *  objective function object.
 *  @see RVObjectiveFunctionCreate()
 */
typedef int (*RVObjectiveEvaluationFun)(const struct RVArray* dv, struct RVArray *obj, void *data);

/*! \fn RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvaluationFun fun, void *data);
 *  \brief Creates an object that defines an optimization problem.
 *  All objects created by this function should be destroyed by RVObjectiveFunctionDestroy(). 
 *  \param dim the dimensionality of the problem, which must be greater or equal to 1.
 *  \param objectives the number of objectives, which must be greater or equal to 1.
 *  \param fun a function that can evaluate a design parameter, which should not be NULL.
 *  \param data the user-supplied data, if any, to be used for the evaluation.
 *  \return An object that represents the optimization problem. This object will be used to 
 *  initialize a RVBasicEvolutionStrategy. The function returns NULL if any of the parameters were invalid. 
 *  @see RVBasicEvolutionStrategyCreate(), RVObjectiveFunctionDestroy()
 */
DLL_PUBLIC struct RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvaluationFun fun, void *data);

/*! \fn void RVObjectiveFunctionDestroy(RVObjectiveFunction *object);
 *  \brief Destroys an RVObjectiveFunction object.
 *  \param obf a pointer to an object created by RVObjectiveFunctionCreate(). The argument 
 *  can be NULL, in which case the function does nothing.
 *  @see RVObjectiveFunctionCreate()
 */
DLL_PUBLIC void RVObjectiveFunctionDestroy(struct RVObjectiveFunction *obf);

/*! \struct RVBasicEvolutionStrategy
 *  \brief The type that represents a basic evolution strategy.
 *  @see RVBasicEvolutionStrategyCreate(), RVBasicEvolutionStrategyDestroy()
 */
struct RVBasicEvolutionStrategy;

/*! \enum RVSelectionMode
 *	\brief The selection operator's selection mode.
 *	The selection mode is used by a RVBasicEvolutionStrategy to select which
 *	atoms will be the parents of the next generation.
 *	@see RVBasicEvolutionStrategyCreate
 */
enum RVSelectionMode
{
	kRVSelectionModeInvalid, /*!< An invalid selection mode */
	kRVSelectionModePlus,    /*!< A plus (+) selection mode */
	kRVSelectionModeComma    /*!< A comma (,) selection mode */
};

/*! \fn struct RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, enum RVSelectionMode mode, struct RVObjectiveFunction *fun);
 *  \brief Creates an object that represents a basic evolution strategy.
 *  Use this function to create a (mu/rho+lambda) or (mu/rho,lambda) evolution strategy. 
 *  \param mu the number of parents. It must be greater than or equal to 1.
 *  \param rho the recombination constant. It must be greater than or equal to 1. 
 *  \param lambda the number of offsprings. It must be greater than or equal to 1. If mode is 
 *  kRVSelectionModeComma, then lambda must also be greater than mu.
 *  \param mode the selection mode.
 *  \param fun the objective function to be minimized. It cannot be NULL.
 *  \return A pointer to an object that represents a basic evolution strategy, or NULL, if any of the arguments
 *  were invalid. 
 *  @see RVBasicEvolutionStrategyDestroy()
 */
DLL_PUBLIC struct RVBasicEvolutionStrategy* RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, enum RVSelectionMode mode, struct RVObjectiveFunction *fun);

/*! \typedef RVPopulationSetInitialValues(struct RVArray *designParams, struct RVArray *objectives, void *data);
 *  \brief A function signature that calculates the initial population values. 
 *  \param designParams the design vector of a candidate solution.
 *  \param objectives a pointer to the objectives vector. You do not need change the objective values. The Objective function will be called automatically after the initialization function.
 *  \param data the user data that may or may not be supplied. These are the same user data used 
 *  during the call of RVBasicEvolutionStrategyInitializePopulation. 
 *  @see RVBasicEvolutionStrategyInitializePopulation(), RVObjectiveFunctionCreate()
 */
typedef void (*RVPopulationSetInitialValues)(struct RVArray *designParams, struct RVArray *objectives, void *data);

/* \fn void RVBasicEvolutionStrategyPopulationSetInitialValues(RVBasicEvolutionStrategy *es, RVSetPopulationInitialValues fun);
 * \brief Sets a function that will be called to calculate the initial values of the population. 
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value and do nothing
 * if es is actually NULL.
 * \param fun the function that will be called to initialize every atom of the population. The function will ignore * NULL values for fun.
 * \param data the user data that may or may not be supplied by the client. 
 * @see RVPopulationSetInitialValues
 */
DLL_PUBLIC void RVBasicEvolutionStrategyInitializePopulation(struct RVBasicEvolutionStrategy *es, RVPopulationSetInitialValues fun, void *data);

/*! \typedef void (*RVConstrainParam)(struct RVArray *dv, void *data);
 *  \brief A function type definition to constrain design params. 
 *  @see RVBasicEvolutionStrategySetParameterConstraints()
 */
typedef void (*RVConstrainParam)(struct RVArray *designParam, void *data);

/*! \fn void RVBasicEvolutionStrategySetParameterConstraints(struct RVBasicEvolutionStrategy *es, RVConstrainParam fun, void *data);
 *  \brief Sets a function to be called before an atom's objectives are evaluated. 
 *  Use this function with caution. Only the design variables can be constrained, and you should
 *  have in mind that the endogenous strategy parameters assume that no constraining will happen. You can avoid 
 *  using this function by an appropriate objective function scheme.
 *  \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 *  \param fun the function that will constrain the design params of an atom. It will be checked for NULL.
 *  \param data the user-supplied data, if any.
 *  @see RVConstrainParam()
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetParameterConstraints(struct RVBasicEvolutionStrategy *es, RVConstrainParam fun, void *data);

/* \fn void RVBasicEvolutionStrategyStart(struct RVBasicEvolutionStrategy *es);
 * \brief Starts an evolution strategy
 * A started evolution strategy will execute for a maximum of 1000 generations, unless some termination
 * criteria has been set by RVBasicEvolutionStrategySetTerminationCriteria.
 * \param es a pointer to a basic evolution strategy object. The function will check for a NULL argument.
 * @see RVBasicEvolutionOnGenerationFinished(), RVBasicEvolutionStrategySetTerminationCriteria()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyStart(struct RVBasicEvolutionStrategy *es);

/*! \typedef void (*RVGenerationFinished)(RVBasicEvolutionStrategy *es, int gen, void *data);
 *  \brief A function type definition for a function to be called after every generation of a
 *  basic evolution strategy.
 *  @see RVBasicEvolutionStrategyOnGenerationFinished()
 */
typedef void (*RVGenerationFinished)(struct RVBasicEvolutionStrategy *es, int gen, void *data);

/* \fn void RVBasicEvolutionOnGenerationFinished(struct RVBasicEvolutionStrategy *es, RVGenerationFinished fun, void *data);
 * \brief Sets a user-supplied function to be called at the end of every evolution generation.
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 * \param fun the function to be called at every generation. A NULL value will be ignored.
 * \param data the user-supplied data, if any.
 * @see RVBasicEvolutionStrategyStart()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyOnGenerationFinished(struct RVBasicEvolutionStrategy *es, RVGenerationFinished fun, void *data);

/* \typedef int (*RVEvolutionShouldTerminate)(struct RVBasicEvolutionStrategy *es, unsigned int g, void *data);
 * \brief A function type definition for terminating a basic evolution strategy.
 * \param es a pointer to a basic evolution strategy.
 * \param g the current generation number
 * \param data the user-supplied data, if any
 * \return Returns 1 if the process should terminate. 
 * @see RVBasicEvolutionStrategySetTerminationCriteria()
 */
typedef int (*RVEvolutionShouldTerminate)(struct RVBasicEvolutionStrategy *es, unsigned int g, void *data);

/* \fn void RVBasicEvolutionStrategySetTerminationCriteria(struct RVBasicEvolutionStrategy *es, RVEvolutionShouldTerminate fun, void *data);
 * \brief Checks if an evolution should terminate after each generation.
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 * \param fun the function that will actually check whether the evolution should terminate.
 * \param data the user-supplied data, if any.
 * @see RVEvolutionShouldTerminate
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetTerminationCriteria(struct RVBasicEvolutionStrategy *es, RVEvolutionShouldTerminate fun, void *data);

/* \fn double RVBasicEvolutionStrategyGetDesignParameter(struct RVBasicEvolutionStrategy *es, int parent, int paramIndex);
 * \brief Gets the design parameter of a parent atom.
 * \param es the pointer the basic evolution strategy whose parent atom we want to check. The function will check for a NULL argument. 
 * \param parent the parent index which should be less than mu, for a (mu/rho,lambda) or (mu/rho+lambda) evolution strategy. The index is 0-based, so use '0' for the first parent. 
 * \param paramIndex the 0-based design variable index which must be less than n, the problem's dimensionality.
 * \return Returns the value of the design parameter for the given index.
 * @see RVBasicEvolutionStrategyGetObjective()
 */
DLL_PUBLIC double RVBasicEvolutionStrategyGetDesignParameter(struct RVBasicEvolutionStrategy *es, int parent, int paramIndex);

/*! double RVBasicEvolutionStrategyGetObjective(struct RVBasicEvolutionStrategy *es, int parent, int objIndex);
 *  \brief Gets an objective value from a parent atom.
 *  \param es the pointer the basic evolution strategy whose parent atom we want to check. The function will check for a NULL  argument.
 *  \param parent the parent index which should be less than mu, for a (mu/rho,lambda) or (mu/rho+lambda) evolution strategy. The index is 0-based, so use '0' for the first parent. 
 *  \param objIndex the 0-based objective index which must be less than m, the problem's number of objectives.
 *  \return Returns the value of the desired objective.
 */
DLL_PUBLIC double RVBasicEvolutionStrategyGetObjective(struct RVBasicEvolutionStrategy *es, int parent, int objIndex);

/*! \typedef double (*RVRandom)(void *data);
 *  \brief A function type definition to generate a random number.
 *  The random number generated should follow a normal distribution with a 0-mean and a unit variance.
 *  \param data the user-supplied data, if any.
 *  \return Returns a random number from a normal distribution with zero mean and unit variance.
 *  @see RVBasicEvolutionStrategySetRNG()
 */
typedef double (*RVRandom)(void *data);

/*! \fn void RVBasicEvolutionStrategySetRNG(struct RVBasicEvolutionStrategy *es, RVRandom fun, void *data);
 *  \brief Sets an external random number generator to be used by an evolution process.
 *  You can use this function to change the internal random number generator. The internal generator is based
 *  on rand(), which is not a very good RNG. 
 *  \param es the basic evolution strategy to use the external random number generator.
 *  \param fun the function that will generate a random number.
 *  \param data the user-supplied data, if any.
 *  @see RVRandom
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetRNG(struct RVBasicEvolutionStrategy *es, RVRandom fun, void *data);

/*! \fn void RVBasicEvolutionStrategyDestroy(struct RVBasicEvolutionStrategy *es);
 *  \brief Destroys an RVBasicEvolutionStrategy objects and frees memory.
 *  \param es the basic evolution strategy to be destroyed. The function will check for NULL.
 *  @see RVBasicEvolutionStrategyCreate()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyDestroy(struct RVBasicEvolutionStrategy *es);

#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_H_INCLUDED
