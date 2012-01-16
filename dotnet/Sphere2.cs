using System;
using System.Runtime.InteropServices;

public class Test {

    static int sphere2(IntPtr d, IntPtr o, IntPtr data)
    {
        double x = Revolution.RVArrayGetElementAtIndex(d, 0);
        double y = Revolution.RVArrayGetElementAtIndex(d, 1);
        Revolution.RVArraySetElementAtIndex(o, 0, x * x + y * y);
        return 0;
    }

    static void init(IntPtr d, IntPtr o, IntPtr data)
    {
        Revolution.RVArraySetElementAtIndex(d, 0, 100.0);
        Revolution.RVArraySetElementAtIndex(d, 1, 100.0);
    }

    static void print(IntPtr es, int gen, IntPtr data)
    {
        Console.WriteLine("{0}:\t{1}\t{2}\t\t:{3}", 
                gen, 
                Revolution.RVBasicEvolutionStrategyGetDesignParameter(es, 0, 0), 
                Revolution.RVBasicEvolutionStrategyGetDesignParameter(es, 1, 0),
                Revolution.RVBasicEvolutionStrategyGetObjective(es, 0, 0));
    }

    static int terminate(IntPtr es, uint gen, IntPtr data)
    {
        return gen == 100 ? 1 : 0;
    }

    public static void Main(string[] args) {
        IntPtr objectiveFunction = Revolution.RVObjectiveFunctionCreate(2, 1, sphere2, IntPtr.Zero);
        IntPtr es = Revolution.RVBasicEvolutionStrategyCreate(5, 1, 5, Revolution.RVSelectionMode.kRVSelectionModePlus, objectiveFunction);
        Revolution.RVBasicEvolutionStrategyInitializePopulation(es, init, IntPtr.Zero);
        Revolution.RVBasicEvolutionStrategyOnGenerationFinished(es, print, IntPtr.Zero);
        Revolution.RVBasicEvolutionStrategySetTerminationCriteria(es, terminate, IntPtr.Zero);
        Revolution.RVBasicEvolutionStrategyStart(es);
        Revolution.RVBasicEvolutionStrategyDestroy(es);
        Revolution.RVObjectiveFunctionDestroy(objectiveFunction);
    }
}
