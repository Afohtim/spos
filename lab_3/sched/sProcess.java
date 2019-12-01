public class sProcess {
  public int cputime;
  public int timeWorkingUntillBlock;
  public int cpudone;
  public int worktime;
  public int numblocked;
  public int priority;
  public int blocktime;

  public sProcess (int cputime, int timeWorkingUntillBlock, int cpudone, int worktime, int numblocked, int priority, int blocktime) {
    this.cputime = cputime;
    this.timeWorkingUntillBlock = timeWorkingUntillBlock;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
    this.priority = priority;
    this.blocktime = blocktime;
  } 	
}
