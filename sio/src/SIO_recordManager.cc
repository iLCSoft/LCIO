// ----------------------------------------------------------------------------
// CVS $Id: SIO_recordManager.cc,v 1.1 2003-03-06 11:01:24 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO records.                           
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_recordManager manages a list of SIO records.                   
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>

#include "SIO_recordManager.h"
#include "SIO_record.h"
#include "SIO_functions.h"

// ----------------------------------------------------------------------------
// Initialize the private static variables.
// ----------------------------------------------------------------------------
recordMap_c*   SIO_recordManager::recordMap = NULL;
SIO_verbosity  SIO_recordManager::verbosity = SIO_ERRORS;

// ----------------------------------------------------------------------------
// Add a record of the given name.
// ----------------------------------------------------------------------------
SIO_record* SIO_recordManager::add
(
    const char*     i_name
)
{

//
// Local variables.
//
std::pair< recordMap_i, bool >
    status;

std::string
    s_name = i_name;

//
// Check the name for validity.
//
if( !SIO_functions::validateName( i_name ) )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [Record Manager] "
                  << "Invalid record name"
                  << i_name
                  << std::endl;
    }
    return( NULL );
}

//
// If the map's never been instantiated, do it now!
//
if( recordMap == NULL )
    recordMap = new recordMap_c;

//
// Initialize the map entry.
//
std::pair< std::string const, SIO_record* >
    entry( s_name, NULL );

//
// Insert the entry.  This may or may not succeed depending on whether the
// named record pre-exists.  If it does pre-exist, print a warning and return
// a NULL pointer (that should get the caller's attention).
//
status = recordMap->insert( entry );
if( !status.second )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [Record Manager] Record"
                  << i_name
                  << "not added (already exists)"
                  << std::endl;
    }
    return( NULL );
}

//
// Create the record to go with this map entry.
//
(status.first)->second = new SIO_record( i_name, verbosity );

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [Record Manager] Added record "
              << i_name
              << std::endl;
}

//
// That's all folks!
//
return (status.first)->second;
}

// ----------------------------------------------------------------------------
// Remove all connections for a block.
// ----------------------------------------------------------------------------
void SIO_recordManager::disconnect
(
    const char*  i_name
)
{

//
// Loop over the map (if it exists yet!)
//
if( recordMap != NULL )
{
    recordMap_i
        iter;

    for( iter = recordMap->begin(); iter != recordMap->end(); iter++ )
    {
        if( iter->second->getConnect( i_name ) != NULL )
            iter->second->disconnect( i_name );
    }
}
 
if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [Record Manager] Block "
              << i_name
              << " disconnected from all records"
              << std::endl;
}

//
// That's all folks!
//
return;
}

// ----------------------------------------------------------------------------
// Given its name, return a pointer to a record.
// ----------------------------------------------------------------------------
SIO_record* SIO_recordManager::get
(
    const char*  i_name
)
{

//
// Search the map (if it exists yet!)
//
if( recordMap != NULL )
{
    recordMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = recordMap->find( s_name )) != recordMap->end() )
    {
        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Record Manager] Record "
                      << i_name
                      << " is defined (pointer returned)"
                      << std::endl;
        }
        return( iter->second );
    }
}
 
if( verbosity >= SIO_ERRORS )
{
    std::cout << "SIO: [Record Manager] Record "
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
SIO_verbosity SIO_recordManager::getVerbosity() { return( verbosity ); }

// ----------------------------------------------------------------------------
// Given its name, remove a record.
// ----------------------------------------------------------------------------
unsigned int SIO_recordManager::remove
(
    const char*  i_name
)
{

//
// Search the map (if it exists!)
//
if( recordMap != NULL )
{
    recordMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = recordMap->find( s_name )) != recordMap->end() )
    {
        delete iter->second;
        recordMap->erase( iter );

        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Record Manager] Removed record "
                      << s_name
                      << std::endl;
        }

        if( recordMap->size() == 0 )
	{
            delete recordMap;
            recordMap = NULL;
        } 
        return( SIO_RECORD_SUCCESS );
    }
}

if( verbosity >= SIO_ERRORS )
{
    std::cout << "SIO: [Record Manager] Cannot remove record "
              << i_name
              << " (it was never added)"
              << std::endl;
}

//
// That's all folks!
//
return( SIO_RECORD_NOTFOUND );
}

// ----------------------------------------------------------------------------
// Given its pointer, remove a record.
// ----------------------------------------------------------------------------
unsigned int SIO_recordManager::remove
(
    SIO_record*  record
)
{ return( remove( record->getName()->c_str() ) ); }

// ----------------------------------------------------------------------------
// Set the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_recordManager::setVerbosity
(
    SIO_verbosity   i_verb
)
{ SIO_verbosity o_verb = verbosity; verbosity = i_verb; return( o_verb ); } 
