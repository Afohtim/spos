public class Process {
  public int cputime;
  public int untillBlock;
  public int cpudone;
  public int worktime;
  public int numblocked;

  public Process(int cputime, int untillBlock, int cpudone, int worktime, int numblocked) {
    this.cputime = cputime;
    this.untillBlock = untillBlock;
    this.cpudone = cpudone;
    this.worktime = worktime;
    this.numblocked = numblocked;
  } 	
}
