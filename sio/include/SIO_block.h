// ----------------------------------------------------------------------------
// CVS $Id: SIO_block.h,v 1.1 2003-03-06 11:01:23 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for an SIO block.                                
// ----------------------------------------------------------------------------
//
// General Description:
//
// Each SIO_block controls one block type.                
//
// ----------------------------------------------------------------------------

#ifndef SIO_BLOCK_H
#define SIO_BLOCK_H 1

#include <string>

#include "SIO_definitions.h"

class SIO_stream;

class SIO_block
{
public:
    SIO_block( const char* );
   ~SIO_block();

    std::string*           getName();
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) = 0;
    virtual unsigned int   version() = 0;

private:
    std::string            name;

}; 
#endif
