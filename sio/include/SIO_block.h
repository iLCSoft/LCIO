// ----------------------------------------------------------------------------
// CVS $Id: SIO_block.h,v 1.2 2003-03-17 12:54:57 gaede Exp $
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
    virtual ~SIO_block();

    std::string*           getName();
    virtual unsigned int   xfer( SIO_stream*, SIO_operation, unsigned int ) = 0;
    virtual unsigned int   version() = 0;

private:
    std::string            name;

}; 
#endif
