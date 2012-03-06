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
#define RV_API_MINOR	1
/*! \def RV_API_PATCH
 *  \brief The library's major version.
 */
#define RV_API_PATCH	0

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

/*! \typedef int (*RV_OBJECTIVE_EVALUATION_FUNCTION(const struct RVArray * dv, struct RVArray *obj, void *data);
 *  \brief A function signature that evaluates a design vector.
 *  This function is called in every generation to evaluate each population atom.
 *  \param dv a pointer to an RVArray of the design parameters.
 *  \param obj a pointer to an RVArray of the objectives, to be calculated
 *  \param data the user-supplied data, if any, used for the creation of 
 *  objective function object.
 *  @see RVObjectiveFunctionCreate()
 */
typedef int (*RV_OBJECTIVE_EVALUATION_FUNCTION)(const struct RVArray* dv, struct RVArray *obj, void *data);

/*! \fn RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data);
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
DLL_PUBLIC struct RVObjectiveFunction* RVObjectiveFunctionCreate(int dim, int objectives, RV_OBJECTIVE_EVALUATION_FUNCTION fun, void *data);

/*! \fn int RVObjectiveFunctionGetDimensionality(struct RVObjectiveFunction *obf)
 *	\brief Gets the dimensionality <i>n</i> for the given objective function.
 *	\param obf an objective function object.
 *	\return the dimensionality
 *	@see RVObjectiveFunctionCreate(), RVObjectiveFunctionGetNumberOfObjectives()
 */
DLL_PUBLIC int RVObjectiveFunctionGetDimensionality(struct RVObjectiveFunction *obf);

/*! \fn int RVObjectiveFunctionGetNumberOfObjectives(struct RVObjectiveFunction *obf)
 *	\brief Gets the number of objectives <i>m</i> of the given objective function.
 *	\param obf an objective function object.
 *	\return the number of objectives
 *	@see RVObjectiveFunctionCreate(), RVObjectiveFunctionGetDimmensionality()
 */
DLL_PUBLIC int RVObjectiveFunctionGetNumberOfObjectives(struct RVObjectiveFunction *obf);

/*! \fn void RVObjectiveFunctionDestroy(RVObjectiveFunction *obf)
 *	\brief Gets the user data from an objective function object.
 *	\param obf an objective function object.
 *	\return the user data, if any, or NULL.
 *	@see RVObjectiveFunctionCreate()
 */
DLL_PUBLIC void * RVObjectiveFunctionGetUserData(struct RVObjectiveFunction *obf);
	
/*! \fn RV_OBJECTIVE_EVALUATION_FUNCTION RVObjectiveFunctionGetEvalFun(struct RVObjectiveFunction *obf);
 *  \brief Gets the evaluation function.
 *  \param obf an objective function object.
 *  \return the evaluation function of type RV_OBJECTIVE_EVALUATION_FUNCTION
 *  @see RVObjectiveFunctionCreate()
 */
DLL_PUBLIC RV_OBJECTIVE_EVALUATION_FUNCTION RVObjectiveFunctionGetEvalFun(struct RVObjectiveFunction *obf);

/*! \fn void RVObjectiveFunctionDestroy(struct RVObjectiveFunction *obf);
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

/*! \typedef RV_SET_INITIAL_VALUES_FUNCTION(struct RVArray *designParams, struct RVArray *objectives, void *data);
 *  \brief A function signature that calculates the initial population values. 
 *  \param designParams the design vector of a candidate solution.
 *  \param objectives a pointer to the objectives vector. You do not need change the objective values. The Objective function will be called automatically after the initialization function.
 *  \param data the user data that may or may not be supplied. These are the same user data used 
 *  during the call of RVBasicEvolutionStrategyInitializePopulation. 
 *  @see RVBasicEvolutionStrategyInitializePopulation(), RVObjectiveFunctionCreate()
 */
typedef void (*RV_SET_INITIAL_VALUES_FUNCTION)(struct RVArray *designParams, struct RVArray *objectives, void *data);

/* \fn void RVBasicEvolutionStrategyPopulationSetInitialValues(RVBasicEvolutionStrategy *es, RVSetPopulationInitialValues fun);
 * \brief Sets a function that will be called to calculate the initial values of the population. 
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value and do nothing
 * if es is actually NULL.
 * \param fun the function that will be called to initialize every atom of the population. The function will ignore * NULL values for fun.
 * \param data the user data that may or may not be supplied by the client. 
 * @see RV_SET_INITIAL_VALUES_FUNCTION
 */
DLL_PUBLIC void RVBasicEvolutionStrategyInitializePopulation(struct RVBasicEvolutionStrategy *es, RV_SET_INITIAL_VALUES_FUNCTION fun, void *data);

/*! \typedef void (*RV_CONSTRAIN_PARAMS_FUNCTION)(struct RVArray *dv, void *data);
 *  \brief A function type definition to constrain design params. 
 *  @see RVBasicEvolutionStrategySetParameterConstraints()
 */
typedef void (*RV_CONSTRAIN_PARAMS_FUNCTION)(struct RVArray *designParam, void *data);

/*! \fn void RVBasicEvolutionStrategySetParameterConstraints(struct RVBasicEvolutionStrategy *es, RV_CONSTRAIN_PARAMS_FUNCTION fun, void *data);
 *  \brief Sets a function to be called before an atom's objectives are evaluated. 
 *  Use this function with caution. Only the design variables can be constrained, and you should
 *  have in mind that the endogenous strategy parameters assume that no constraining will happen. You can avoid 
 *  using this function by an appropriate objective function scheme.
 *  \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 *  \param fun the function that will constrain the design params of an atom. It will be checked for NULL.
 *  \param data the user-supplied data, if any.
 *  @see RV_CONSTRAIN_PARAMS_FUNCTION
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetParameterConstraints(struct RVBasicEvolutionStrategy *es, RV_CONSTRAIN_PARAMS_FUNCTION fun, void *data);

/* \fn void RVBasicEvolutionStrategyStart(struct RVBasicEvolutionStrategy *es);
 * \brief Starts an evolution strategy
 * A started evolution strategy will execute for a maximum of 1000 generations, unless some termination
 * criteria has been set by RVBasicEvolutionStrategySetTerminationCriteria.
 * \param es a pointer to a basic evolution strategy object. The function will check for a NULL argument.
 * @see RVBasicEvolutionOnGenerationFinished(), RVBasicEvolutionStrategySetTerminationCriteria()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyStart(struct RVBasicEvolutionStrategy *es);

/*! \typedef void (*RV_BASIC_GENERATION_FINISHED_FUN)(RVBasicEvolutionStrategy *es, int gen, void *data);
 *  \brief A function type definition for a function to be called after every generation of a
 *  basic evolution strategy.
 *  @see RVBasicEvolutionStrategyOnGenerationFinished()
 */
typedef void (*RV_BASIC_GENERATION_FINISHED_FUNCTION)(struct RVBasicEvolutionStrategy *es, int gen, void *data);

/* \fn void RVBasicEvolutionOnGenerationFinished(struct RVBasicEvolutionStrategy *es, RV_BASIC_GENERATION_FINISHED_FUNCTION fun, void *data);
 * \brief Sets a user-supplied function to be called at the end of every evolution generation.
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 * \param fun the function to be called at every generation. A NULL value will be ignored.
 * \param data the user-supplied data, if any.
 * @see RVBasicEvolutionStrategyStart()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyOnGenerationFinished(struct RVBasicEvolutionStrategy *es, RV_BASIC_GENERATION_FINISHED_FUNCTION fun, void *data);

/* \typedef int (*RV_BASIC_SHOULD_TERMINATE_FUNCTION)(struct RVBasicEvolutionStrategy *es, unsigned int g, void *data);
 * \brief A function type definition for terminating a basic evolution strategy.
 * \param es a pointer to a basic evolution strategy.
 * \param g the current generation number
 * \param data the user-supplied data, if any
 * \return Returns 1 if the process should terminate. 
 * @see RVBasicEvolutionStrategySetTerminationCriteria()
 */
typedef int (*RV_BASIC_SHOULD_TERMINATE_FUNCTION)(struct RVBasicEvolutionStrategy *es, unsigned int g, void *data);

/* \fn void RVBasicEvolutionStrategySetTerminationCriteria(struct RVBasicEvolutionStrategy *es, RV_BASIC_SHOULD_TERMINATE_FUNCTION fun, void *data);
 * \brief Checks if an evolution should terminate after each generation.
 * \param es a pointer to a basic evolution strategy. The function will check for a NULL value.
 * \param fun the function that will actually check whether the evolution should terminate.
 * \param data the user-supplied data, if any.
 * @see RV_BASIC_SHOULD_TERMINATE_FUNCTION
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetTerminationCriteria(struct RVBasicEvolutionStrategy *es, RV_BASIC_SHOULD_TERMINATE_FUNCTION fun, void *data);

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

/*! \typedef double (*RV_RANDOM_FUNCTION)(void *data);
 *  \brief A function type definition to generate a random number.
 *  The random number generated should follow a normal distribution with a 0-mean and a unit variance.
 *  \param data the user-supplied data, if any.
 *  \return Returns a random number from a normal distribution with zero mean and unit variance.
 *  @see RVBasicEvolutionStrategySetRNG()
 */
typedef double (*RV_RANDOM_FUNCTION)(void *data);

/*! \fn void RVBasicEvolutionStrategySetRNG(struct RVBasicEvolutionStrategy *es, RV_RANDOM_FUNCTION fun, void *data);
 *  \brief Sets an external random number generator to be used by an evolution process.
 *  You can use this function to change the internal random number generator. The internal generator is based
 *  on rand(), which is not a very good RNG. 
 *  \param es the basic evolution strategy to use the external random number generator.
 *  \param fun the function that will generate a random number.
 *  \param data the user-supplied data, if any.
 *  @see RV_RANDOM_FUNCTION
 */
DLL_PUBLIC void RVBasicEvolutionStrategySetRNG(struct RVBasicEvolutionStrategy *es, RV_RANDOM_FUNCTION fun, void *data);

/*! \fn void RVBasicEvolutionStrategyDestroy(struct RVBasicEvolutionStrategy *es);
 *  \brief Destroys an RVBasicEvolutionStrategy objects and frees memory.
 *  \param es the basic evolution strategy to be destroyed. The function will check for NULL.
 *  @see RVBasicEvolutionStrategyCreate()
 */
DLL_PUBLIC void RVBasicEvolutionStrategyDestroy(struct RVBasicEvolutionStrategy *es);
    
/*! \struct RVDifferentialEvolution;
 *  \brief Represents a differential evolution algorithm.
 *  @see RVDifferentialEvolutionCreate()
 */
struct RVDifferentialEvolution;

/*!	\fn struct RVDifferentialEvolution *RVDifferentialEvolutionCreate(unsigned int pnum, double Fp, double CRp, struct RVObjectiveFunction *fun)
 *	\brief Creates a differential evolution object.
 *	Use this function to build a differential evolution object.
 *	\param pnum the population size, a number greater than zero.
 *	\param Fp the differential weight, which is a number in the [0, 2] interval.
 *	\param CRp the crossover coefficient, which is a number in the [0, 1] interval.
 *	\param fun the objective function object to be minimized. It must not be NULL. 
 *  \return a differential evolution object or NULL, if any of the arguments were invalid.
 *	@see RVDifferentialEvolutionStart(), RVDifferentialEvolutionDestroy()
 */
DLL_PUBLIC struct RVDifferentialEvolution *RVDifferentialEvolutionCreate(unsigned int pnum, double Fp, double CRp, struct RVObjectiveFunction *fun);

/* \fn void RVDifferentialEvolutionInitializePopulation(RVDifferentialEvolutionStrategy *de, RV_SET_INITIAL_VALUES_FUNCTION fun);
 * \brief Sets a function that will be called to calculate the initial values of the population. 
 * \param es a pointer to a differential evolution object. The function will check for a NULL value and do nothing
 * if de is actually NULL.
 * \param fun the function that will be called to initialize every agent of the population. The function will ignore NULL values for fun.
 * \param data the user data that may or may not be supplied by the client. 
 * @see RV_SET_INITIAL_VALUES_FUNCTION
 */
DLL_PUBLIC void RVDifferentialEvolutionInitializePopulation(struct RVDifferentialEvolution *de, RV_SET_INITIAL_VALUES_FUNCTION fun, void *data);

/*! \typedef void (*RV_DE_GENERATION_FINISHED_FUNCTION)(struct RVDifferentialEvolution *de, int gen, void *data);
 *  \brief A function type definition for a function to be called after every generation of a
 *  differential evolution.
 *  @see RVDifferentialEvolutionSetOnGenerationFinishedFun()
 */
typedef void (*RV_DE_GENERATION_FINISHED_FUNCTION)(struct RVDifferentialEvolution *de, unsigned int generation, void *data);

/* \fn void RVDifferentialEvolutionSetOnGenerationFinishedFun(struct RVDifferentialEvolution *de, RV_DE_GENERATION_FINISHED_FUNCTION fun, void *data);
 * \brief Sets a user-supplied function to be called at the end of every evolution generation.
 * \param es a pointer to a differential evolution. The function will check for a NULL value.
 * \param fun the function to be called at every generation. A NULL value will be ignored.
 * \param data the user-supplied data, if any.
 * @see RVDifferentialEvolutionStart()
 */
DLL_PUBLIC void RVDifferentialEvolutionSetOnGenerationFinishedFun(struct RVDifferentialEvolution *de, RV_DE_GENERATION_FINISHED_FUNCTION fun, void *data);

/* \typedef int (*RV_DE_SHOULD_TERMINATE_FUNCTION)(struct RVDifferentialEvolutionStrategy *de, unsigned int g, void *data);
 * \brief A function type definition for terminating a differential evolution.
 * \param es a pointer to a differential evolution.
 * \param g the current generation number
 * \param data the user-supplied data, if any
 * \return Returns 1 if the process should terminate. 
 * @see RVDifferentialEvolutionSetTerminationFun()
 */
typedef int (*RV_DE_SHOULD_TERMINATE_FUNCTION)(struct RVDifferentialEvolution *de, unsigned int generation, void *data);

/* \fn void RVDifferentialEvolutionSetTerminationFun(struct RVDifferentialEvolution *de, RV_DE_SHOULD_TERMINATE_FUNCTION fun, void *data);
 * \brief Checks if an evolution should terminate after each generation.
 * \param es a pointer to a differential evolution. The function will check for a NULL value.
 * \param fun the function that will actually check whether the evolution should terminate.
 * \param data the user-supplied data, if any.
 */
DLL_PUBLIC void RVDifferentialEvolutionSetTerminationFun(struct RVDifferentialEvolution *de, RV_DE_SHOULD_TERMINATE_FUNCTION fun, void *data);

/* \fn double RVDifferentialEvolutionGetDesignParameter(struct RVDifferentialEvolution *de, int agentIndex, int paramIndex);
 * \brief Gets the design parameter of an agent.
 * \param de the pointer the differential evolution whose agent we want to check. The function will check for a NULL argument. 
 * \param agentIndex the agent index. The index is 0-based, so use '0' for the first agent. 
 * \param paramIndex the 0-based design variable index which must be less than n, the problem's dimensionality.
 * \returnagentIndexReturns the value of the design parameter for the given index.
 * @see RVDifferentialEvolutionStrategyGetObjective()
 */
DLL_PUBLIC double RVDifferentialEvolutionGetDesignParameter(struct RVDifferentialEvolution *de, int agentIndex, int paramIndex);

/*! double RVDifferentialEvolutionGetObjective(struct RVDifferentialEvolution *de, int agentIndex, int objIndex);
 *  \brief Gets an objective value from an agent.
 *  \param de the pointer the differential evolution whose agent we want to check. The function will check for a NULL  argument.
 *  \param agentIndex the agent index. The index is 0-based, so use '0' for the first parent. 
 *  \param objIndex the 0-based objective index which must be less than m, the problem's number of objectives.
 *  \return Returns the value of the desired objective.
 */
DLL_PUBLIC double RVDifferentialEvolutionGetObjective(struct RVDifferentialEvolution *de, int agentIndex, int objIndex);

/* \fn void RVDifferentialEvolutionStart(struct RVDifferentialEvolutionStart *de);
 * \brief Starts an differential evolution.
 * A started differential evolution will execute for a maximum of 1000 generations, unless some termination
 * criteria has been set by RVDifferentialEvolutionSetTerminationFun.
 * \param de a pointer to a differential evolution object. The function will check for a NULL argument.
 * @see RVDifferentialEvolutionOnGenerationFinished(), RVDifferentialEvolutionSetTerminationFun()
 */
DLL_PUBLIC void RVDifferentialEvolutionStart(struct RVDifferentialEvolution *de);

/*!	\fn void RVDifferentialEvolutionDestroy(struct RVDifferentialEvolution *de);
 *	\brief Destroys a differential evolution object.
 *	Use this function to destroy a differential evolution object and free memory.
 *	\param de the differential evolution to be destroyed. If NULL, the function will do nothing.
 *	@see RVDifferentialEvolutionCreate()
*/
DLL_PUBLIC void RVDifferentialEvolutionDestroy(struct RVDifferentialEvolution *de);

/*! \struct RVCmaEvolutionStrategy;
 *  \brief Represents a CMA evolution algorithm.
 *  @see RVCmaEvolutionStrategyCreate()
 */
struct RVCmaEvolutionStrategy;

/*! \fn struct RVCmaEvolutionStrategy* RVCmaEvolutionStrategyCreate(unsigned int lambda, struct RVObjectiveFunction *fun);
 *  \brief Creates a CMA evolution object.
 *  Use this function to build a CMA evolution object.
 *  \param lambda the number of offsprings the evolution will use. Must be equal or greater to 1.
 *  \param fun the objective function to optimize. Must not be NULL.
 *  \return a CMA evolution object, or NULL if any of the parameters were invalid.
 *  @see RVCmaEvolutionStrategyDestroy
 */
DLL_PUBLIC struct RVCmaEvolutionStrategy* RVCmaEvolutionStrategyCreate(unsigned int lambda, struct RVObjectiveFunction *fun);

/*! \typedef int (*RV_CMA_SHOULD_TERMINATE_FUNCTION)(struct RVCmaEvolutionStrategy *cma, unsigned int g, void *data);
 *  \brief A function type definition for terminating a CMA evolution.
 *  \param es a pointer to a CMA evolution.
 *  \param g the current generation number
 *  \param data the user-supplied data, if any
 *  \return Returns 1 if the process should terminate.
 *  @see RVCmaEvolutionSetTerminationFun()
 */
typedef int (*RV_CMA_SHOULD_TERMINATE_FUNCTION)(struct RVCmaEvolutionStrategy *cma, unsigned int g, void *data);

/*! \fn void RVCmaEvolutionSetTerminationFun(struct RVCmaEvolution *cma, RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data);
 *  \brief Checks if an evolution should terminate after each generation.
 *  \param es a pointer to a CMA evolution. The function will check for a NULL value.
 *  \param fun the function that will actually check whether the evolution should terminate.
 *  \param data the user-supplied data, if any.
 */
DLL_PUBLIC void RVCmaEvolutionSetTerminationFun(struct RVCmaEvolutionStrategy *cma, RV_CMA_SHOULD_TERMINATE_FUNCTION fun, void *data);

/*! \fn void RVCmaEvolutionStrategyDestroy(struct RVCmaEvolutionStrategy *cma);
 *  \brief Destroys a CMA evolution object.
 *  Use this function to destroy a CMA evolution object.
 *  \param cma the CMA evolution strategy to destroy and free memory. The function will check
 *  for a NULL argument.
 *  @see RVCmaEvolutionStrategyCreate
 */
DLL_PUBLIC void RVCmaEvolutionStrategyDestroy(struct RVCmaEvolutionStrategy *es);
#ifdef __cplusplus
}
#endif

#endif // REVOLUTION_H_INCLUDED
