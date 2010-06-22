package hep.lcio.implementation.sio;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORecord;
import hep.io.sio.SIOWriter;
import hep.lcio.event.LCIO;
import java.io.IOException;

/**
 * A rewritable block written into a RandomAccessRecord in LCIO files
 * @author tonyj
 */
class RandomAccessBlock implements Comparable<RunEvent> {

    private static final String LCIORANDOMACCESS = "LCIORandomAccess";
    private static final int minorVersion = LCIO.MINORVERSION;
    private static final int majorVersion = LCIO.MAJORVERSION;

    public static final int LCRANDOMACCESSRECORDSIZE = 136 ;

    private static final RunEvent NOTSET = new RunEvent(0, 0);
    private RunEvent minRunEvent = NOTSET;
    private RunEvent maxRunEvent = NOTSET;
    private int nRunHeaders;
    private int nEvents;
    private boolean recordsAreOrdered = true;
    private long indexLocation;
    private long previousAccessBlockLocation;
    private long nextAccessBlockLocation;
    private long myLocation;
    private long firstRecordLocation;
    private SIOWriter writer;


    RandomAccessBlock() {
    }

    RandomAccessBlock(SIORecord record) throws IOException {
        read(record);
    }

    void write(SIOWriter writer) throws IOException {
        this.writer = writer;
        myLocation = writer.createRecord(LCIORANDOMACCESS, false);
        writeBlock(writer);
    }

    void flush() throws IOException {
        writer.rewriteRecord(myLocation, false);
        writeBlock(writer);
    }

    private void writeBlock(SIOWriter writer) throws IOException {
        SIOOutputStream sio = writer.createBlock(LCIORANDOMACCESS, majorVersion, minorVersion);
        sio.writeInt(minRunEvent.getRun());
        sio.writeInt(minRunEvent.getEvent());
        sio.writeInt(maxRunEvent.getRun());
        sio.writeInt(maxRunEvent.getEvent());
        sio.writeInt(nRunHeaders);
        sio.writeInt(nEvents);
        sio.writeInt(recordsAreOrdered ? 1 : 0);
        sio.writeLong(indexLocation);
        sio.writeLong(previousAccessBlockLocation);
        sio.writeLong(nextAccessBlockLocation);
        sio.writeLong(firstRecordLocation);

	// write the random access record size - with 0xabcd as 'check marker' in bits 31-16
	sio.writeInt( 0xabcd0000 | LCRANDOMACCESSRECORDSIZE ) ; 

        sio.close();
    }

    private void read(SIORecord record) throws IOException {
        SIOBlock block = record.getBlock();
        if (!block.getBlockName().equals(LCIORANDOMACCESS)) {
            throw new IOException("Unexpected LCIORandomAccess block");
        }
        SIOInputStream sio = block.getData();
        minRunEvent = new RunEvent(sio.readInt(), sio.readInt());
        maxRunEvent = new RunEvent(sio.readInt(), sio.readInt());
        nRunHeaders = sio.readInt();
        nEvents = sio.readInt();
        recordsAreOrdered = sio.readBoolean();
        sio.pad();
        indexLocation = sio.readLong();
        previousAccessBlockLocation = sio.readLong();
        nextAccessBlockLocation = sio.readLong();
        firstRecordLocation = sio.readLong();

	sio.readInt() ;   // read record size - ignored for now ...

        sio.close();
    }

    int getRecordCount() {
        return nRunHeaders + nEvents;
    }

    int getRunHeaderCount() {
        return nRunHeaders;
    }

    int getEventCount() {
        return nEvents;
    }

    long getIndexLocation() {
        return indexLocation;
    }

    void setIndexBlock(IndexBlock indexBlock) {
        minRunEvent = indexBlock.getMinEntry();
        maxRunEvent = indexBlock.getMaxEntry();
        recordsAreOrdered = indexBlock.areRecordsOrdered();
        nEvents = indexBlock.getEventCount();
        nRunHeaders = indexBlock.getRunHeaderCount();
        indexLocation = indexBlock.getLocation();
        firstRecordLocation = indexBlock.getFirstRecordLocation();
    }

    /**
     * Used to add a randomAccessBlock to the top level (file) random
     * access block.
     * @param randomAccessBlock
     */
    void add(RandomAccessBlock randomAccessBlock) {
        if (previousAccessBlockLocation == 0) {
            previousAccessBlockLocation = randomAccessBlock.myLocation;
        }
        nextAccessBlockLocation = randomAccessBlock.myLocation;
        recordsAreOrdered &= randomAccessBlock.recordsAreOrdered;
        if (minRunEvent == NOTSET) {
            minRunEvent = randomAccessBlock.minRunEvent;
            maxRunEvent = randomAccessBlock.maxRunEvent;
        } else {
            if (maxRunEvent.compareTo(randomAccessBlock.minRunEvent) > 0) {
                recordsAreOrdered = false;
            }
            if (minRunEvent.compareTo(randomAccessBlock.minRunEvent) > 0) {
                minRunEvent = randomAccessBlock.minRunEvent;
            }
            if (maxRunEvent.compareTo(randomAccessBlock.maxRunEvent) < 0) {
                maxRunEvent = randomAccessBlock.maxRunEvent;
            }
        }
        nEvents += randomAccessBlock.nEvents;
        nRunHeaders += randomAccessBlock.nRunHeaders;
    }

    void setPrevious(RandomAccessBlock previous) {
        this.previousAccessBlockLocation = previous.myLocation;
    }

    void setNext(RandomAccessBlock next) {
        this.nextAccessBlockLocation = next.myLocation;
    }

    long getLocation() {
        return myLocation;
    }

    boolean contains(RunEvent re) {
        return compareTo(re) == 0;
    }

    public int compareTo(RunEvent re) {
        if (minRunEvent.compareTo(re) > 0) {
            return 1;
        } else if (maxRunEvent.compareTo(re) < 0) {
            return -1;
        } else {
            return 0;
        }
    }

    long getPreviousLocation() {
        return previousAccessBlockLocation;
    }

    long getNextLocation() {
        return nextAccessBlockLocation;
    }

    long getFirstRecordLocation() {
        return firstRecordLocation;
    }
}
