// ----------------------------------------------------------------------------
// CVS $Id: SIO_streamManager.cc,v 1.1 2003-03-06 11:01:24 gaede Exp $
// ----------------------------------------------------------------------------
// => Manager for a list of SIO streams.                           
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_streamManager manages a list of SIO streams.                   
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>

#include "SIO_streamManager.h"
#include "SIO_stream.h"
#include "SIO_functions.h"

// ----------------------------------------------------------------------------
// Initialize the private static variables.
// ----------------------------------------------------------------------------
streamMap_c*   SIO_streamManager::streamMap = NULL;
SIO_verbosity  SIO_streamManager::verbosity = SIO_ERRORS;

// ----------------------------------------------------------------------------
// Add a stream of the given name (buffer reserve not provided).
// ----------------------------------------------------------------------------
SIO_stream* SIO_streamManager::add
(
    const char*  i_name
)
{ return add( i_name, 4 * SIO_KBYTE ); }

// ----------------------------------------------------------------------------
// Add a stream of the given name (buffer reserve provided).
// ----------------------------------------------------------------------------
SIO_stream* SIO_streamManager::add
(
    const char*    i_name,
    unsigned int   i_reserve
)
{

//
// Local variables.
//
std::pair< streamMap_i, bool >
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
        std::cout << "SIO: [Stream Manager] "
                  << "Invalid stream name "
                  << i_name
                  << std::endl;
    }
    return( NULL );
}

//
// Ensure a minimum 4 kByte buffer.
//
if( i_reserve < 4 * SIO_KBYTE )
    i_reserve = 4 * SIO_KBYTE;

//
// If the map's never been instantiated, do it now!
//
if( streamMap == NULL )
    streamMap = new streamMap_c;

//
// Initialize the map entry.
//
std::pair< std::string const, SIO_stream* >
    entry( s_name, NULL );

//
// Insert the entry.  This may or may not succeed depending on whether the
// named stream pre-exists.  If it does pre-exist, print a warning and return
// a NULL pointer (that should get the caller's attention).
//
status = streamMap->insert( entry );
if( !status.second )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [Stream Manager] Stream "
                  << i_name
                  << " not added (already exists)"
                  << std::endl;
    }
    return( NULL );
}

//
// Create the stream to go with this map entry.
//
(status.first)->second = new SIO_stream( i_name, i_reserve, verbosity );

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [Stream Manager] Added stream "
              << i_name
              << std::endl;
}

//
// That's all folks!
//
return (status.first)->second;
}

// ----------------------------------------------------------------------------
// Given its name, return a pointer to a stream.
// ----------------------------------------------------------------------------
SIO_stream* SIO_streamManager::get
(
    const char*  i_name
)
{

//
// Search the map (if it exists yet!)
//
if( streamMap != NULL )
{
    streamMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = streamMap->find( s_name )) != streamMap->end() )
    {
        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Stream Manager] Stream "
                      << i_name
                      << " is defined (pointer returned)"
                      << std::endl;
        }
        return( iter->second );
    }
}
 
if( verbosity >= SIO_ERRORS )
{
    std::cout << "SIO: [Stream Manager] Stream "
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
SIO_verbosity SIO_streamManager::getVerbosity() { return( verbosity ); }

// ----------------------------------------------------------------------------
// Given its name, remove a stream.
// ----------------------------------------------------------------------------
unsigned int SIO_streamManager::remove
(
    const char*  i_name
)
{

//
// Search the map (if it exists!)
//
if( streamMap != NULL )
{
    streamMap_i
        iter;

    std::string
        s_name = i_name;
    
    if( (iter = streamMap->find( s_name )) != streamMap->end() )
    {
        delete iter->second;
        streamMap->erase( iter );

        if( verbosity >= SIO_ALL )
        {
            std::cout << "SIO: [Stream Manager] Removed stream "
                      << i_name
                      << std::endl;
        }

        if( streamMap->size() == 0 )
	{
            delete streamMap;
            streamMap = NULL;
        } 
        return( SIO_STREAM_SUCCESS );
    }
}

if( verbosity >= SIO_ERRORS )
{
    std::cout << "SIO: [Stream Manager] Cannot remove stream "
              << i_name
              << " (it was never added)"
              << std::endl;
}

//
// That's all folks!
//
return( SIO_STREAM_NOTFOUND );
}

// ----------------------------------------------------------------------------
// Given its pointer, remove a stream.
// ----------------------------------------------------------------------------
unsigned int SIO_streamManager::remove
(
    SIO_stream*  record
)
{ return( remove( record->getName()->c_str() ) ); }

// ----------------------------------------------------------------------------
// Set the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_streamManager::setVerbosity
(
    SIO_verbosity   i_verb
)
{ SIO_verbosity o_verb = verbosity; verbosity = i_verb; return( o_verb ); } 

