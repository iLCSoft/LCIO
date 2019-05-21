

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

The same applies for writing. In the current version of SIO, these 3 steps are done using the `sio::stream::read_next_record()` method, all in one step.

The idea of a parallel version of SIO would be to decoupled these steps with independent data structures (handlers) and functions. The user can then decide how to call them, either sequentially or in different threads. The actual reading (writing) operation from (to) file can **possibly** done using a mutex to enforce thread-safety.

## Example use cases

### Record reading/writing: sequential, deferred, async modes

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

The idea of a parallel version of SIO would be to split the function `stream::read_next_record()` in many functions. Each step of the record reading can be performed in three modes:
- `sequential`: the operation is performed directly on call
- `deferred` or `lazy`: the operation returns directly a handle on the result. When the user calls `handle.get()`, the operation is triggered.
- `async`: an asynchronous task is launched and the operation directly returns a handle on the result to acquire later on.

The record and stream are setup as for the current version:

```cpp
// setup stream, record and blocks as before
auto stream = std::make_shared<sio::stream>();
auto status = stream->open( fileName , SIO_MODE_READ );
auto record = std::make_shared<sio::record>( "SimpleRecord" );
record->add_block<SimpleBlock>();
```

The following illustrates how to extract a record from the stream, but how to unpack it step by step.

We first extract the record buffer from the stream:

```cpp
// Here fread is called on the next record.
// The readResult the read buffer from the stream.
auto readResult = stream->read_next_record();
```
The following lines show how the decompression can be handled:

```cpp
// This is the standard case (synchronous)
auto decompResult = readResult.decompress() ;
// or in a async call
auto decompResult = readResult.decompress( std::launch::async ) ;
// or in a deferred call
auto decompResult = readResult.decompress( std::launch::deferred ) ;
```

The blocks decoding is the next step. Here, as for the decompression, different launch policy can be used:

```cpp
// Depending on the launch policy specified above, the decode
// function act like this:
//   - no argument: the buffer is already uncompressed
//   - async: decompResult stores a std::future and
//            we wait for decompression to finish before decoding
//   - deferred: the decompression starts when calling decode()
// This is the standard case (synchronous)
auto decodeResult = decompResult.decode() ;
// or in a async call
auto decodeResult = decompResult.decode( std::launch::async ) ;
// or in a deferred call
auto decodeResult = decompResult.decode( std::launch::deferred ) ;
```

Here the `decodeResult` contains the final record product. The result can be accessed with simple getter, but the depending on the launch policy passed in the `decode()` method, the decoding operation can be triggered when using the getter (lazy triggering) and also maybe in a task.

```cpp
auto readData = decodeResult->result_as<BlockData>();
```

For writing, the same mechanism should be provided:
- encoding of input record object to a buffer (sequential, deferred or async)
- compression to buffer (sequential, deferred or async)
- writing to ouput stream (file, buffer, socket)

For both reading and writing these operation can also be combined in a smart way. For example, for reading, the decompression and decoding can be performed asynchronously in the same task and not in two steps.

## Other functionalities

- The stream must be transparent in terms of input/output source (raw buffer, file, socket). The API should foresee either an abstract interface or a template implementation to support the different stream types. Example of possibly supported types:
    - File streams
    - Raw buffer streams
    - Shared memory streams
    - Socket streams
