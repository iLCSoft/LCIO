// ----------------------------------------------------------------------------
// CVS $Id: SIO_recordManager.h,v 1.1 2003-03-06 11:01:23 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO records.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_recordManager manages a list of SIO records.
//
// ----------------------------------------------------------------------------

#ifndef SIO_RECORDMANAGER_H
#define SIO_RECORDMANAGER_H 1

#include <map>
#include <string>

#include "SIO_definitions.h"

class SIO_record;

typedef std::map< std::string, SIO_record* >           recordMap_c;
typedef std::map< std::string, SIO_record* >::iterator recordMap_i;

class SIO_recordManager
{
public:
    static SIO_record*     add( const char* );
    static SIO_record*     get( const char* );
    static SIO_verbosity   getVerbosity();
    static unsigned int    remove( const char* );
    static unsigned int    remove( SIO_record* );
    static SIO_verbosity   setVerbosity( SIO_verbosity );

private:
    static void            disconnect( const char* );
    static recordMap_c*    recordMap;
    static SIO_verbosity   verbosity;

friend class SIO_blockManager;            // Disconnect all a block's connect's
}; 
#endif


