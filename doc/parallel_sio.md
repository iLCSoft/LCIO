

# Design document of parallel SIO implementation

## Scope of a parallel version of SIO

We want a simple IO package enabling explicit multi-threading by the user (no implicit multi-threading). While reading records from files, the following operations are done:

- reading the record size and the record buffer from the file (2 system calls).
    - input: the file handle (FILE*)
    - output: a raw buffer (char*)
- dealing with decompression (zlib).
    - input: a raw buffer (char*)
    - output: if the buffer is compressed, creates a new buffer (char*), else just points to the original buffer
- decoding the event using `blocks` decoder setup in the `record` object.
    - input: a raw buffer (char*)
    - output: a record object (LCEvent, LCRunHeader, ...)

In the current version of SIO, these 3 steps are done using the `sio::stream::read_next_record()` method, all in one step.

The idea of a parallel version of SIO would be to decoupled these steps with independent data structures (handlers) and functions. The user can then decide how to call them, either sequentially or in different threads. The first step has to remain the only sequential call since performing system calls in parallel might be inefficient and potentially not thread safe.

## Use cases

With the current version, reading a record from a file is performed in this way:

```cpp
// setup stream, record and blocks
auto stream = std::make_shared<sio::stream>();
auto status = stream->open( fileName , SIO_MODE_READ );
auto record = std::make_shared<sio::record>( "SimpleRecord" );
record->add_block<SimpleBlock>();

// here is the long IO operation: fread() + uncompressing + block xfer
auto readResult = stream->read_next_record({{ record->name() , record }});

// Here we get the unpacked data after reading
auto block = record->get_block_as<SimpleBlock>("SimpleBlock");
auto readData = block->data();
```

The idea of a parallel version of SIO would be to split the function `stream::read_next_record()` in many functions. Example:

```cpp
// setup stream, record and blocks as before
auto stream = std::make_shared<sio::stream>();
auto status = stream->open( fileName , SIO_MODE_READ );
auto record = std::make_shared<sio::record>( "SimpleRecord" );
record->add_block<SimpleBlock>();

// here is fread is called on the next record. 
// The readResult is a different object type than before.
// It handles the read buffer from the stream.
auto readResult = stream->read_next_record({{ record->name() , record }});

// The following lines are examples of how the decompression can be handled
// This is the standard case (synchronous) 
auto decompResult = readResult.decompress() ;
// or in a async call
auto decompResult = readResult.decompress( std::launch::async ) ;
// or in a deferred call
auto decompResult = readResult.decompress( std::launch::deferred ) ;

// Here is the step of block decoding.
// Depending on the launch policy specified above, the decode 
// function act like this:
//   - no argument: the buffer is already uncompressed
//   - async: decompResult stores a std::future and
//            we wait for decompression to finish before decoding
//   - deferred: the decompression starts when calling decode()
// The same strategy is adopted as for decompression.
// This is the standard case (synchronous) 
auto decodeResult = decompResult.decode() ;
// or in a async call
auto decodeResult = decompResult.decode( std::launch::async ) ;
// or in a deferred call
auto decodeResult = decompResult.decode( std::launch::deferred ) ;

// Here we get the unpacked data after reading.
// The same logic applies here. When calling result_as<T>,
// depending on the launch policy specified in the decode() function
// the following will happen:
//   - no arg: the result is directly acquired since the decoding was sequential
//   - async: the result is acquired when the parallel decoding task is finished
//   - the decoding is triggered on call (lazy mode) and the result returned on completion 
auto readData = decodeResult->result_as<BlockData>();
```

## API definition

Buffers are handled in a simple wrapper class `buffer_handler`:

```cpp
class buffer_handle {
public:
  buffer_handle() = default ;
  buffer_handle( const char* buf, std::size_t len ) ;
  
  const char* buffer() const ;
  std::size_t size() const ;
  const char &operator[](std::size_t index) const ;
  const char &operator[](std::size_t index) const ;
};
```

More generally, the stream class should provide a simple method for reading a record from the stream (file)

```
class stream {
public:
  // ...
  buffer_handle read_next_record();
};
```

The `buffer_handle` is a simple class handle a raw buffer (char*).
- read time of the record from stream (for statistics)
- the start and end positions of the record in the file
- compression info (compressed ?, compression level, etc...)

