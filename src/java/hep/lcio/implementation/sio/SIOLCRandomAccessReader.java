package hep.lcio.implementation.sio;

import hep.io.sio.SIOBlock;
import hep.io.sio.SIOReader;
import hep.io.sio.SIORecord;
import hep.lcio.event.LCEvent;
import hep.lcio.event.LCIO;
import hep.lcio.event.LCRunHeader;
import java.io.EOFException;
import java.io.IOException;
import java.util.AbstractList;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 *
 * @author tonyj
 */
class SIOLCRandomAccessReader extends SIOLCReader {

    private RandomAccessSupport randomAccess;

    public void open(String filename) throws IOException {
        super.open(filename);
        // Peek at the last record to see if this file supports random access
        if (reader.isRandomAccess()) {
            try {
                //FIXME: Should not hardwire record length
                SIORecord record = reader.readRecord( - RandomAccessBlock.LCRANDOMACCESSRECORDSIZE );
                if ("LCIORandomAccess".equals(record.getRecordName())) {
                    randomAccess = new FileRandomAccessSupport(reader, record);
                }
            } catch (IOException x) {
                // OK, just assume random access is not supported
            } finally {
                reader.seek(0);
            }
        }
    }

    @Override
    public void open(String[] filenames) throws IOException {
        if (filenames.length == 1) {
            open(filenames[0]);
        } else {
            super.open(filenames);
            List<FileRandomAccessSupport> fileBlocks = new ArrayList<FileRandomAccessSupport>(filenames.length);
            for (String filename : filenames) {
                SIOReader reader = new SIOReader(filename);
                try {
                    SIORecord record = reader.readRecord( - RandomAccessBlock.LCRANDOMACCESSRECORDSIZE );
                    if (!"LCIORandomAccess".equals(record.getRecordName())) {
                        fileBlocks = null;
                        break;
                    } else {
                        fileBlocks.add(new FileRandomAccessSupport(reader, record));
                    }
                } finally {
                    //reader.close();
                }
            }
            if (fileBlocks != null) {
                randomAccess = new ChainRandomAccessSupport(fileBlocks);
            }
        }
    }

    public LCRunHeader readNextRunHeader(int accessMode) throws IOException {
        if (randomAccess != null) {
            long position = randomAccess.findNextRunHeader();
            if (position < 0) {
                return null;
            } else {
                SIORecord record = reader.readRecord(position);
                SIOBlock block = record.getBlock();
                int major = block.getMajorVersion();
                int minor = block.getMinorVersion();
                if ((major < 1) && (minor < 8)) {
                    throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");
                }

                // FIX ME: need to set access mode here....
                return new SIORunHeader(block.getData(), major, minor);
            }
        } else {
            return super.readNextRunHeader(accessMode);
        }
    }
    public LCRunHeader readRunHeader(int runNumber, int accessMode)
    throws IOException {
        if (randomAccess != null) {
            long position = randomAccess.findRunHeader(runNumber) ;
            if (position < 0) {
                throw new IOException(String.format("Run: %d not found", runNumber));
            }
            SIORecord record = reader.readRecord(position);
            SIOBlock block = record.getBlock();
            int major = block.getMajorVersion();
            int minor = block.getMinorVersion();
//            if ((major < 1) && (minor < 8)) {
//                throw new IOException("Sorry: files created with versions older than v00-08" + " are no longer supported !");
//            }

            // FIX ME: need to set access mode here....
            return new SIORunHeader(block.getData(), major, minor);
        } else {
            return super.readRunHeader(runNumber );
        }
    }

    public LCEvent readEvent(int runNumber, int evtNumber, int accessMode) throws IOException {
        if (randomAccess != null) {
            long position = randomAccess.findEvent(runNumber, evtNumber);
            if (position < 0) {
                throw new IOException(String.format("Run: %d Event: %d not found", runNumber, evtNumber));
            }
            SIORecord record = reader.readRecord(position);
            SIOEvent event = new SIOEvent(record, accessMode );
            event.readData(reader.readRecord());
            return event;
        } else {
            return super.readEvent(runNumber, evtNumber);
        }
    }

    @Override
    public void skipNEvents(int n) throws IOException {
        if (randomAccess != null) {
            randomAccess.skipNEvents(n);
        } else {
            super.skipNEvents(n);
        }
    }

    private interface RandomAccessSupport {

    	long findNextRunHeader() throws IOException;
    	
    	long findRunHeader(int runNum ) throws IOException;

        void skipNEvents(int n) throws IOException;

        long findEvent(int run, int event) throws IOException;
    }

    private class ChainRandomAccessSupport implements RandomAccessSupport {

        List<RandomAccessBlock> fileRandomAccessBlocks;
        List<FileRandomAccessSupport> fileBlocks;

        ChainRandomAccessSupport(List<FileRandomAccessSupport> fileBlocks) {
            this.fileBlocks = fileBlocks;
            fileRandomAccessBlocks = new ArrayList<RandomAccessBlock>(fileBlocks.size());
            for (FileRandomAccessSupport fra : fileBlocks) {
                fileRandomAccessBlocks.add(fra.fileRandomAccessBlock);
            }
        }

        public long findNextRunHeader() throws IOException {
            throw new UnsupportedOperationException("Not supported yet.");
        }

        public void skipNEvents(int n) throws IOException {
            throw new UnsupportedOperationException("Not supported yet.");
        }

        public long findEvent(int run, int event) throws IOException {
            RunEvent re = new RunEvent(run, event);

//            RandomAccessBlock fab = findFileRandomAccessBlock();
//            if (!fab.contains(re)) {
//                return -1;
//            }

            int location = Collections.binarySearch(fileRandomAccessBlocks, re);
            reader = fileBlocks.get(location).reader;
            return fileBlocks.get(location).findEvent(run, event);
        }

		public long findRunHeader(int runNum) throws IOException {
			
            RunEvent re = new RunEvent(runNum, -1 );

          int location = Collections.binarySearch(fileRandomAccessBlocks, re);
          reader = fileBlocks.get(location).reader;
          return fileBlocks.get(location).findRunHeader(runNum);
		}
    }

    private static class FileRandomAccessSupport implements RandomAccessSupport {

        private RandomAccessBlock fileRandomAccessBlock;
        private List<RandomAccessBlock> indexRandomAccessBlocks = new ArrayList<RandomAccessBlock>();
        private boolean indexBlocksRead;
        private SIOReader reader;
        //FIXME: This should be a softHashMap
        private Map<RandomAccessBlock, IndexBlock> indexHash = new HashMap<RandomAccessBlock, IndexBlock>();

        FileRandomAccessSupport(SIOReader reader, SIORecord record) throws IOException {
            this.reader = reader;
            fileRandomAccessBlock = new RandomAccessBlock(record);
        }

        public long findRunHeader(int runNum) throws IOException {
            RunEvent re = new RunEvent( runNum, -1 );

            RandomAccessBlock fab = findFileRandomAccessBlock();
            if (!fab.contains(re)) {
                return -1;
            }

            List<RandomAccessBlock> iab = findIndexRandomAccessBlocks();
            int location = Collections.binarySearch(iab, re);
            IndexBlock ib = findIndexBlock(iab.get(location));
            return ib.getLocation(re);
		}

		public long findNextRunHeader() throws IOException {
            List<RandomAccessBlock> iab = findIndexRandomAccessBlocks();
            int iabIndex = findIndexOfRandomAccessBlockContaining(reader.getNextRecordPosition());
            for (RandomAccessBlock rab : iab.subList(iabIndex, iab.size())) {
                if (rab.getRunHeaderCount() > 0) {
                    IndexBlock ib = findIndexBlock(rab);
                    long position = ib.findRecordHeader(reader.getNextRecordPosition());
                    if (position >= 0) {
                        return position;
                    }
                }
            }
            return -1;
        }

        public void skipNEvents(int n) throws IOException {
            if (n == 0) {
                return;
            }
            int iabIndex = findIndexOfRandomAccessBlockContaining(reader.getNextRecordPosition());
            List<RandomAccessBlock> iab = findIndexRandomAccessBlocks();
            IndexBlock ib = findIndexBlock(iab.get(iabIndex));
            // Find how many events are left in current block.
            int recordIndex = ib.findIndexOfRecordLocation(reader.getNextRecordPosition());
            for (int i = recordIndex; i < ib.getRecordCount(); i++) {
                if (ib.isEvent(i)) {
                    if (n-- == 0) {
                        reader.seek(ib.getLocation(i));
                        return;
                    }
                }
            }

            for (RandomAccessBlock rab : iab.subList(iabIndex + 1, iab.size())) {
                ib = findIndexBlock(rab);
                int nEvents = ib.getEventCount();
                if (nEvents <= n) {
                    n -= nEvents;
                } else {
                    for (int i = 0; i < ib.getRecordCount(); i++) {
                        if (ib.isEvent(i)) {
                            if (n-- == 0) {
                                reader.seek(ib.getLocation(i));
                                return;
                            }
                        }
                    }
                }
            }
            throw new EOFException();
        }

        public long findEvent(int run, int event) throws IOException {
            RunEvent re = new RunEvent(run, event);

            RandomAccessBlock fab = findFileRandomAccessBlock();
            if (!fab.contains(re)) {
                return -1;
            }

            List<RandomAccessBlock> iab = findIndexRandomAccessBlocks();
            int location = Collections.binarySearch(iab, re);
            IndexBlock ib = findIndexBlock(iab.get(location));
            return ib.getLocation(re);
        }

        private RandomAccessBlock findFileRandomAccessBlock() throws IOException {
            return fileRandomAccessBlock;
        }

        private List<RandomAccessBlock> findIndexRandomAccessBlocks() throws IOException {
            if (!indexBlocksRead) {
                long originalPosition = reader.getNextRecordPosition();
                RandomAccessBlock fab = findFileRandomAccessBlock();
                //NB: We read the records in reverse order since c++ implementation only
                // fills the backward pointers.
                if (indexRandomAccessBlocks.isEmpty()) {
                    SIORecord record = reader.readRecord(fab.getNextLocation());
                    RandomAccessBlock rab = new RandomAccessBlock(record);
                    indexRandomAccessBlocks.add(0, rab);
                    indexBlocksRead = rab.getPreviousLocation() == 0;
                }
                while (!indexBlocksRead) {
                    long previousLocation = indexRandomAccessBlocks.get(0).getPreviousLocation();
                    SIORecord record = reader.readRecord(previousLocation);
                    RandomAccessBlock rab = new RandomAccessBlock(record);
                    indexRandomAccessBlocks.add(0, rab);
                    indexBlocksRead = rab.getPreviousLocation() == 0;
                }
                reader.seek(originalPosition);
            }
            return indexRandomAccessBlocks;
        }

        private IndexBlock findIndexBlock(RandomAccessBlock rab) throws IOException {
            IndexBlock result = indexHash.get(rab);
            if (result == null) {
                long originalPosition = reader.getNextRecordPosition();
                result = new IndexBlock(reader.readRecord(rab.getIndexLocation()));
                indexHash.put(rab, result);
                reader.seek(originalPosition);
            }
            return result;
        }

        private int findIndexOfRandomAccessBlockContaining(long recordLocation) throws IOException {
            int position = Collections.binarySearch(new RecordLocationList(), recordLocation);
            if (position < 0) {
                position = Math.max(0, -position - 2);
            }
            return position;
        }

        private class RecordLocationList extends AbstractList<Long> {

            List<RandomAccessBlock> iab;

            RecordLocationList() throws IOException {
                iab = findIndexRandomAccessBlocks();
            }

            @Override
            public Long get(int index) {
                return iab.get(index).getFirstRecordLocation();
            }

            @Override
            public int size() {
                return iab.size();
            }
        }
    }
}
