// ----------------------------------------------------------------------------
// CVS $Id: SIO_blockManager.h,v 1.2 2004-04-05 13:33:11 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO blocks.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_blockManager manages a list of SIO blocks.
//
// ----------------------------------------------------------------------------

#ifndef SIO_BLOCKMANAGER_H
#define SIO_BLOCKMANAGER_H 1

#include <map>
#include <string>

#include "SIO_definitions.h"

class SIO_block;

typedef std::map< std::string, SIO_block* >           blockMap_c;
typedef std::map< std::string, SIO_block* >::iterator blockMap_i;

class SIO_blockManager
{
public:
    static SIO_block*      add( SIO_block* );
    static SIO_block*      get( const char* );
    static SIO_verbosity   getVerbosity();
    static unsigned int    remove( const char* );
    static unsigned int    remove( SIO_block* );
    static SIO_verbosity   setVerbosity( SIO_verbosity );

    //FG 05042004:  clean up needed for memory management
    static void            clear() ;

private:
    static blockMap_c*     blockMap;
    static SIO_verbosity   verbosity;

}; 
#endif
