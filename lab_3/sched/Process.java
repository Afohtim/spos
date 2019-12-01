public class Process {
  public int cputime;
  public int timeWorkingUntillBlock;
  public int cpudone;
  public int worktime;
  public int numblocked;

  public Process(int cputime, int timeWorkingUntillBlock, int cpudone, int worktime, int numblocked) {
    this.cputime = cputime;
    this.timeWorkingUntillBlock = timeWorkingUntillBlock;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
  } 	
}
