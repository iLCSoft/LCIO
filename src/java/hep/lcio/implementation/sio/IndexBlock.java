package hep.lcio.implementation.sio;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOInputStream;
import hep.io.sio.SIOOutputStream;
import hep.io.sio.SIORecord;
import hep.io.sio.SIOWriter;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;
import java.io.IOException;
import java.util.AbstractList;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

/**
 * An index block written into LCIO files to support random access
 * @author tonyj
 */
class IndexBlock {

    private static final String LCIOINDEX = "LCIOIndex";
    private static final int minorVersion = LCIO.MINORVERSION;
    private static final int majorVersion = LCIO.MAJORVERSION;
    private boolean recordsAreInOrder = true;
    private IndexEntry lastEntry;
    private IndexEntry minEntry;
    private IndexEntry maxEntry;
    private List<IndexEntry> index;
    private int maxEntries;
    private int nEvents;
    private int nRunHeaders;
    private long myLocation;

    IndexBlock(int size) {
        index = new ArrayList<IndexEntry>(size);
        maxEntries = size;
    }

    IndexBlock(SIORecord record) throws IOException {
        read(record);
    }

    boolean add(long recordLocation, LCRunHeader runHeader) {
        IndexEntry entry = new IndexEntry(recordLocation, runHeader);
        index.add(entry);
        checkOrder(entry);
        nRunHeaders++;
        return index.size() >= maxEntries;
    }

    boolean add(long recordLocation, LCEvent event) {
        IndexEntry entry = new IndexEntry(recordLocation, event);
        index.add(entry);
        checkOrder(entry);
        nEvents++;
        return index.size() >= maxEntries;
    }

    private void checkOrder(IndexEntry entry) {
        if (minEntry == null) {
            minEntry = maxEntry = entry;
        } else {
            if (minEntry.compareTo(entry) > 0) {
                minEntry = entry;
            }
            if (maxEntry.compareTo(entry) < 0) {
                maxEntry = entry;
            }
            if (lastEntry.compareTo(entry) > 0) {
                recordsAreInOrder = false;
            }
        }
        lastEntry = entry;
    }

    boolean isEmpty() {
        return index.isEmpty();
    }

    void clear() {
        index.clear();
        lastEntry = minEntry = maxEntry = null;
        recordsAreInOrder = true;
        nEvents = nRunHeaders = 0;
        myLocation = 0;
    }

    void write(SIOWriter writer) throws IOException {
        myLocation = writer.createRecord(LCIOINDEX, true);
        SIOOutputStream sio = writer.createBlock(LCIOINDEX, majorVersion, minorVersion);
        boolean oneRun = minEntry.getRun() == maxEntry.getRun();
        long firstLocation = index.get(0).recordLocation;
        boolean longOffset = lastEntry.recordLocation - firstLocation > Integer.MAX_VALUE;
        int controlWord = 0;
        if (oneRun) {
            controlWord |= 1;
        }
        if (longOffset) {
            controlWord |= 2;
        }
        sio.writeInt(controlWord);
        sio.writeInt(minEntry.getRun());
        sio.writeLong(firstLocation);
        sio.writeInt(index.size());
        for (IndexEntry entry : index) {
            if (!oneRun) {
                sio.writeInt(entry.getRun() - minEntry.getRun());
            }
            sio.writeInt(entry.getEvent());
            if (longOffset) {
                sio.writeLong(entry.recordLocation - firstLocation);
            } else {
                sio.writeInt((int) (entry.recordLocation - firstLocation));
            }
        }
        sio.close();
    }

    private void read(SIORecord record) throws IOException {
        SIOBlock block = record.getBlock();
        if (!block.getBlockName().equals(LCIOINDEX)) {
            throw new IOException("Unexpected block in LCIOIndex record");
        }
        SIOInputStream sio = block.getData();
        int controlWord = sio.readInt();
        boolean oneRun = (controlWord & 1) == 1;
        boolean longOffset = (controlWord & 2) == 1;
        int minRun = sio.readInt();
        long firstLocation = sio.readLong();
        int size = sio.readInt();
        index = new ArrayList<IndexEntry>(size);
        nEvents = 0;
        nRunHeaders = 0;
        maxEntries = size;
        for (int i = 0; i < size; i++) {
            int run = oneRun ? minRun : minRun + sio.readInt();
            int event = sio.readInt();
            if (event >= 0) nEvents++;
            else nRunHeaders++;
            long location = firstLocation + (longOffset ? sio.readLong() : sio.readInt());
            index.add(new IndexEntry(run, event, location));
        }
        //fg: this needs to be sorted as later a binary search is used
        //    in getLocation(RunEvent re)
        //  - in case this is not wanted, than a linear search would have
        //    to be used there 
        Collections.sort( index ) ;
        
        sio.close();
    }

    RunEvent getMinEntry() {
        return minEntry;
    }

    RunEvent getMaxEntry() {
        return maxEntry;
    }

    long getLocation() {
        return myLocation;
    }

    boolean areRecordsOrdered() {
        return recordsAreInOrder;
    }

    int getEventCount() {
        return nEvents;
    }

    int getRunHeaderCount() {
        return nRunHeaders;
    }

    long getLocation(RunEvent re) {
        int position = Collections.binarySearch(index, re);
        if (position < 0) {
            return -1;
        } else {
            return index.get(position).recordLocation;
        }
    }

    long getLocation(int position) {
        return index.get(position).recordLocation;
    }

    long findRecordHeader(long startPosition) {
        int startIndex = findIndexOfRecordLocation(startPosition);
        for (IndexEntry entry : index.subList(startIndex, index.size())) {
            if (entry.recordLocation > startPosition && entry.getEvent() == -1) {
                return entry.recordLocation;
            }
        }
        return -1;
    }

    long getFirstRecordLocation() {
        return index.isEmpty() ? 0 : index.get(0).recordLocation;
    }

    int findIndexOfRecordLocation(long recordLocation) {
        int position = Collections.binarySearch(new RecordLocationList(), recordLocation);
        if (position < 0) {
            position = Math.max(0, -position - 2);
        }
        return position;
    }

    int getRecordCount() {
        return nEvents+nRunHeaders;
    }

    boolean isEvent(int i) {
        return index.get(i).getEvent() >= 0;
    }

    private class RecordLocationList extends AbstractList<Long> {

        public Long get(int i) {
            return index.get(i).recordLocation;
        }

        public int size() {
            return index.size();
        }
    }

    private static class IndexEntry extends RunEvent {

        private long recordLocation;

        private IndexEntry(long recordLocation, LCRunHeader runHeader) {
            super(runHeader.getRunNumber(), -1);
            this.recordLocation = recordLocation;
        }

        private IndexEntry(long recordLocation, LCEvent eventHeader) {
            super(eventHeader.getRunNumber(), eventHeader.getEventNumber());
            this.recordLocation = recordLocation;
        }

        private IndexEntry(int run, int event, long location) {
            super(run, event);
            this.recordLocation = location;
        }
    }
}
