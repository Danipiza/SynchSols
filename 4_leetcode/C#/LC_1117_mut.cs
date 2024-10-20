using System;
using System.Threading;

public class H2O {

    private int h;
    private int o;
    private object mut;

    public H2O() {
        h=2;
        o=1;
        
        mut=new object();
    }

    private void reset_values(){

        if(h==0&&o==0){
            h=2;
            o=1;
            Monitor.PulseAll(mut);         
        }
    }

    public void Hydrogen(Action releaseHydrogen) {

		lock (mut) {
            while (h==0) Monitor.Wait(mut);

            h--;
            
            // releaseHydrogen() outputs "H". Do not change or remove this line.
            releaseHydrogen();
            reset_values();            
        }
        
    }

    public void Oxygen(Action releaseOxygen) {
        
        lock (mut) {
            while (o==0) Monitor.Wait(mut);
            o--;
            
            // releaseOxygen() outputs "O". Do not change or remove this line.
		    releaseOxygen();
            reset_values();
        }
        
    }
}