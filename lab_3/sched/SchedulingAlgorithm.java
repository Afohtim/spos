// Run() is called from Scheduling.main() and is where
// the scheduling algorithm written by the user resides.
// User modification should occur within the Run() function.

import java.util.Vector;
import java.io.*;

public class SchedulingAlgorithm {

  private static float PriorityFunction(int priority) {
    return (float)Math.pow(2.0f, priority/20);
    //return (float)(priority == 0 ? 1 : priority);
  }
  
  private static float ratio(sProcess process, float expectedCpudone, int n) {
    return (float)process.cpudone / expectedCpudone / PriorityFunction(process.priority);
  }


  public static Results Run(final int runtime, final Vector processVector, final Results result) {
    int i = 0;
    int comptime = 0;
    int currentProcess = 0;
    int previousProcess = 0;
    int size = processVector.size();
    int completed = 0;
    Boolean noWork = true;
    Vector<Integer> currentBlockTimes = new Vector<>();
    for(int j = 0; j < size; ++j) {
      currentBlockTimes.addElement(0);
    }
    

    String resultsFile = "Summary-Processes";

    result.schedulingType = "Interactive";
    result.schedulingName = "Guaranteed Scheduling"; 
    try {
      PrintStream out = new PrintStream(new FileOutputStream(resultsFile));
      sProcess process = (sProcess) processVector.elementAt(currentProcess);
      out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.currentCPUTimeWorking + " " + process.cpudone + " " + process.priority + ")");
      while (comptime < runtime || completed != size) {
        //if process finished
        if (currentProcess != -1 && process.cpudone == process.cputime) {
          completed++;
          out.println("Process: " + currentProcess + " completed... (" + process.cputime + " " + process.currentCPUTimeWorking + " " + process.cpudone + " " + process.priority + ")");
          if (completed == size) {
            result.compuTime = comptime;
            out.close();
            return result;
          }

          // Chosing process with minimal cpu usage done
          float minratio = 10000;
          currentProcess = -1;
          
          int numberProcessesReady = 0;
          float cpudoneAvarage = 0;
          for(i = 0; i < size; ++i) {
            process = (sProcess) processVector.elementAt(i);
            if (process.cpudone < process.cputime && currentBlockTimes.elementAt(i) == 0) {
              numberProcessesReady++;
              cpudoneAvarage += process.cpudone;
            }
          }
          if(numberProcessesReady > 0 )
            cpudoneAvarage /= numberProcessesReady;

          for (i = 0; i < size; ++i) {
            process = (sProcess) processVector.elementAt(i);
            float processRatio = ratio(process, cpudoneAvarage, numberProcessesReady);
            if (process.cpudone < process.cputime && currentBlockTimes.elementAt(i) == 0 && processRatio < minratio) { 
              minratio = processRatio;
              currentProcess = i;
            }
          }
          if(currentProcess == -1) {
            if(!noWork) {
              noWork = true;
              out.println("Process: None... ");

            }
          }
          else {
            noWork = false;
            process = (sProcess) processVector.elementAt(currentProcess);
            out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.currentCPUTimeWorking + " " + process.cpudone + " " + process.priority + ")");
          }
        }  
        // If Blocked  
        if (currentProcess == -1 || process.currentCPUTimeWorking == process.worktime) {
          if(currentProcess != -1) {
            currentBlockTimes.setElementAt(process.blocktime ,currentProcess);
            out.println("Process: " + currentProcess + " blocked... (" + process.cputime + " " + process.currentCPUTimeWorking + " " + process.cpudone + " " + process.priority + ")");
            process.numblocked++;
            process.worktime = 0;
          }
          previousProcess = currentProcess;


          // Chosing process with minimal ratio
          float minratio = 10000;
          currentProcess = -1;

          int numberProcessesReady = 0;
          float cpudoneAvarage = 0;
          for(i = 0; i < size; ++i) {
            process = (sProcess) processVector.elementAt(i);
            if (process.cpudone < process.cputime && currentBlockTimes.elementAt(i) == 0) {
              numberProcessesReady++;
              cpudoneAvarage += process.cpudone;
            }
          }
          if(numberProcessesReady > 0 )
            cpudoneAvarage /= numberProcessesReady;

          for (i = 0; i < size; ++i) {
            if(i == previousProcess)
              continue;
            process = (sProcess) processVector.elementAt(i);
            float processRatio = ratio(process, cpudoneAvarage, numberProcessesReady);
            if (process.cpudone < process.cputime && currentBlockTimes.elementAt(i) == 0 && processRatio < minratio) { 
              minratio = process.cpudone;
              currentProcess = i;
            }
          }
          if(currentProcess == -1) {
            if(!noWork) {
              noWork = true;
              out.println("Process: None... ");

            }
          }
          else {
            noWork = false;
            process = (sProcess) processVector.elementAt(currentProcess);
            out.println("Process: " + currentProcess + " registered... (" + process.cputime + " " + process.currentCPUTimeWorking + " " + process.cpudone + " " + process.priority + ")");
            
            process.cpudone++;       
            if (process.currentCPUTimeWorking > 0) {
              process.worktime++;
            }
            
          }
        }
        if (currentProcess != -1) {
          process.cpudone++;       
          if (process.currentCPUTimeWorking > 0) {
            process.worktime++;
          }
        }
        for(int j = 0; j < currentBlockTimes.size(); ++j) {
          if(currentBlockTimes.elementAt(j) != 0) currentBlockTimes.setElementAt(currentBlockTimes.elementAt(j) - 1, j);
        }

        comptime++;
        sProcess process4 = (sProcess)processVector.elementAt(4);
        out.print(currentProcess);
        out.print(" ");
        out.println(process4.cpudone);
      }
      out.close();
    } catch (IOException e) { /* Handle exceptions */ }
    result.compuTime = comptime;
    return result;
  }


}
