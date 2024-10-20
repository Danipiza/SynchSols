using System;
using System.Threading;

public class H2O {

    private int h;
    private int o;

    private SemaphoreSlim semGlobal;
    private SemaphoreSlim semH;
    private SemaphoreSlim semO;

    public H2O() {
        h=2;
        o=1;

        semGlobal =new SemaphoreSlim(2, 2);
        semH      =new SemaphoreSlim(2, 2);
        semO      =new SemaphoreSlim(1, 1);
    }

    private void ResetValues() {
        if (h==0 && o==0) {
            h=2;
            o=1;
            semH.Release(2);
            semO.Release();
        }
    }

    public void Hydrogen(Action releaseHydrogen) {
		  
        semH.Wait();
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();

        semGlobal.Wait();
        h--;
        ResetValues();
        semGlobal.Release();
    }

    public void Oxygen(Action releaseOxygen) {
        
        semO.Wait();
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        semGlobal.Wait();
        o--;
        ResetValues();
        semGlobal.Release();
    }
}