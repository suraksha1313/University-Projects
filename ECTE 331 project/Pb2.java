package prob2;

class Data { //this class holds the shared data and synchronization flags for communication between threads.
	
//initialize boolean flags as false
    boolean GoA1 = false;
    boolean GoA2 = false;
    boolean GoA3 = false;
    boolean GoB1 = false;
    boolean GoB2 = false;
    boolean GoB3 = false;
    
//initialize the shared variables as double 
    public double A1, A2, A3, B1, B2, B3;
    
//initialize sum variable
    public static int s = 0;

//method to calculate the sum using a synchronized block to ensure thread safety.
    public static synchronized double Sum(int n) {
       s = n * (n + 1) / 2; 
        return s;
    }
}

class Thread1 implements Runnable { //represents the first thread
    private Data d;

    public Thread1(Data d) {
        this.d = d; 
    }

    @Override
    public void run() {
    	//run function A1
        synchronized (d) {
            d.A1 = Data.Sum(100); //calculate A1 value
            d.GoB1 = true; // Signal Thread2 that A1 value is ready
            System.out.println("Thread A1 value is: " + d.A1);
            d.notify();
        }
        
      //run function A2
        synchronized (d) {
            while (!d.GoA2) {
                try {
                    d.wait(); // Wait until Thread2 signals GoA2
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
            //flag is clear
            d.A2 = d.B2 + Data.Sum(400); //calculate A2 value
            System.out.println("Thread A2 value is: " + d.A2);
            d.GoB3 = true; // Signal Thread2 that A2 value is ready
            d.notify();
        }
        
      //run function A3
        synchronized (d) {
            while (!d.GoA3) {
                try {
                    d.wait(); // Wait until Thread2 signals GoA3
                } catch (InterruptedException e) {e.printStackTrace();}
            }
          //flag is clear
            d.A3 = d.B3 + Data.Sum(600); //calculate A3 value
            System.out.println("Thread A3 value is: " + d.A3);
            d.notify();
        }
    }
}

class Thread2 extends Thread { //represents the second thread

    private Data d;

    public Thread2(Data d) {
        this.d = d;
    }

    @Override
    public void run() {
    	//run function B1
        synchronized (d) {
            while (!d.GoB1) {
                try {
                    d.wait(); // Wait until Thread1 signals GoB1
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
          //flag is clear
            d.B1 = d.A1 + Data.Sum(200); //calculate B1 value
            System.out.println("Thread B1 value is: " + d.B1);
            d.GoB2 = true; // Signal Thread1 that B1 value is ready
            d.notify();
        }

      //run function B2
        synchronized (d) {
            while (!d.GoB2) {
                try {
                    d.wait(); // Wait until Thread1 signals GoB2
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
          //flag is clear
            d.B2 = Data.Sum(300); //calculate B2 value
            System.out.println("Thread B2 value is: " + d.B2);
            d.GoA2 = true; // Signal Thread1 that B2 value is ready
            d.notify();
        }

      //run function B3
        synchronized (d) {
            while (!d.GoB3) {
                try {
                    d.wait(); // Wait until Thread1 signals GoB3
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
          //flag is clear
            d.B3 = d.A2 + Data.Sum(500); //calculate B3 value
            System.out.println("Thread B3 value is: " + d.B3);
            d.GoA3 = true; // Signal Thread1 that B3 value is ready
            d.notify();
        }
    }
}

public class Pb2 { //main class

    public static void main(String[] args) {
        Data d = null;
        int i = 0;
        int num_iter = 1000;
        while (i++ < num_iter) { //a loop is set up to run multiple iterations
            d = new Data(); //new instance of Data is created
            //Instances of Thread2 and Thread are created
            Thread2 t1 = new Thread2(d);
            Thread t2 = new Thread(new Thread1(d));

            t1.start();
            t2.start();

            try {
                t1.join();
                t2.join();
            } catch (InterruptedException e) {e.printStackTrace();}
            
            // the values of A1, A2, A3, B1, B2, B3 are printed for the current iteration
            System.out.println("---------------------------------------");
            System.out.println("Iteration " + i + " - A1: " + d.A1);
            System.out.println("Iteration " + i + " - A2: " + d.A2);
            System.out.println("Iteration " + i + " - A3: " + d.A3);
            System.out.println("Iteration " + i + " - B1: " + d.B1);
            System.out.println("Iteration " + i + " - B2: " + d.B2);
            System.out.println("Iteration " + i + " - B3: " + d.B3);
            System.out.println("---------------------------------------");
        }

    }
}
