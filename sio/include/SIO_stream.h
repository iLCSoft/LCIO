// ----------------------------------------------------------------------------
// CVS $Id: SIO_stream.h,v 1.5 2007-10-16 16:22:45 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for a single SIO stream.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_stream manages operations of a single SIO stream.
//
// ----------------------------------------------------------------------------

#ifndef SIO_STREAM_H
#define SIO_STREAM_H 1

#include <map>
#include <string>

#include <stdio.h>

#include "SIO_definitions.h"
#include "SIO_functions.h"
#include "SIO_record.h"

typedef std::map< void*, void* >                pointedAtMap_c;
typedef std::map< void*, void* >::iterator      pointedAtMap_i;

typedef std::multimap< void*, void* >           pointerToMap_c;
typedef std::multimap< void*, void* >::iterator pointerToMap_i;

struct z_stream_s;

// ----------------------------------------------------------------------------
// Class SIO_stream.         
// ----------------------------------------------------------------------------
class SIO_stream
{
public:
    unsigned int           close();
    unsigned int           flush();
    void                   dump( unsigned int, unsigned int );
    std::string*           getName();
    std::string*           getFilename();
    SIO_stream_mode        getMode();
    SIO_stream_state       getState();  
    SIO_verbosity          getVerbosity();  
    unsigned int           open( const char *, SIO_stream_mode );
    unsigned int           read( SIO_record** );
    SIO_verbosity          setVerbosity( SIO_verbosity );
    unsigned int           write( const char* );
    unsigned int           write( SIO_record* );

    // return the start of the last record read; -1 if not sucessfull
    SIO_64BITINT           lastRecordStart() { return recPos ; }
    unsigned int           seek(SIO_64BITINT pos) ;

private:
    SIO_stream( const char *, unsigned int, SIO_verbosity );
   ~SIO_stream();

    unsigned int           write( SIO_record*, const char* );

    unsigned char*         bufloc;        // Buffer pointer (beginning)
    unsigned char*         buffer;        // Buffer pointer (current)
    unsigned char*         bufmax;        // Buffer pointer (end)
    unsigned char*         recmax;        // Record pointer (end)
    unsigned char*         blkmax;        // Block  pointer (end)

    unsigned char*         cmploc;        // Compression buffer pointer (beg)
    unsigned char*         cmpmax;        // Compression buffer pointer (end)
    struct z_stream_s*     z_strm;        // Compression buffer control

    std::string            name;          // Stream's name
    std::string            filename;      // Stream's associated file
    FILE*                  handle;        // File handle

    std::string            rec_name;      // Record name being read
    std::string            blk_name;      // Block  name being read

    pointedAtMap_c*        pointedAt;     // Map      of 'pointed at'
    pointerToMap_c*        pointerTo;     // Multimap of 'pointer to'

    SIO_stream_mode        mode;          // Stream mode
    unsigned int           reserve;       // Reserved size of buffer
    SIO_stream_state       state;         // Stream state  
    SIO_verbosity          verbosity;     // Reporting level

    SIO_64BITINT           recPos  ;      // start Position of last record read


friend class SIO_streamManager;           // Access to constructor/destructor
friend class SIO_record;                  // Access to buffer
friend class SIO_functions;               // Access to buffer and pointer maps
}; 

#endif




















