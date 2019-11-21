// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {
  
  private static float ratio(sProcess process) {
    return (float)process.cpudone / (float)process.cputime / (float)process.priority;
  }

  public static Results Run(final int runtime, final Vector processVector, final Results result) {
    int i = 0;
    int comptime = 0;
    int currentProcess = 0;
    int previousProcess = 0;
    int size = processVector.size();
    int completed = 0;
    
    //Vector<Float> cpuUsageRatios = new Vector<>(processVector.size(), 0);

    String resultsFile = "Summary-Processes";

    result.schedulingType = "Interactive";
    result.schedulingName = "Guaranteed Scheduling"; 
    try {
      //BufferedWriter out = new BufferedWriter(new FileWriter(resultsFile));
      //OutputStream out = new FileOutputStream(resultsFile);
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(currentProcess);
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
      while (comptime < runtime) {
        if (process.cpudone == process.cputime) {
          completed++;
          out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
          if (completed == size) {
            result.compuTime = comptime;
            out.close();
            return result;
          }

          // Chosing process with minimal cpu usage done
          float minratio = ratio((sProcess) processVector.elementAt(0));
          currentProcess = 1;
          for (i = 1; i < size; ++i) {
            process = (sProcess) processVector.elementAt(i);
            float processRatio = ratio(process);
            if (process.cpudone < process.cputime && processRatio < minratio) { 
              minratio = processRatio;
              currentProcess = i;
            }
          }
          process = (sProcess) processVector.elementAt(currentProcess);
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
        }  
        if (process.ioblocking == process.ionext) {
          out.println("Process: " + currentProcess + " I/O blocked... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
          process.numblocked++;
          process.ionext = 0; 
          previousProcess = currentProcess;


          // Chosing process with minimal cpu usage done
          float minratio = ratio((sProcess) processVector.elementAt(0));
          currentProcess = 1;
          for (i = 1; i < size; ++i) {
            if(i == previousProcess)
              continue;
            process = (sProcess) processVector.elementAt(i);
            float processRatio = ratio(process);
            if (process.cpudone < process.cputime && processRatio < minratio) { 
              minratio = process.cpudone;
              currentProcess = i;
            }
          }
          if(currentProcess == -1)
            currentProcess = 0;

          process = (sProcess) processVector.elementAt(currentProcess);
          out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.ioblocking + " " + process.cpudone + " " + process.cpudone + ")");
        }        
        process.cpudone++;       
        if (process.ioblocking > 0) {
          process.ionext++;
        }
        comptime++;
      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }
}
