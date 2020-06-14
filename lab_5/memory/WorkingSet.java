import java.util.Vector;
import java.util.Random;

public class WorkingSet {

    Vector<Page> pages;
    int k;
    int t;

    public WorkingSet(Vector mem, int size) {
        pages = new Vector<>();
        k = size;

        for (int i = 0; i < size; ++i) {
            Page page = (Page) mem.elementAt(i);
            if (page.physical == -1)
                continue;
            addPage(page);
        }
    }

    public void addPage(Page page) {
        pages.add(page);
    }

    public Page getPageToRemove() {
        int id = -1;
        int time = -1;

        for(int i = 0; i < pages.size(); ++i) {
            if(pages.elementAt(i).lastTouchTime > t && pages.elementAt(i).lastTouchTime > time) {
                time = pages.elementAt(i).lastTouchTime;
                id = i;
            }
        }

        if(id == -1) {
            id = (new Random()).nextInt(k);
        }

        Page pageToRemove = pages.remove(id);
        return pageToRemove;
    }
    
}