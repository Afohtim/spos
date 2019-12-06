public class sProcess {
  public int cputime;
  public int currentCPUTimeWorking;
  public int cpudone;
  public int worktime;
  public int numblocked;
  public int priority;
  public int blocktime;

  public sProcess (int cputime, int currentCPUTimeWorking, int cpudone, int worktime, int numblocked, int priority, int blocktime) {
    this.cputime = cputime;
    this.currentCPUTimeWorking = currentCPUTimeWorking;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
    this.priority = priority;
    this.blocktime = blocktime;
  } 	
}
