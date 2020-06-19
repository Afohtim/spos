import java.util.Vector;
import java.util.Random;

public class WorkingSet {

    Vector<Page> pages;
    int t;

    public WorkingSet(Vector mem, int size) {
        pages = new Vector<>();

        t = 320;
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

        for(int i = 0; i < pages.size(); ++i) {
            if(pages.elementAt(i).lastTouchTime > t) {
                id = i;
                break;
            }
        }

        if(id == -1) {
            id = (new Random()).nextInt(pages.size());
        }

        Page pageToRemove = pages.remove(id);
        return pageToRemove;
    }
    
}