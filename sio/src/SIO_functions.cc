// ----------------------------------------------------------------------------
// CVS $Id: SIO_functions.cc,v 1.9 2008-05-28 14:02:09 engels Exp $
// ----------------------------------------------------------------------------
// => Function package for SIO                            
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_functions implements the basic I/O functions of SIO along with a number
// of general purpose utilities.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>
#include <cstdlib>
#include <cstring>

#include "SIO_stream.h"
#include "SIO_functions.h"

// ----------------------------------------------------------------------------
// Deal with 'endian-ness'.  Try to base this on the processor type (because
// the operating system doesn't guarantee endian-ness ... Linux runs happily
// on both x86 CPUs (little endian) and PPC CPUs (big endian)).
//
// Branch on flag provided by compiler:
//
// OS            CPU           Macro         Provided by   Endian-ness
// ------------  ------------  ------------  ------------  -----------
// AIX           PPC(?)        _AIX          GNU compiler  Big
// OSF1          Alpha         __alpha__     GNU compiler  Little
// Linux         x86           __i386__      GNU compiler  Little
// Linux         Opteron       _LP64         GNU compiler  Little
// Linux         itanium       _LP64         GNU compiler  Little
// SunOS         Sparc         __sparc__     GNU compiler  Big
// Windows/NT    Alpha         _M_ALPHA      VC  compiler  Little
// Windows/NT    x86           _M_IX86       VC  compiler  Little
// Windows/NT    MIPS          _M_MRX000     VC  compiler  ?
// Windows/NT    PPC           _M_PPC        VC  compiler  Big
// ----------------------------------------------------------------------------
#if defined(__alpha__) ||   defined(__i386__)  ||   defined(_M_ALPHA)  ||   defined(_M_IX86) || defined(_LP64) || defined(__LITTLE_ENDIAN__)
#define SIO_LITTLE_ENDIAN
#endif

#if defined(_AIX)      ||   defined(__sparc__) ||   defined(_M_PPC) || ( defined(__APPLE_CC__) && !defined(__LITTLE_ENDIAN__)  ) 
#define SIO_BIG_ENDIAN
#endif

// ****************************************************************************
// Primitive copying functions.
//
// This is where the endian-ness problem is resolved.  Note that there are two
// implementations of SIO_functions::copy.  Only (at most) one version gets
// passed to the compiler by the preprocesor.  In the case of a new target CPU
// whose endian-ness cannot determined by the preprocessor directives above,
// -no- version is passed through to the compiler.  The result is a compilation
// failure (i.e. you can't compile the SIO package on a particular CPU if the
// CPU's endian-ness cannot be determined by the SIO preprocessor directives).
//
// In SIO all datasets have the same endian-ness.  The choice between a little-
// or a big-endian dataset representation should be driven by the type of CPU
// which will do most of the work (swapping the endian-ness on read or
// write is somewhat inefficient).  In a ditributed multi-platform environment
// that's a tough call, so the chice is really somewhat arbitrary.  SIO uses
// big-endian xdr-like data representation.
// ****************************************************************************

#ifdef SIO_BIG_ENDIAN
// ----------------------------------------------------------------------------
// => Keep the byte ordering during the copy.
// ----------------------------------------------------------------------------
void SIO_functions::copy
(
    unsigned char*     from,
    unsigned char*     dest,
    const int          size,
    const int          count
)
{

//
// Local variables.
//
int
    totlen;

//
// Just do it!
//
totlen = size * count;
memcpy( dest, from, totlen );

//
// That's all folks!
//
return;
}
#endif

#ifdef SIO_LITTLE_ENDIAN
// ----------------------------------------------------------------------------
// => Reverse the byte ordering during the copy.
// ----------------------------------------------------------------------------
void SIO_functions::copy
(
    unsigned char*     from,
    unsigned char*     dest,
    const int          size,
    const int          count
)
{

//
// Local variables.
//
int
    icnt,
    ibyt,
    jump;

//
// Just do it!
//
dest += size;
jump  = size << 1;
for( icnt = 0; icnt < count; icnt++ )
{
    for( ibyt = 0; ibyt < size; ibyt++  )
    {
        *--dest = *from++;
    }
    dest += jump;
}

//
// That's all folks!
//
return;
}
#endif

// ----------------------------------------------------------------------------
// => Transfer to/from the buffer.
// ----------------------------------------------------------------------------
unsigned int SIO_functions::xfer
(
    SIO_stream*         stream,
    const int           size,
    const int           count,
    unsigned char*      fromto
)
{

//
// Local variables.
//
int
    bytcnt,
    bytlen,
    padlen;

//
// Is the stream healthy.
//
if( stream->state == SIO_STATE_ERROR )
    return( SIO_STREAM_BADSTATE );

//
// Byte count length and padded byte count length.
//
bytlen = count * size;
padlen = (bytlen + 3) & 0xfffffffc;

//
// Ensure the data will fit into the output buffer (write).
//
if( stream->mode != SIO_MODE_READ )
{ 
    if( (stream->buffer + padlen) > stream->bufmax )
    {
        int
            newlen,
            oldlen;

        unsigned char
           *newbuf;

//fg: --- this is not correct if the new request is larger than double the size...
//         newlen = (stream->bufmax - stream->bufloc) << 1;

	int minNeeded = (stream->buffer + padlen) - stream->bufmax ;
	int bufSize = stream->bufmax - stream->bufloc ;
	int resizeFactor = 2 ;
	while( bufSize * (resizeFactor-1)  < minNeeded ) 
	  ++resizeFactor ;

	newlen = bufSize * resizeFactor  ;

// 	std::cout << " resizing buffer - needed : " << minNeeded 
// 		  << " bufSize : " << bufSize 
// 		  << " resize factor : " << resizeFactor
// 		  << " new size: " << std::endl ;
//fg: ---

        newbuf = (unsigned char *)malloc( newlen );
        if( newbuf == NULL )
        {
            if( stream->verbosity >= SIO_ERRORS )
	    {
                std::cout << "SIO: ["  
                          << stream->name     << "/"
                          << stream->rec_name << "/"
                          << stream->blk_name << "] "
                          << "Buffer allocation failed"
                          << std::endl;
            }
        
            stream->state = SIO_STATE_ERROR;
            return( SIO_STREAM_NOALLOC );
        }


	oldlen = stream->buffer - stream->bufloc;
 
        memcpy( newbuf, stream->bufloc, oldlen );
        free( stream->bufloc );
        stream->blkmax = newbuf + (stream->blkmax - stream->bufloc);
        stream->bufmax = newbuf + newlen;
        stream->buffer = newbuf + oldlen;
        stream->bufloc = newbuf;

        if( stream->verbosity >= SIO_ALL )
                std::cout << "SIO: ["  
                          << stream->name     << "/"
                          << stream->rec_name << "/"
                          << stream->blk_name << "] "
                          << "Allocated a "
                          << newlen
                          << "(0x" << std::hex << newlen << std::dec << ")"
                          << " byte buffer"
                          << std::endl;
    }

    //
    // Copy and write padding as necessary (xdr format).
    //
    SIO_functions::copy( fromto, stream->buffer, size, count );
    stream->buffer += bytlen;
    for( bytcnt = bytlen; bytcnt < padlen; bytcnt++ )
        *stream->buffer++ = 0;
}

//
// Ensure the request stays in bounds (read)
//
else
{ 
    if( (stream->buffer + padlen) > stream->blkmax )
    {
        if( stream->verbosity >= SIO_ERRORS )
	{
            std::cout << "SIO: ["  
                      << stream->name     << "/"
                      << stream->rec_name << "/"
                      << stream->blk_name << "] "
                      << "Read request exceeds size of block! "
                      << std::endl;
	}

        stream->state = SIO_STATE_ERROR;
        return( SIO_STREAM_OFFEND );
    }

    //
    // Copy and skip over padding as necessary (xdr format).
    //
    SIO_functions::copy( stream->buffer, fromto, size, count );
    stream->buffer += padlen;
}

//
// That's all folks!
//
return( SIO_STREAM_SUCCESS);
}

// ****************************************************************************
// Transfer functions for (arrays of) primitive data types.
// ****************************************************************************
// ----------------------------------------------------------------------------
// => Transfer (array of) 'char'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    char*                      xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_SB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'unsigned char'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    unsigned char*             xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_SB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'short'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    short*                     xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_DB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'unsigned short'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    unsigned short*            xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_DB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'int'
// ----------------------------------------------------------------------------
// An 'int' is four bytes on -all- machines!  On DEC/OSF1, avoid using 'long'
// which it treats as 8 bytes (as opposed to 4 on AIX(PPC), Linux(I86) and
// SunOS(Sparc)). 
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    int*                       xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_QB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'unsigned int'
// ----------------------------------------------------------------------------
// An 'unsigned int' is four bytes on -all- machines!  On DEC/OSF1, avoid using
// 'unsigned long' which it treats as 8 bytes (as opposed to 4 on AIX(PPC),
// Linux(I86) and SunOS(Sparc)). 
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    unsigned int*              xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_QB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'long long' (8 bytes on all machines).
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    SIO_64BITINT*              xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_OB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'unsigned long long' (8 bytes on all machines)
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    unsigned SIO_64BITINT*     xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_OB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'float'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    float*                     xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_QB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer (array of) 'double'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::data
(
    SIO_stream*                stream,
    double*                    xfer,
    const int                  cnt
)
{ return( SIO_functions::xfer( stream, SIO_LEN_OB, cnt, UCHR_CAST( xfer ))); }

// ----------------------------------------------------------------------------
// => Transfer a 'pointed at by something'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::pointed_at
(
    SIO_stream*                stream,
    SIO_POINTER_DECL*          xfer
)
{

static unsigned int
    SIO_ptag = 0xffffffff;

//
// Whether reading or writing the basic principle of SIO_pointed_at is to
// build a map of pairs of values called the 'pointed at' table.  The routine
// SIO_pointer_to builds a similar table called the 'pointer to' table.  The
// first value in each table is the 'match' value.  When writing, both tables
// have the same layout:
//
//  First value:    Pointer to a memory location
// Second value:    Offset in the output buffer
//
// When reading, that changes to:
//
//  First value:    Simple 32-bit integer value read from the buffer
// Second value:    Pointer to a memory location
//
// That means that these tables have different 'shapes' depending on whether
// the program is reading or writing.  Given that a stream must be either
// read or write, I was loath to allocate four tables when at most two
// would ever be used.  I have therefore reused the same tables for both
// purposes.  Hmmm.  Maybe not the world's best decision.  I have been
// forced to use a lot of 'reinterpret_cast' statements to make it all
// work (a process complicated by the fact that pointers are not the same
// size on all architectures).
//

//
// Write.  Save the memory location of this object along with the offset
// in the output buffer where the generated match quantity must go.  Put
// a placeholder in the output buffer (it will be overwritten at the "output
// relocation" stage).
//
if( stream->mode != SIO_MODE_READ )
{
    std::pair< void* const, void* >
        entry( xfer, 
               reinterpret_cast<void *>( stream->buffer - stream->bufloc ) );

    stream->pointedAt->insert( entry );

    return( SIO_functions::xfer( stream, SIO_LEN_QB, 1, UCHR_CAST(&SIO_ptag)));
}

//
// Read.  Keep a record of the "match" quantity read from the buffer and
// the location in memory which will need relocating.
//
else
{
    unsigned int
        match,
        status;

    status = SIO_functions::xfer( stream, SIO_LEN_QB, 1, UCHR_CAST( &match ) );
    if( !( status & 1 ) )
        return( status );

    //
    // Ignore match = SIO_ptag .  This is basically a pointer target which was
    // never relocated when the record was written. i.e. nothing points to it!
    // Don't clutter the maps with information that can never be used. 
    //
    if( match != SIO_ptag )
    {
        std::pair< void* const, void* >
            entry( reinterpret_cast<void *>(match), xfer );

        stream->pointedAt->insert( entry );
    }
}

//
// That's all folks!
//
return( SIO_STREAM_SUCCESS );
}

// ----------------------------------------------------------------------------
// => Transfer a 'pointer to something'
// ----------------------------------------------------------------------------
unsigned int SIO_functions::pointer_to
(
    SIO_stream*                stream,
    SIO_POINTER_DECL*          xfer
)
{
static unsigned int
    SIO_pntr = 0x00000000;              // Placeholder value for 'pointer to'

void
   *ifer;                               // Indirect xfer (actually **xfer)

//
// xfer is really a pointer-to-a-pointer.  This routine is most interested
// in the value of *xfer when treated as a pointer.  C++ tends to object
// to this as being 'not type safe'.  To keep the compiler happy (and purists
// miserable), do one 'reinterpret_cast' immediately to make later code
// easier to read.
//
ifer = reinterpret_cast<void *>(*xfer);

//
// Whether reading or writing the basic principle of SIO_pointer_to is to
// build a map of pairs of values called the 'pointer to' table.  The routine
// SIO_pointed_at builds a similar table called the 'pointed at' table.  The
// first value in each table is the 'match' value.  When writing, both tables
// have the same layout:
//
//  First value:    Pointer to a memory location
// Second value:    Offset in the output buffer
//
// When reading, that changes to:
//
//  First value:    Simple 32-bit integer value read from the buffer
// Second value:    Pointer to a memory location
//
// That means that these tables have different 'shapes' depending on whether
// the program is reading or writing.  Given that a stream must be either
// read or write, I was loath to allocate four tables when at most two
// would ever be used.  I have therefore reused the same tables for both
// purposes.  Hmmm.  Maybe not the world's best decision.  I have been
// forced to use a lot of 'reinterpret_cast' statements to make it all
// work (a process complicated by the fact that pointers are not the same
// size on all architectures).
//

//
// Write.  Keep a record of the "match" quantity (i.e. the value of the
// pointer (which may be different lengths on different machines!)) and
// the current offset in the output buffer.  Put a placeholder in the
// output buffer (it will be overwritten at the "output relocation" stage).
//
if( stream->mode != SIO_MODE_READ )
{
    //
    // Ignore NULL pointers.  These are always recorded in the buffer with a
    // zero match word (and are treated specially when read back).  There's no
    // point in putting useless information in the maps.
    //
    if( ifer != NULL )
    {
        std::pair< void* const, void* >
            entry( ifer,
            reinterpret_cast<void *>(stream->buffer - stream->bufloc) );

        stream->pointerTo->insert( entry );
    }
    return( SIO_functions::xfer( stream, SIO_LEN_QB, 1, UCHR_CAST(&SIO_pntr)));
}

//
// Read.  Keep a record of the "match" quantity read from the buffer and
// the location in memory which will need relocating.
//
else
{
    unsigned int
        match,
        status;

    status = SIO_functions::xfer( stream, SIO_LEN_QB, 1, UCHR_CAST( &match ) );
    if( !( status & 1 ) )
        return( status );

    //
    // Ignore match = SIO_pntr.  This is basically a null pointer which can
    // never be relocated, so don't fill the multimap with a lot of useless
    // information.
    //
    //
    // C cast replaces static_cast for:
    //
    // std::pair
    //     entry( static_cast<void *>(match),
    //            static_cast<void *>(xfer) );
    //
    if( match != SIO_pntr )
    {
        std::pair< void* const, void* >
            entry( reinterpret_cast<void *>(match), xfer );

        stream->pointerTo->insert( entry );
    }

    //
    // Hand -something- back to the caller.  The number passed back is -not-
    // a pointer, and pointer relocation will not occur until the whole record
    // has been read.  The only circumstance where the next line is important
    // is the case of a NULL pointer which the caller may be relying on to
    // find the end of (for instance) a singly linked list.
    //
    *xfer = static_cast<SIO_POINTER_DECL>(match);
}

//
// That's all folks!
//
return( SIO_STREAM_SUCCESS );
}

// ****************************************************************************
// Miscellaneous functions
// ****************************************************************************

// ----------------------------------------------------------------------------
// => Test for a legal name
// ----------------------------------------------------------------------------
//
// General Description:
//
// validateName tests the validity (according to SIO) of a name.  SIO only
// accepts names starting with (regular expression) [A-Za-z_] and continuing
// with [A-Za-z0-9_] (which most people will recognize as the definition of
// a C/C++ variable name).
//
// ----------------------------------------------------------------------------
bool SIO_functions::validateName
(
    const char* name
)
{

//
// Just do it!
//
if( *name < 0                              ) return( false );
if( !isalpha( (int)*name ) && *name != '_' ) return( false );

for( name += 1; *name != '\0'; name++ )
{
    if( *name < 0                              ) return( false );
    if( !isalnum( (int)*name ) && *name != '_' ) return( false );
} 

//
// That's all folks!
//
return( true );
}

