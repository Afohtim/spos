public class sProcess {
  public int cputime;
  public int untillBlock;
  public int cpudone;
  public int worktime;
  public int numblocked;
  public int priority;
  public int blocktime;
  public int numrescheduled;

  public sProcess (int cputime, int untillBlock, int cpudone, int worktime, int numblocked, int priority, int blocktime) {
    this.cputime = cputime;
    this.untillBlock = untillBlock;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
    this.priority = priority;
    this.blocktime = blocktime;
    this.numrescheduled = 0;
  } 	
}
