// ----------------------------------------------------------------------------
// CVS $Id: SIO_streamManager.h,v 1.1 2003-03-06 11:01:23 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO streams.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_streamManager manages a list of SIO streams.
//
// ----------------------------------------------------------------------------

#ifndef SIO_STREAMMANAGER_H
#define SIO_STREAMMANAGER_H 1

#include <map>
#include <string>

#include "SIO_definitions.h"

class SIO_stream;

typedef std::map< std::string, SIO_stream* >           streamMap_c;
typedef std::map< std::string, SIO_stream* >::iterator streamMap_i;

class SIO_streamManager
{
public:
    static SIO_stream*     add( const char* );
    static SIO_stream*     add( const char*, unsigned int );
    static SIO_stream*     get( const char* );
    static SIO_verbosity   getVerbosity();
    static unsigned int    remove( const char* );
    static unsigned int    remove( SIO_stream* );
    static SIO_verbosity   setVerbosity( SIO_verbosity );

private:
    static streamMap_c*    streamMap;
    static SIO_verbosity   verbosity;

}; 
#endif
