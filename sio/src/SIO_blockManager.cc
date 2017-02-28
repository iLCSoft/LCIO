// ----------------------------------------------------------------------------
// CVS $Id: SIO_blockManager.cc,v 1.2 2004-04-05 13:33:11 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO blocks.                           
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_blockManager manages a list of SIO blocks.                   
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>
#include <vector>

#include "SIO_recordManager.h"
#include "SIO_blockManager.h"
#include "SIO_block.h"
#include "SIO_functions.h"

// ----------------------------------------------------------------------------
// Initialize the private static variables.
// ----------------------------------------------------------------------------
blockMap_c*    SIO_blockManager::blockMap  = NULL;
SIO_verbosity  SIO_blockManager::verbosity = SIO_SILENT;

// ----------------------------------------------------------------------------
// Add a block of the given name.
// ----------------------------------------------------------------------------
SIO_block* SIO_blockManager::add
(
    SIO_block*     block
)
{

//
// Local variables.
//
std::string
   *s_name;

std::pair< blockMap_i, bool >
    status;

const char
   *i_name;

//
// Check the name for validity.
//
s_name = block->getName();
i_name = s_name->c_str();
if( !SIO_functions::validateName( i_name ) )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [Block Manager] "
                  << "Invalid block name"
                  << i_name
                  << std::endl;
    }
    return( NULL );
}

//
// If the map's never been instantiated, do it now!
//
if( blockMap == NULL )
    blockMap = new blockMap_c;

//
// Initialize the map entry.
//
std::pair< std::string const, SIO_block* >
    entry( *s_name, block );

//
// Insert the entry.  This may or may not succeed depending on whether the
// named block pre-exists.  If it does pre-exist, print a warning and return
// a NULL pointer (that should get the caller's attention).
//
status = blockMap->insert( entry );
if( !status.second )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [Block Manager] Block"
                  << i_name
                  << "not added (already exists)"
                  << std::endl;
    }
    return( NULL );
}

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [Block Manager] Added block "
              << i_name
              << std::endl;
}

//
// That's all folks!
//
return (status.first)->second;
}

// ----------------------------------------------------------------------------
// Given its name, return a pointer to a block.
// ----------------------------------------------------------------------------
SIO_block* SIO_blockManager::get
(
    const char*  i_name
)
{

//
// Search the map (if it exists yet!)
//
if( blockMap != NULL )
{
    blockMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = blockMap->find( s_name )) != blockMap->end() )
    {
        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Block Manager] Block "
                      << i_name
                      << " is defined (pointer returned)"
                      << std::endl;
        }
        return( iter->second );
    }
}
 
if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [Block Manager] Block "
              << i_name
              << " has not been defined"
              << std::endl;
}

//
// That's all folks!
//
return( NULL );
}

// ----------------------------------------------------------------------------
// Get the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_blockManager::getVerbosity() { return( verbosity ); }


// ----------------------------------------------------------------------------
//  remove and delete all known blocks
// ----------------------------------------------------------------------------
// FG 05042004: needed LCIO memory mgmt
void SIO_blockManager::clear() {

  blockMap_i iter;
  std::vector<std::string> blockNames ;
    
  if( blockMap == 0 ) return ; // nothing to clear

  // as the d'tor of SIO_block removes the block from the map
  // using the map iterator is not safe, so get a list of keys first
  for(iter = blockMap->begin() ;  iter != blockMap->end() ; iter++) {
    blockNames.push_back( iter->first  ) ;
  }

  for(unsigned i=0 ;i<blockNames.size() ;i++){
    delete (*blockMap)[ blockNames[i] ] ;
  }
  
  delete blockMap;
  blockMap = NULL;

}

// ----------------------------------------------------------------------------
// Given its name, remove a block.
// ----------------------------------------------------------------------------
unsigned int SIO_blockManager::remove
(
    const char*  i_name
)
{

//
// Search the map (if it exists!)
//
if( blockMap != NULL )
{
    blockMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = blockMap->find( s_name )) != blockMap->end() )
    {

        //
        // The block is in the list.  Before removing it, ensure that it
        // isn't connected to any records.
        //
        SIO_recordManager::disconnect( i_name );


        //
        // OK, remove it from management.
        //
        blockMap->erase( iter );

        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Block Manager] Removed block "
                      << i_name
                      << std::endl;
        }

	// fg - done in clear
//         if( blockMap->size() == 0 )
//         {
//             delete blockMap;
//             blockMap = NULL;
//         } 
        return( SIO_BLOCK_SUCCESS );
    }
}

if( verbosity >= SIO_ERRORS )
{
    std::cout << "SIO: [Block Manager] Cannot remove block "
              << i_name
              << " (it was never added)"
              << std::endl;
}

//
// That's all folks!
//
return( SIO_BLOCK_NOTFOUND );
}

// ----------------------------------------------------------------------------
// Given its pointer, remove a block.
// ----------------------------------------------------------------------------
unsigned int SIO_blockManager::remove
(
    SIO_block*   block
)
{ return( remove( block->getName()->c_str() ) ); }

// ----------------------------------------------------------------------------
// Set the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_blockManager::setVerbosity
(
    SIO_verbosity   i_verb
)
{ SIO_verbosity o_verb = verbosity; verbosity = i_verb; return( o_verb ); } 
