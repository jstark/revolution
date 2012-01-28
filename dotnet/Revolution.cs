using System;
using System.Runtime.InteropServices;


public static class Revolution {
	[DllImport("revolution")]
	public extern static int RVGetMajorVersion();
	[DllImport("revolution")]
	public extern static int RVGetMinorVersion();
	[DllImport("revolution")]
	public extern static int RVGetPatchVersion();
	[DllImport("revolution")]
	public extern static uint RVArrayGetSize(System.IntPtr array);
	[DllImport("revolution")]
	public extern static double RVArrayGetElementAtIndex(System.IntPtr array, uint index);
	[DllImport("revolution")]
	public extern static void RVArraySetElementAtIndex(System.IntPtr array, uint index, double val);
	public delegate int RVObjectiveEvaluationFun (System.IntPtr dv, System.IntPtr obj, System.IntPtr data);
	[DllImport("revolution")]
	public extern static System.IntPtr RVObjectiveFunctionCreate(int dim, int objectives, RVObjectiveEvaluationFun fun, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVObjectiveFunctionDestroy(System.IntPtr obf);
	public enum RVSelectionMode {
		kRVSelectionModeInvalid,
		kRVSelectionModePlus,
		kRVSelectionModeComma
	}
	[DllImport("revolution")]
	public extern static System.IntPtr RVBasicEvolutionStrategyCreate(int mu, int rho, int lambda, RVSelectionMode mode, System.IntPtr fun);
	public delegate void RVPopulationSetInitialValues (System.IntPtr designParams, System.IntPtr objectives, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategyInitializePopulation(System.IntPtr es, RVPopulationSetInitialValues fun, System.IntPtr data);
	public delegate void RVConstrainParam (System.IntPtr designParam, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategySetParameterConstraints(System.IntPtr es, RVConstrainParam fun, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategyStart(System.IntPtr es);
	public delegate void RVGenerationFinished (System.IntPtr es, int gen, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategyOnGenerationFinished(System.IntPtr es, RVGenerationFinished fun, System.IntPtr data);
	public delegate int RVEvolutionShouldTerminate (System.IntPtr es, uint g, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategySetTerminationCriteria(System.IntPtr es, RVEvolutionShouldTerminate fun, System.IntPtr data);
	[DllImport("revolution")]
	public extern static double RVBasicEvolutionStrategyGetDesignParameter(System.IntPtr es, int parent, int paramIndex);
	[DllImport("revolution")]
	public extern static double RVBasicEvolutionStrategyGetObjective(System.IntPtr es, int parent, int objIndex);
	public delegate double RVRandom (System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategySetRNG(System.IntPtr es, RVRandom fun, System.IntPtr data);
	[DllImport("revolution")]
	public extern static void RVBasicEvolutionStrategyDestroy(System.IntPtr es);
}
