public class Process {
  public int cputime;
  public int currentCPUTimeWorking;
  public int cpudone;
  public int worktime;
  public int numblocked;

  public Process(int cputime, int currentCPUTimeWorking, int cpudone, int worktime, int numblocked) {
    this.cputime = cputime;
    this.currentCPUTimeWorking = currentCPUTimeWorking;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
  } 	
}
