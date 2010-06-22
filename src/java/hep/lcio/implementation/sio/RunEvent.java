package hep.lcio.implementation.sio;

/**
 *
 * @author tonyj
 */
class RunEvent implements Comparable<RunEvent> {

    private int run;
    private int event;

    RunEvent(int run, int event) {
        this.run = run;
        this.event = event;
    }

    public int compareTo(RunEvent other) {
        if (this.run != other.run) {
            return this.run - other.run;
        } else {
            return this.event - other.event;
        }
    }

    int getEvent() {
        return event;
    }

    int getRun() {
        return run;
    }

    @Override
    public String toString() {
        return String.format("Run: %d Event: %d",run,event);
    }

}
