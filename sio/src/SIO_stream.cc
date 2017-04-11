// ----------------------------------------------------------------------------
// CVS $Id: SIO_stream.cc,v 1.9 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for a single SIO stream.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_stream manages operations of a single SIO stream.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "zlib.h"

#include "SIO_block.h"
#include "SIO_definitions.h"
#include "SIO_functions.h"
#include "SIO_record.h"
#include "SIO_recordManager.h"
#include "SIO_stream.h"



static unsigned int
    SIO_align       = 0x00000003,
    SIO_mark_record = 0xabadcafe;

// ----------------------------------------------------------------------------
// Constructor (private function!)   
// ----------------------------------------------------------------------------
SIO_stream::SIO_stream
(
    const char*      i_name,
    unsigned int     i_reserve,
    SIO_verbosity    i_verbosity
 ) : name( i_name ),
	 reserve( i_reserve ),
	 verbosity( i_verbosity ),
	 compLevel(Z_DEFAULT_COMPRESSION) 
{}

// ----------------------------------------------------------------------------
// Destructor (private function!)
// ----------------------------------------------------------------------------
SIO_stream::~SIO_stream()
{

//
// Always try to close the stream.
//
if( state != SIO_STATE_CLOSED )
    close();

//
// That's all folks!
//
return;
}

// ----------------------------------------------------------------------------
// Flush the stream.
// ----------------------------------------------------------------------------
unsigned int SIO_stream::flush()
{
//
// Can't close what ain't open!
//
if( state == SIO_STATE_CLOSED )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Not open"
                  << std::endl;
    }
    return( SIO_STREAM_NOTOPEN );
}
int rc = FFLUSH( handle );
return (rc == 0) ? SIO_STREAM_SUCCESS : SIO_STREAM_BADWRITE;
}

// ----------------------------------------------------------------------------
// Close the stream.
// ----------------------------------------------------------------------------
unsigned int SIO_stream::close()
{

//
// Local variables.
//
int
    z_stat;

unsigned int
    status;

//
// Can't close what ain't open!
//
if( state == SIO_STATE_CLOSED )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Not open"
                  << std::endl;
    }
    return( SIO_STREAM_NOTOPEN );
}

//
// Assume all will go well.
//
status = SIO_STREAM_SUCCESS;

//
// Dispose of the pointer relocation tables.
//
delete pointedAt;
delete pointerTo;

//
// Dispose of compression control structure.
//
if( z_strm != NULL )
{
    z_stat = (mode == SIO_MODE_READ) ? inflateEnd( z_strm )
                                     : deflateEnd( z_strm );

    if( z_stat != Z_OK )
    {
        status = SIO_STREAM_BADCOMPRESS;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "//] "
                      << "ZLIB error number " << z_stat
                      << std::endl;

            std::cout << "SIO: ["  << name << "//] "
                      << "Compression de-initialization failed"
                      << std::endl;
        }
    }
    free( z_strm );
    z_strm = NULL;
}
    
//
// Dispose of any compression/decompression buffer.
//
if( cmploc != NULL )
{
    free( cmploc );
    cmploc = NULL;
    cmpmax = NULL;
}

//
// Dispose of any associated raw data buffer.
//
if( bufloc != NULL )
{
    free( bufloc );
    bufloc = NULL;
    buffer = NULL;
    bufmax = NULL;
    recmax = NULL;
    blkmax = NULL;
}

//
// Close the stream and destroy associated information.
//
if( FCLOSE( handle ) == EOF )
{
    status = SIO_STREAM_GOTEOF;
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "EOF on close" 
                  << std::endl;
    }
}
else
{
}
filename.erase( filename.begin(), filename.end() );
handle = NULL;

//
// Miscellany.
//
mode   = SIO_MODE_UNDEFINED;
state  = SIO_STATE_CLOSED;

//
// Summarize.
//
if( status == SIO_STREAM_SUCCESS )
{
    if( verbosity >= SIO_ALL )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Closed"
                  << std::endl;
    }
}

//
// That's all folks!
//
return( status );
}

// ----------------------------------------------------------------------------
// Dump the buffer.  
// ----------------------------------------------------------------------------
void SIO_stream::dump
(
    unsigned int    offset,
    unsigned int    length
)
{

//
// Local variables.
//
unsigned int
    count;

char
   *ascpnt,
   *hexpnt,
    outbuf[77];

unsigned char
   *dmpbeg,
   *dmpend;

static char
    hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7',
                '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

//
// Header.
//
printf( "\nDump buffer of stream %s\n\n", name.c_str() );

if( buffer == NULL )
{
    printf( "No buffer associated with stream %s\n", name.c_str() );
    return;
}

//
// Offset must be in bounds.
//
dmpbeg = bufloc + offset;
if( dmpbeg >= bufmax )
{
    printf( "Offset beyond end of buffer\n");
    return;
}

//
// Length must not run off end of buffer.
//
dmpend = dmpbeg + length;
if( dmpend > bufmax )
    dmpend = bufmax;

//
// Buffer characteristics.
//
 printf("   Start address: %8ld  (%p)\n", (long int)bufloc, (void*) bufloc );
count = buffer - bufloc;
printf(" Current address: %8ld  (%p)    Offset: %8d  (0x%08x)\n", 
        (long int)buffer, (void*) buffer, count, count );
count = bufmax - bufloc;
printf("     End address: %8ld  (%p)    Offset: %8d  (0x%08x)\n",
        (long int)bufmax,  (void*) bufmax, count, count );

//
// Put out a header.
//
printf("\n --Offset:Address-");
printf(  "    -Hex------------------------------");
printf(  "    -ASCII----------\n");

// dddddddd:xxxxxxxx    xxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx    aaaaaaaaaaaaaaaa
//-123456789-123456789-123456789-123456789-123456789-123456789-123456789-123456

//
// Scan down the buffer, printing a dump record every 16 bytes.
//
memset(  &outbuf[ 0], ' ', sizeof( outbuf ) );
sprintf( &outbuf[ 1], "%8d",  offset );
sprintf( &outbuf[10], "%p",  (void*) dmpbeg );
outbuf[ 9] = ':';
outbuf[18] = ' ';
outbuf[76] = '\0';
hexpnt     = &outbuf[22];
ascpnt     = &outbuf[60];
count      = 0;

while( dmpbeg < dmpend )
{

    *hexpnt++ = hex[ ((*dmpbeg >>   4) & 15) ]; 
    *hexpnt++ = hex[ ( *dmpbeg         & 15) ];
    if( (count & 0x3) == 3 ) hexpnt++;

    if( isprint( *dmpbeg ) ) *ascpnt++ = *dmpbeg;
    else                     *ascpnt++ = '.';

    count++;
    offset++;
    dmpbeg++;

    if( count == 16 )
    {
        printf( "%s\n", outbuf );

        memset(  &outbuf[ 0], ' ', sizeof( outbuf ) );
        sprintf( &outbuf[ 1], "%8d",  offset );
        sprintf( &outbuf[10], "%p",  (void*) dmpbeg );
        outbuf[ 9] = ':';
        outbuf[18] = ' ';
        outbuf[76] = '\0';
        hexpnt     = &outbuf[22];
        ascpnt     = &outbuf[60];
        count      = 0;
    }
}

if( count != 0 )
    printf( "%s\n", outbuf );

//
// That's all folks!
//
return;
}

// ----------------------------------------------------------------------------
// Return pointer to stream name.
// ----------------------------------------------------------------------------
std::string* SIO_stream::getName()         { return( &name ); }

// ----------------------------------------------------------------------------
// Return pointer to filename.
// ----------------------------------------------------------------------------
std::string* SIO_stream::getFilename()     { return( &filename ); }

// ----------------------------------------------------------------------------
// Return mode.
// ----------------------------------------------------------------------------
SIO_stream_mode SIO_stream::getMode()      { return( mode ); }

// ----------------------------------------------------------------------------
// Return state.
// ----------------------------------------------------------------------------
SIO_stream_state SIO_stream::getState()    { return( state ); }

// ----------------------------------------------------------------------------
// Return verbosity.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_stream::getVerbosity()   { return( verbosity ); }


// ----------------------------------------------------------------------------
// Set compresision level
// ----------------------------------------------------------------------------
void SIO_stream::setCompressionLevel(int level) { 
  
  // zlib knows comp levels:  -1/Z_DEFAULT_COMPRESSION, 1-9
  if( level < 0 ) 

    compLevel = Z_DEFAULT_COMPRESSION ;

  else if( level > 9 )
    
    compLevel = 9  ;
 
  else 

    compLevel = level ;

}



// ----------------------------------------------------------------------------
// Associate a file name and a mode with this stream and open the file.
// ----------------------------------------------------------------------------
unsigned int SIO_stream::open
(
    const char*      i_filename,
    SIO_stream_mode  i_mode
)
{

//
// Local variables.
//
int
    z_stat;

static char
  SIO_filemode[4][3] = { "rb", "wb", "ab","r+" };

//
// Can't open what ain't closed!
//
if( state == SIO_STATE_OPEN || state == SIO_STATE_ERROR )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Already open"
                  << std::endl;
    }
    return( SIO_STREAM_ALREADYOPEN );
}

//
// Can't open in mode undefined.
//
 if( i_mode == SIO_MODE_UNDEFINED )
   {
     if( verbosity >= SIO_ERRORS )
       {
	 std::cout << "SIO: ["  << name << "//] "
		   << "Cannot open in mode SIO_MODE_UNDEFINED"
		   << std::endl;
       }
     return( SIO_STREAM_BADMODE );
   }
 
//
// Open the file.
//
if( (handle = FOPEN( i_filename, SIO_filemode[i_mode] )) == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Cannot open file "
                  << i_filename
                  << std::endl;
    }
    return( SIO_STREAM_OPENFAIL );
}

//
// Open succeeded.  Save the context.
//
filename = i_filename;
mode     = i_mode;
state    = SIO_STATE_OPEN;

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: ["  << name << "//] "
              << "Opened file " 
              << i_filename
              << std::endl;
}

//
// Allocate a raw data buffer to go with the stream.
//
buffer = NULL;
bufmax = NULL;
bufloc = static_cast<unsigned char*>(malloc( reserve ));
if( bufloc == NULL )
{
    state = SIO_STATE_ERROR;
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Buffer allocation failed"
                  << std::endl;
    }
    return( SIO_STREAM_NOALLOC );
}
buffer = bufloc;
bufmax = bufloc + reserve;

//
// Allocate a compression/decompression buffer and initialize it.
//
cmploc = NULL;
cmpmax = NULL;
cmploc = static_cast<unsigned char*>(malloc( reserve >> 2 ));
if( cmploc == NULL )
{
    state = SIO_STATE_ERROR;
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Compression buffer allocation failed"
                  << std::endl;
    }
    return( SIO_STREAM_BADCOMPRESS );
}
cmpmax = cmploc + (reserve >> 2);

//
// Allocate the z_stream structure and initialize it.
//
z_strm = static_cast<z_stream*>( malloc( sizeof( z_stream ) ));

z_strm->zalloc = Z_NULL;
z_strm->zfree  = Z_NULL;
z_strm->opaque = 0;

z_stat = (mode == SIO_MODE_READ) ? inflateInit( z_strm )
                                 : deflateInit( z_strm, compLevel );

if( z_stat != Z_OK )
{
    state = SIO_STATE_ERROR;
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "ZLIB error number " << z_stat
                  << std::endl;

        std::cout << "SIO: ["  << name << "//] "
                  << "Compression initialization failed"
                  << std::endl;
    }
    return( SIO_STREAM_BADCOMPRESS );
}
    
//
// Allocate the pointer relocation tables.
//
pointedAt = new pointedAtMap_c;
pointerTo = new pointerToMap_c;

//
// That's all folks!
//
return( SIO_STREAM_SUCCESS );
}


//fg: need ftell for direct access 
SIO_64BITINT SIO_stream::currentPosition() { 

  return  FTELL( handle ) ; 
}

//
//fg: add method to position the file pointer
//
unsigned int SIO_stream::seek(SIO_64BITINT pos, int whence) {  
  
  unsigned int status;
  
  //fg:  also need the seek in 'append' mode ( delete old file record )
//   //
//   // This must be a readable stream!
//   //
//   if( mode != SIO_MODE_READ ) {
    
//     //    if( verbosity >= SIO_ERRORS ) {
//     if( true ) {
      
//       std::cout << "SIO: ["  << name << "//] "
// 		<< "Cannot seek (stream is write only)"
// 		<< std::endl;
//     }
//     return( SIO_STREAM_WRITEONLY );
//   }

  status = FSEEK( handle, pos , whence )  ;
  
  if( status != 0 ) {
    
    state = SIO_STATE_ERROR;
    
    if( verbosity >= SIO_ERRORS ) {
      
      std::cout << "SIO: ["  << name << "/] "
		<< "Failed seeking position" << pos
		<< std::endl;
    }
    return( SIO_STREAM_EOF );
  }
  
//   //FIXME: debug...
//   std::cout << " SIO_stream::seek( " << pos << ", " << whence << " ) - ftell : " << FTELL( handle ) 
// 	    << " stream-state: " << state << std::endl ;

  return( SIO_STREAM_SUCCESS );
}

unsigned int SIO_stream::reset(SIO_64BITINT pos){

  unsigned int status;

  status = seek( pos ) ;

  if( status == SIO_STREAM_SUCCESS ){

    // if we can seek the file  the stream should be fine ...
    state = SIO_STATE_OPEN ;
  }

  return status ;
}


// ----------------------------------------------------------------------------
// Read the next record.
// ----------------------------------------------------------------------------
unsigned int SIO_stream::read
(
    SIO_record**    record
)
{

int
    z_stat;

unsigned int
    data_length,
    head_length,
    name_length,
    ucmp_length,
    buftyp,
    compress,
    padlen,
    options,
    status;

char
   *tmploc;

bool
    requested;

//
// Initialize the returned record pointer to something nasty.
//
*record = NULL;

 recPos = -1 ;  
 SIO_64BITINT  recStart = -1 ;

//
// The stream must be open!
//
if( state != SIO_STATE_OPEN )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Cannot read (stream is not open)"
                  << std::endl;
    }
    return( SIO_STREAM_NOTOPEN );
}

//
// This must be a readable stream!
//
if( mode != SIO_MODE_READ )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Cannot read (stream is write only)"
                  << std::endl;
    }
    return( SIO_STREAM_WRITEONLY );
}

//
// Loop over records until a requested one turns up.
//
requested = false;
while( requested == false )
{

  recStart = FTELL( handle ) ;

    //
    // Initialize the buffer and read the first eight bytes.  A read failure
    // at this point is treated as an end-of-file (even if there are a few
    // bytes dangling in the file).
    //
    buffer = bufloc;
    status = FREAD( buffer, SIO_LEN_SB, 8, handle );
    if( status < 8 )
        return( SIO_STREAM_EOF );
 
    //
    // Interpret: 1) The length of the record header.
    //            2) The record marker.
    //
    blkmax = bufloc + 8;
    SIO_DATA( this, &head_length,  1 );
    SIO_DATA( this, &buftyp,       1 );
    if( buftyp != SIO_mark_record )
    {
        state = SIO_STATE_ERROR;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "//] "
                      << "Expected record marker not found"
                      << std::endl;
        }
        return( SIO_STREAM_NORECMARKER );
    }

    //
    // Interpret: 3) The options word.
    //            4) The length of the record data (compressed).
    //            5) The length of the record name (uncompressed).
    //            6) The length of the record name.
    //            7) The record name.
    //
    status = FREAD( buffer, SIO_LEN_SB, (head_length - 8), handle );
    if( status < (head_length - 8) )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "//] "
                      << "Unexpected EOF reading record header"
                      << std::endl;
        }
        return( SIO_STREAM_EOF );
    }

    blkmax = bufloc + head_length;
    SIO_DATA( this, &options,      1 );
    SIO_DATA( this, &data_length,  1 );
    SIO_DATA( this, &ucmp_length,  1 );
    SIO_DATA( this, &name_length,  1 );

    tmploc = static_cast<char*>(malloc( name_length + 1 ));
    if( tmploc == NULL )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "//] "
                      << "Buffer allocation failed"
                      << std::endl;
        }
        return( SIO_STREAM_NOALLOC );
    }

    //    SIO_DATA( this, tmploc, name_length );
    //fg: keep coverity happy be deleting the temp memory in case of a read error
    status = SIO_functions::data(  this, tmploc, name_length );
    if( !(status & 1) ) {
      free( tmploc );
      return status;
    }

    tmploc[name_length]  = '\0';
    *record              = SIO_recordManager::get( tmploc );
    rec_name             = tmploc;
    free( tmploc );

    if( verbosity >= SIO_ALL )
    {
        std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                  << "Record header read"
                  << std::endl;
    }

    //
    // Unpack this record?
    //
    if( *record == NULL )
    {
        if( verbosity >= SIO_ALL )
	{
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                      << "Ignored (name not recognized)"
                      << std::endl;
        }
    }

    else if( !(*record)->getUnpack() )
    {
        if( verbosity >= SIO_ALL )
	{
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                      << "Ignored (unpacking not requested)"
                      << std::endl;
        }
    }

    else
    {
        requested = true;
    }

    //
    // If the record's not interesting, move on.  Remember to skip over any
    // padding bytes inserted to make the next record header start on a
    // four byte boundary in the file.
    //
    if( requested == false )
    {
        data_length += (4 - (data_length & SIO_align)) & SIO_align;
        status = FSEEK( handle, data_length, 1 );
        if( status != 0 )
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Failed skipping record"
                          << std::endl;
            }
            return( SIO_STREAM_EOF );
        }
        continue;
    }

    //
    // Ensure sufficient buffering for the uncompressed record.
    //
    if( (head_length + ucmp_length) >= (bufmax - bufloc) )
    {
        unsigned int
            newlen;

        unsigned char
           *newbuf;

        newlen = head_length + ucmp_length;
        newbuf = static_cast<unsigned char*>(malloc( newlen )); 
        if( newbuf == NULL )
        {
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Uncompressed buffer allocation failed"
                          << std::endl;
            }
            return( SIO_STREAM_NOALLOC );
        }

        memcpy( newbuf, bufloc, head_length );
        free( bufloc );
        bufloc = newbuf;
        buffer = newbuf + head_length;
        bufmax = bufloc + newlen;
    }

    //
    // Extract the compression bit from the options word.
    //
    compress = options & SIO_OPT_COMPRESS;

    if( !compress )
    {
        //
        // Read the rest of the record data.  Note that uncompressed data is
        // -always- aligned to a four byte boundary in the file, so no pad
        // skipping is necessary,
        //
        status = FREAD( buffer, SIO_LEN_SB, data_length, handle );
        if( status < data_length )
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Failed reading uncompressed record data"
                          << std::endl;
            }
            return( SIO_STREAM_EOF );
        }
    }

    else
    {
        //
        // Ensure sufficient buffering for the compressed record.
        //
        if( data_length >= (cmpmax - cmploc) )
        {
            unsigned char
               *newbuf;

            newbuf = static_cast<unsigned char*>(malloc( data_length )); 
            if( newbuf == NULL )
            {
                if( verbosity >= SIO_ERRORS )
                {
                    std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                              << "Compressed buffer allocation failed"
                              << std::endl;
                }
                return( SIO_STREAM_NOALLOC );
            }

            free( cmploc );
            cmploc = newbuf;
            cmpmax = cmploc + data_length;
        }

        //
        // Read the compressed record data.
        //
        status = FREAD( cmploc, SIO_LEN_SB, data_length, handle );
        if( status < data_length )
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Failed reading compressed record data"
                          << std::endl;
            }
            return( SIO_STREAM_EOF );
        }

        //
        // Skip the read pointer over any padding bytes that may have been
        // inserted to make the next record header start on a four byte
        // boundary in the file.
        //
        padlen = (4 - (data_length & SIO_align)) & SIO_align;
        if( padlen > 0 )
        {
            status = FSEEK( handle, padlen, 1 );
            if( status != 0 )
            {
                state = SIO_STATE_ERROR;
                if( verbosity >= SIO_ERRORS )
                {
                    std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                              << "Failed reading end-of-record pad data"
                              << std::endl;
                }
                return( SIO_STREAM_EOF );
            }
        }

        //
        // Set up for the decompression.
        //
        z_strm->next_in   = cmploc;
        z_strm->avail_in  = data_length;
        z_strm->total_in  = 0;

        z_strm->next_out  = buffer;
        z_strm->avail_out = bufmax-buffer;
        z_strm->total_out = 0;

        z_stat = inflate( z_strm, Z_FINISH );
        if( z_stat != Z_STREAM_END )
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "ZLIB error number " << z_stat
                          << std::endl;

                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Decompression failed"
                          << std::endl;
            }
            return( SIO_STREAM_BADCOMPRESS );
        }
    
        z_stat = inflateReset( z_strm );
        if( z_stat != Z_OK )
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "ZLIB error number " << z_stat
                          << std::endl;

                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "Decompression de-initialization failed"
                          << std::endl;
            }
            return( SIO_STREAM_BADCOMPRESS );
        }
    
    }

    //
    // Let the record manager sort out reading all the blocks.
    //
    recmax = bufloc + head_length + ucmp_length;
    status = (*record)->read( this, options );

    //
    // Clear the maps that may have accumulated during record unpacking.
    // This must be done unconditionally (otherwise tables from a busted
    // record may persist into the next record).
    //
    pointerTo->erase( pointerTo->begin(), pointerTo->end() );
    pointedAt->erase( pointedAt->begin(), pointedAt->end() );

    if( !( status & 1 ) )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
	              << "Unpacking error"
                      << std::endl;
        }
    } else {
      // save position of record start // can be queried with lastRecordStart()
      recPos = recStart ;
    }

}

//
// That's all folks!
//
return( status );
}

// ----------------------------------------------------------------------------
// Set the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_stream::setVerbosity
(
    SIO_verbosity   i_verb
)
{ SIO_verbosity o_verb = verbosity; verbosity = i_verb; return( o_verb ); } 

// ----------------------------------------------------------------------------
// Write a record (by record name).
// ----------------------------------------------------------------------------
unsigned int SIO_stream::write
(
    const char*     i_name
)
{

//
// Local variables.
//
SIO_record
   *record;

//
// Validate the record name.
//
record = SIO_recordManager::get( i_name );
if( record == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "/" << i_name << "//] "
                  << "Record name not recognized" 
                  << std::endl;
    }
    return( SIO_STREAM_NOSUCHRECORD );
}

//
// Call the workhorse.
//
return( write( record, i_name ) );
}

// ----------------------------------------------------------------------------
// Write a record (by record pointer).
// ----------------------------------------------------------------------------
unsigned int SIO_stream::write
(
    SIO_record*     record
)
{

//
// Call the workhorse.
//
return( write( record, record->getName()->c_str() ) );
}

// ----------------------------------------------------------------------------
// Write a record (workhorse)
// ----------------------------------------------------------------------------
unsigned int SIO_stream::write
(
    SIO_record*     record,
    const char*     i_name
)
{

//
// Local variables.
//
int
    z_stat;

unsigned int
    data_length,
    data_length_off,
    head_length,
    head_length_off,
    name_length,
    ucmp_length,
    ucmp_length_off,
    bufout,
    compress,
    newlen,
    oldlen,
    options,
    status;

unsigned char
   *newbuf;

static unsigned char
    pad[4] = { 0, 0, 0, 0 };
 
//
// The stream must be open!
//
if( state != SIO_STATE_OPEN )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Cannot write (stream is not open)"
                  << std::endl;
    }
    return( SIO_STREAM_NOTOPEN );
}

//
// This must be a writeable stream!
//
if( mode == SIO_MODE_READ )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "//] "
                  << "Cannot write (stream is read only)"
                  << std::endl;
    }
    return( SIO_STREAM_READONLY );
}

//
// Capture the record name for error reporting.
//
rec_name = i_name;

//
// Initialize the buffer.
//
buffer = bufloc;

//
// Save begin of record (current file end) 
//
recPos = FTELL( handle ) ;

//
// Output: 1) A placeholder for the record header length.
//         2) A 'framing' marker (to help in debugging).
//         3) An options word.
//         4) A placeholder for the record data length (compressed).
//         5) A placeholder for the record data length (uncompressed).
//         6) The length of the record name.
//         7) The record name.
//
compress = record->getCompress();
options  = record->getOptions();

head_length_off = buffer - bufloc;
SIO_DATA( this, &SIO_mark_record,            1           );
SIO_DATA( this, &SIO_mark_record,            1           );
SIO_DATA( this, &options,                    1           );

data_length_off = buffer - bufloc;
SIO_DATA( this, &SIO_mark_record,            1           );

ucmp_length_off = buffer - bufloc;
SIO_DATA( this, &SIO_mark_record,            1           );

name_length = strlen( i_name );
SIO_DATA( this, &name_length,                1           );
SIO_DATA( this,  const_cast<char *>(i_name), name_length );

//
// Back fill the length of the record header.
//
head_length = buffer - bufloc;
SIO_functions::copy( UCHR_CAST(&head_length), (bufloc + head_length_off),
                     SIO_LEN_QB,              1                        );

//
// Ask the record object to fill the buffer with blocks. If an error is
// reported, just print a complaint and skip writing this buffer.
//
status = record->write( this );
if( !(status & 1) )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
	          << "Packing error"
                  << std::endl;
    }
    return( status );
}

//
// Fill in the uncompressed record length.
//
ucmp_length = (buffer - bufloc) - head_length;
SIO_functions::copy( UCHR_CAST(&ucmp_length), (bufloc + ucmp_length_off),
                     SIO_LEN_QB,              1                        );

// //debug
// std::cout << "*************** SIO: ["  << name << "/" << rec_name << "/] "
// << "writing record -  compressed : "  << compress 
// << std::endl;


//
// Write out the complete record.
//
if( !compress )
{
    //
    // Fill in the written length of the record data (identical with the
    // uncompressed length when no compression is being performed).
    //
    data_length = ucmp_length;
    SIO_functions::copy( UCHR_CAST(&data_length), (bufloc + data_length_off),
                         SIO_LEN_QB,              1                        );

    //
    // When not compressing, both the record header and the record data
    // can be written in one swell foop.  Note that uncompressed data always
    // satisfies the requirement that it ends on a four byte boundary, so
    // no padding is required.
    //
    data_length += head_length;
    bufout = FWRITE( bufloc, sizeof(char), data_length, handle );
    if( bufout != data_length && ! FFLUSH( handle ) ) // fg 20030514 - flush the record
    {
        state = SIO_STATE_ERROR;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                      << "File error writing record" 
                      << std::endl;
        }
        return( SIO_STREAM_BADWRITE );
    }
}
else
{
    //
    // Set up for the compression.
    //
    z_strm->next_in   = bufloc + head_length;
    z_strm->avail_in  = ucmp_length;
    z_strm->total_in  = 0;

    z_strm->next_out  = cmploc;
    z_strm->avail_out = cmpmax - cmploc;
    z_strm->total_out = 0;

    //
    // Loop the compression in case the compression buffer is not big enough.
    //
    for(;;)
    {
        z_stat = deflate( z_strm, Z_FINISH );
        if( z_strm->avail_out > 0 )
            break;

        newlen = (cmpmax - cmploc) << 1;
        newbuf = static_cast<unsigned char*>(malloc( newlen ));
        if( newbuf == NULL )
        {
            if( verbosity >= SIO_ERRORS )
	    {
                std::cout << "SIO: ["  
                          << name     << "/"
                          << rec_name << "//] "
                          << "Compression buffer allocation failed"
                          << std::endl;
            }
        
            state = SIO_STATE_ERROR;
            return( SIO_STREAM_NOALLOC );
        }

        oldlen = z_strm->next_out - cmploc;
        memcpy( newbuf, cmploc, oldlen );
        free( cmploc );
        cmploc = newbuf;
        cmpmax = cmploc + newlen;

        z_strm->next_out  = cmploc + oldlen;
        z_strm->avail_out = cmpmax - z_strm->next_out;

        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: ["  
                      << name     << "/"
                      << rec_name << "//] "
                      << "Allocated a "
                      << newlen
                      << "(0x" << std::hex << newlen << std::dec << ")"
                      << " byte compression buffer"
                      << std::endl;
        }
    }

    z_stat = deflateReset( z_strm );
    if( z_stat != Z_OK )
    {
        state = SIO_STATE_ERROR;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "//] "
                      << "ZLIB error number " << z_stat
                      << std::endl;

            std::cout << "SIO: ["  << name << "//] "
                      << "Compression de-initialization failed"
                      << std::endl;
        }
        return( SIO_STREAM_BADCOMPRESS );
    }

    //
    // Fill in the length of the compressed buffer.
    //
    data_length = z_strm->next_out - cmploc;
    SIO_functions::copy( UCHR_CAST(&data_length), (bufloc + data_length_off),
                         SIO_LEN_QB,              1                        );

    //
    // Write the record header.
    //
    bufout = FWRITE( bufloc, sizeof(char), head_length, handle );
    if( bufout != head_length )
    {
        state = SIO_STATE_ERROR;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                      << "File error writing record header" 
                      << std::endl;
        }
        return( SIO_STREAM_BADWRITE );
    }
    
    //
    // Write the compressed record data.
    //
    bufout = FWRITE( cmploc, sizeof(char), data_length, handle );
    if( bufout != data_length && ! FFLUSH(handle) ) // fg 20030514 - flush the record
    {
        state = SIO_STATE_ERROR;
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                      << "File error writing record content" 
                      << std::endl;
        }
        return( SIO_STREAM_BADWRITE );
    }

    //
    // Insert any necessary padding to make the next record header start
    // on a four byte boundary in the file (to make it directly accessible
    // for xdr read).
    //
    newlen = (4 - (data_length & SIO_align)) & SIO_align;
    if( newlen > 0 )
    {
        bufout = FWRITE( pad, sizeof(char), newlen, handle );
        if( bufout != newlen && ! FFLUSH(handle))// fg 20030514 - flush the record
        {
            state = SIO_STATE_ERROR;
            if( verbosity >= SIO_ERRORS )
            {
                std::cout << "SIO: ["  << name << "/" << rec_name << "/] "
                          << "File error writing end-of-record pad" 
                          << std::endl;
            }
            return( SIO_STREAM_BADWRITE );
        }
    }
}

//
// Clear the maps that may have accumulated during record writing.
//
pointerTo->erase( pointerTo->begin(), pointerTo->end() );
pointedAt->erase( pointedAt->begin(), pointedAt->end() );

//
// That's all folks!
//
return( SIO_STREAM_SUCCESS );
}

