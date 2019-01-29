#include <SIO_testing.h>
#include <SIO_testing_common.h>

// -- std headers
#include <SIO_stream.h>
#include <SIO_record.h>

using UnitTest = sio::test::UnitTest;
using SimpleBlock = sio::test::SimpleBlock;
using BlockWithPtr = sio::test::BlockWithPtr;

int main() {

  UnitTest unitTest("test_block_wr");
  unsigned int status = 0;
  const std::string fileName = "test_block_wr.sio";
  
  // prepare record and block
  auto record = std::make_shared<sio::record>( "SimpleRecord" );
  record->set_compression(true);
  record->add_block<SimpleBlock>();
  auto block = record->get_block_as<SimpleBlock>("SimpleBlock");
  unitTest.test( "block is not null", nullptr != block );
  
  // setup data
  block->data().m_integer = 42;
  block->data().m_float = 3.1415;
  block->data().m_double = 7854.458;
  
  // test the block with ptr references
  record->add_block<BlockWithPtr>();
  auto block_ptr = record->get_block_as<BlockWithPtr>("BlockWithPtr");
  unitTest.test( "block is not null 2", nullptr != block_ptr );
  
  for(unsigned int i=0 ; i<100 ; i++) {
    block_ptr->addData( i, i*0.5, i*i*45 );
  }
  
  unitTest.test( "validate block with pointers 1", block_ptr->validateData() );

  // prepare stream
  auto stream = std::make_shared<sio::stream>();
  stream->set_compression_level(5);
  status = stream->open( fileName, SIO_MODE_WRITE_NEW );
  
  unitTest.test( "stream opened", status == SIO_STREAM_SUCCESS );
  
  // write record
  auto writeResult = stream->write_record( record );
  unitTest.test( "write record", writeResult._status == SIO_RECORD_SUCCESS );

  // re-open stream in read mode
  status = stream->close();
  unitTest.test( "close stream", status == SIO_STREAM_SUCCESS );
  status = stream->open( fileName , SIO_MODE_READ );
  unitTest.test( "re-open stream", status == SIO_STREAM_SUCCESS );
  
  // prepare record and block for reading
  record = std::make_shared<sio::record>( "SimpleRecord" );
  record->add_block<SimpleBlock>();
  block = record->get_block_as<SimpleBlock>("SimpleBlock");
  unitTest.test( "block is not null", nullptr != block );
  record->add_block<BlockWithPtr>();
  block_ptr = record->get_block_as<BlockWithPtr>("BlockWithPtr");
  std::cout << "data size: " << block_ptr->data().size() << std::endl;
  std::cout << "ptrs size: " << block_ptr->ptrs().size() << std::endl;
  
  // read back record
  auto readResult = stream->read_next_record({{ record->name() , record }});
  std::cout << "data size: " << block_ptr->data().size() << std::endl;
  std::cout << "ptrs size: " << block_ptr->ptrs().size() << std::endl;
  unitTest.test( "read record", readResult._status == SIO_RECORD_SUCCESS );
  unitTest.test( "requested record", readResult._record == record );
  unitTest.test( "start of record", readResult._record_begin == writeResult._record_begin );
  unitTest.test( "end of record", readResult._record_end == writeResult._record_end );
  unitTest.test( "validate block with pointers 2", block_ptr->validateData() );
  
  // seek at start of record ...
  status = stream->seek(readResult._record_begin);
  unitTest.test( "seek start of record", status == SIO_STREAM_SUCCESS );
  // ... and read again !
  readResult = stream->read_next_record({{ record->name() , record }});
  unitTest.test( "read record 2", readResult._status == SIO_RECORD_SUCCESS );
  unitTest.test( "requested record 2", readResult._record == record );
  unitTest.test( "start of record 2", readResult._record_begin == writeResult._record_begin );
  unitTest.test( "end of record 2", readResult._record_end == writeResult._record_end );
  unitTest.test( "validate block with pointers 3", block_ptr->validateData() );
  
  status = stream->close();
  unitTest.test( "close stream last", status == SIO_STREAM_SUCCESS );
  
  return 0;
}