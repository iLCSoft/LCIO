// ----------------------------------------------------------------------------
// CVS $Id: SIO_block.cc,v 1.2 2004-04-05 13:33:11 gaede Exp $
// ----------------------------------------------------------------------------
// => Base class for real I/O blocks.
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_block is a base class from which real I/O blocks are derived.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include "SIO_blockManager.h"
#include "SIO_block.h"

// ----------------------------------------------------------------------------
// Constructor
// ----------------------------------------------------------------------------
SIO_block::SIO_block
(
    const char*      i_name
)
 : name( i_name ) 
{
  //FG 05042004:  register all blocks automatically with block manager :
  SIO_blockManager::add(  this  ) ;
}

// ----------------------------------------------------------------------------
// Destructor
// ----------------------------------------------------------------------------
SIO_block::~SIO_block()
{

//
// This is tricky.  Before the block can be deleted, ensure that it does not
// appear in the list managed by block manager.  The only way to find out if
// this block is under the control of the block manager is to ask the block
// manager to find this block's name in its list and return a pointer to it.
// I told you this was tricky!
//
if( (SIO_blockManager::get( getName()->c_str() )) != NULL )
    SIO_blockManager::remove( this );

return;
}

// ----------------------------------------------------------------------------
// Return block name.
// ----------------------------------------------------------------------------
std::string* SIO_block::getName()          { return( &name ); }



