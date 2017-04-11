// ----------------------------------------------------------------------------
// CVS $Id: SIO_record.cc,v 1.6 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for a single SIO record.                          
// ----------------------------------------------------------------------------
//
// General Description:
//
// Each SIO_record controls one record type.
//
// ----------------------------------------------------------------------------

#ifdef _MSC_VER
#   pragma warning(disable:4786)        // >255 characters in debug information
#endif

#include <iostream>
#include <cstdlib>

#include "SIO_block.h"
#include "SIO_blockManager.h"
#include "SIO_definitions.h"
#include "SIO_functions.h"
#include "SIO_record.h"
#include "SIO_stream.h"

static unsigned int
    SIO_mark_block = 0xdeadbeef;

// ----------------------------------------------------------------------------
// Constructor.
// ----------------------------------------------------------------------------
SIO_record::SIO_record
(
    const char*     i_name,
    SIO_verbosity   i_verbosity
 ) : name(i_name), verbosity( i_verbosity )
{}

// ----------------------------------------------------------------------------
// Destructor.
// ----------------------------------------------------------------------------
SIO_record::~SIO_record() {}

// ----------------------------------------------------------------------------
// Connect a block to a record (by name).
// ----------------------------------------------------------------------------
unsigned int SIO_record::connect
(
    const char*  i_name
)
{

//
// Local variables.
//
SIO_block
   *block;

std::string
    s_name = i_name;

//
// Validate the name.
//
block = SIO_blockManager::get( i_name );
if( block == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/" << i_name << "] "
                  << "Block not connected (name not recognized)"
                  << std::endl;
    }
    return( SIO_RECORD_BADARGUMENT );
}

//
// Go to the workhorse.
//
return( connect( &s_name, block ) );
}

// ----------------------------------------------------------------------------
// Connect a block to a record (by pointer).
// ----------------------------------------------------------------------------
unsigned int SIO_record::connect
(
    SIO_block*  block
)
{

//
// Local variables.
//
std::string
    s_name;

//
// Validate the block pointer.
//
if( block == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/] "
                  << "SIO: Block not connected (pointer is NULL)"
                  << std::endl;
    }
    return( SIO_RECORD_BADARGUMENT );
}
s_name = *(block->getName());

//
// Go to the workhorse.
//
return( connect( &s_name, block ) );
}

// ----------------------------------------------------------------------------
// Connect a block to a record (workhorse).                               
// ----------------------------------------------------------------------------
unsigned int SIO_record::connect
(
   std::string*         s_name,
   SIO_block*           block    
)
{

//
// Local variables.
//
std::pair< connectMap_i, bool >
    status;

//
// Initialize the map entry.
//
std::pair< std::string const, SIO_block* >
    entry( *s_name, block );

//
// Insert the new entry.
//

status = connectMap.insert( entry );
if( status.second )
{
  if( verbosity >= SIO_ALL )
    {
        std::cout << "SIO: [/"  << name << "/" << entry.first << "] "
                  << "Connected" 
                  << std::endl;
    }
}
else
{
  //fg:  no need to keep old block handler if user wants to connect a new one

  status.first->second = block ;

    if( verbosity >= SIO_ALL )
    {
        std::cout << "SIO: [/"  << name << "/" << entry.first << "] "
                  << "connected new handler "
                  << std::endl;
    }


//     if( verbosity >= SIO_ERRORS )
//     {
//         std::cout << "SIO: [/"  << name << "/" << entry.first << "] "
//                   << "Already connected"
//                   << std::endl;
//     }
//    return( SIO_RECORD_DUPCONNECT );
}

//
// That's all folks!
//
return( SIO_RECORD_SUCCESS );
}


// ----------------------------------------------------------------------------
// fg: Disconnect all blocks from  record 
// ----------------------------------------------------------------------------
unsigned int SIO_record::disconnectAll() {

  connectMap_i iter;
  
  if( verbosity >= SIO_ALL ) {
    for( iter =  connectMap.begin() ; iter != connectMap.end() ; ++iter) {

    std::cout << "SIO: [/"  << name << "/" << iter->first << "]  "
	      << *(iter->second->getName()) 
	      << "  Disconnected" 
	      << std::endl;
    }
  }
  connectMap.erase(connectMap.begin(), connectMap.end());

  return( SIO_RECORD_SUCCESS );
}
// ----------------------------------------------------------------------------
// Disconnect a block from a record (by name).
// ----------------------------------------------------------------------------
unsigned int SIO_record::disconnect
(
    const char*  i_name
)
{

//
// Local variables.
//
SIO_block
   *block;

std::string
    s_name = i_name;

//
// Validate the name.
//
block = SIO_blockManager::get( i_name );
if( block == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/" << i_name << "] "
                  << "Not disconnected (block name not recognized)"
                  << std::endl;
    }
    return( SIO_RECORD_BADARGUMENT );
}

//
// Go to the workhorse.
//
return( disconnect( &s_name, block ) );
}

// ----------------------------------------------------------------------------
// Disconnect a block from a record (by pointer).
// ----------------------------------------------------------------------------
unsigned int SIO_record::disconnect
(
    SIO_block*  block
)
{

//
// Local variables.
//
std::string
    s_name;

//
// Validate the block pointer.
//
if( block == NULL )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/] "
                  << "Not disconnected (pointer is NULL)"
                  << std::endl;
    }
    return( SIO_RECORD_BADARGUMENT );
}
s_name = *(block->getName());

//
// Go to the workhorse.
//
return( disconnect( &s_name, block ) );
}

// ----------------------------------------------------------------------------
// Disconnect a block from a record (workhorse).
// ----------------------------------------------------------------------------
unsigned int SIO_record::disconnect
(
    std::string*          s_name,
    SIO_block*           /* block */
)
{

//
// Local variables.
//
connectMap_i
    iter;

//
// Use the map's find function.  It the name doesn't exist, return an error.
//
if( (iter = connectMap.find( *s_name )) == connectMap.end() )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/" << *s_name << "] "
                  << "Not disconnected (block is not connected)"
                  << std::endl;
    }
    return( SIO_RECORD_NOTCONNECTED );
}

//
// Remove the connection in the connection map.
//
connectMap.erase( iter );

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [/"  << name << "/" << *s_name << "] "
              << "Disconnected" 
              << std::endl;
}

//
// That's all folks!
//
return( SIO_RECORD_SUCCESS );
}

// ----------------------------------------------------------------------------
// Find a connected block pointer from the name.
// ----------------------------------------------------------------------------
SIO_block* SIO_record::getConnect
(
   const char*   i_name
)
{

//
// Local variables.
//
connectMap_i
    iter;

std::string
    s_name = i_name;

//
// Use the map's find function.  It the name doesn't exist, return NULL.
//
if( (iter = connectMap.find( s_name )) == connectMap.end() )
{
    if( verbosity >= SIO_ERRORS )
    {
        std::cout << "SIO: [/"  << name << "/" << i_name << "] "
                  << "Block is not connected" 
                  << std::endl;
    }
    return( NULL );
}

if( verbosity >= SIO_ALL )
{
    std::cout << "SIO: [/"  << name << "/" << i_name << "] "
              << "Block is connected" 
              << std::endl;
}

//
// That's all folks!
//
return( iter->second );
}

// ----------------------------------------------------------------------------
// Return compression flag.
// ----------------------------------------------------------------------------
bool SIO_record::getCompress()
{
if( (options & SIO_OPT_COMPRESS) != 0 )
    return( true ); 

return( false );
}

// ----------------------------------------------------------------------------
// Return pointer to record name.
// ----------------------------------------------------------------------------
std::string* SIO_record::getName()         { return( &name );     }

// ----------------------------------------------------------------------------
// Return options word.
// ----------------------------------------------------------------------------
unsigned int SIO_record::getOptions()      { return( options );   }

// ----------------------------------------------------------------------------
// Return unpacking flag.
// ----------------------------------------------------------------------------
bool SIO_record::getUnpack()               { return( unpack );    }

// ----------------------------------------------------------------------------
// Return verbosity.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_record::getVerbosity()   { return( verbosity ); }

// ----------------------------------------------------------------------------
// Read a record.
// ----------------------------------------------------------------------------
unsigned int SIO_record::read
(
    SIO_stream*         stream,
    unsigned int        i_options
)
{

//
// Local variables.
//
SIO_block
   *block;

pointedAtMap_i
    pati;

pointerToMap_i
    ptoh,
    ptoi,
    ptol;
    
SIO_POINTER_DECL
   *pointer;

unsigned int
    buflen,
    buftyp,
    status,
    tmplen,
    version;

char
   *tmploc;

//
// Save the options.
//
options = i_options;

//
// Walk along the record buffer unpacking blocks.
//
while( stream->buffer < stream->recmax )
{
    //
    // Set the block maximum marker out of the way while interpreting the
    // block length and type (but even so, don't let reads escape off the
    // total length of the buffer!
    //
    stream->blkmax = stream->recmax;

    //
    // Interpret: 1) The length of the block.
    //            2) The block marker.
    //
    SIO_DATA( stream, &buflen, 1 );
    SIO_DATA( stream, &buftyp, 1 );
    if( buftyp != SIO_mark_block )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << stream->name << "/" << name << "/] "
                      << "Expected block marker not found"
                      << std::endl;
        }
        return( SIO_RECORD_NOBLKMARKER );
    }
    stream->blkmax = stream->buffer + buflen - 8;

    //
    // Read the block version.
    //
    SIO_DATA( stream, &version, 1 );

    //
    // Read and interpret the block name.
    //
    SIO_DATA( stream, &tmplen, 1 );

    tmploc = static_cast<char *>(malloc( tmplen + 1 ));
    if( tmploc == NULL )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  << stream->name << "/" << name << "/] "
                      << "Buffer allocation failed"
                      << std::endl;
        }
        return( SIO_STREAM_NOALLOC );
    }

    //SIO_DATA( stream, tmploc, tmplen );
    //fg: keep coverity happy be deleting the temp memory in case of a read error
    status = SIO_functions::data( stream, tmploc, tmplen ); 
    if( !(status & 1) ) {
      free( tmploc );
      return status;
    }
    tmploc[tmplen]   = '\0';
    block            = SIO_blockManager::get( tmploc );
    stream->blk_name = tmploc;
    free( tmploc );

    //
    // Try to unpack the block.
    //
    if( block != NULL )
    {
        status = block->xfer( stream, SIO_OP_READ, version );
        if( !(status & 1) )
            return( status );
    }

    else
        stream->buffer = stream->blkmax;
}

//
// Pointer relocation on read.
//
// Some of these variables are a little terse!  Expanded meanings:
//
// ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
// ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
// ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
// pati:  Iterator in the 'pointed at' map (search map for ptol->first)
//
ptol  = stream->pointerTo->begin();
while( ptol != stream->pointerTo->end() )
{
    ptoh = stream->pointerTo->upper_bound( ptol->first );
    pati = stream->pointedAt->find( ptol->first );

    bool pat_found( pati != stream->pointedAt->end() ) ;
    
    // if the pointed at object is not found we set the pointer to null
    for( ptoi = ptol; ptoi != ptoh; ptoi++ ) {
      pointer = static_cast     <SIO_POINTER_DECL *>(ptoi->second) ; 
      *pointer = ( pat_found ? reinterpret_cast<SIO_POINTER_DECL  >(pati->second) : 0 ) ;
    }
    
    // if( pati != stream->pointedAt->end() )
    // {
    //     for( ptoi = ptol; ptoi != ptoh; ptoi++ )
    // 	{
    //          pointer = static_cast     <SIO_POINTER_DECL *>(ptoi->second); 
    //         *pointer = reinterpret_cast<SIO_POINTER_DECL  >(pati->second);
    //     }
    // }
    // else{
    //   std::cout << " ----SIO_record: no pointed At object found for : " << ptol->first << std::endl ;
    // }

    ptol = ptoh;
}

//
// That's all folks!
//
return( SIO_RECORD_SUCCESS );
}

// ----------------------------------------------------------------------------
// Set/reset the compression flag.
// ----------------------------------------------------------------------------
bool SIO_record::setCompress
(
    bool     compress
)
{
unsigned int 
    o_options = (options & SIO_OPT_COMPRESS);

               options &= ~SIO_OPT_COMPRESS;
if( compress ) options |=  SIO_OPT_COMPRESS;

if( o_options != 0 )
    return( true ); 

return( false );
}

// ----------------------------------------------------------------------------
// Set/reset the unpacking flag.
// ----------------------------------------------------------------------------
bool SIO_record::setUnpack
(
    bool  i_unpack
)
{ 
  bool o_unpack = unpack; 
  unpack = i_unpack; 
 
 if( verbosity >= SIO_ERRORS ){
   std::cout << "SIO: [/"  << name << "] "
	     << " setting unpack from " << o_unpack << " to  " << unpack << std::endl ;
 }

 return( o_unpack ); 
}

// ----------------------------------------------------------------------------
// Set the verbosity level.
// ----------------------------------------------------------------------------
SIO_verbosity SIO_record::setVerbosity
(
    SIO_verbosity   i_verb
)
{ SIO_verbosity o_verb = verbosity; verbosity = i_verb; return( o_verb ); } 

// ----------------------------------------------------------------------------
// Write a record.
// ----------------------------------------------------------------------------
unsigned int SIO_record::write
(
    SIO_stream*         stream
)
{

//
// Local variables.
//
pointedAtMap_i
    pati;

pointerToMap_i
    ptoh,
    ptoi,
    ptol;
    

connectMap_i
    iter;

unsigned int
    blkver,
    buflen,
    match,
    namlen,
    status;

unsigned char
   *pointer;

const char
   *nampnt;

//
// Loop over blocks, getting their input.
//
for( iter = connectMap.begin(); iter != connectMap.end(); iter++ )
{
    //
    // Save the beginning of block pointer.  Reuse the stream->blkmax variable
    // (which has no use during buffer writing) to save it so that if the copy
    // routine in SIO_functions is forced to reallocate the output buffer,
    // the pointer to the beginning of block can be updated as well.
    // 
    stream->blkmax = stream->buffer;

    //
    // Output: 1) A placeholder where the block length will go.
    //         2) A 'framing' marker (to help in debugging).
    //         3) The version of the block.
    //         4) The length of the block name.
    //         5) The block name.
    //
    SIO_DATA( stream, &SIO_mark_block,             1      );
    SIO_DATA( stream, &SIO_mark_block,             1      );

    blkver = iter->second->version();
    SIO_DATA( stream, &blkver,                     1      );

    namlen = iter->first.size();
    nampnt = iter->first.c_str();

    SIO_DATA( stream, &namlen,                     1      );
    SIO_DATA( stream,  const_cast<char *>(nampnt), namlen );

    //
    // Write the block content.
    //
    status = iter->second->xfer( stream, SIO_OP_WRITE, blkver );
    if( status != SIO_BLOCK_SUCCESS )
    {
        if( verbosity >= SIO_ERRORS )
        {
            std::cout << "SIO: ["  
                      << stream->name << "/" 
                      << name << "/" 
	              << iter->first << "] "
                      << "Write error" 
                      << std::endl;
        }
        return( status );
    }
   
    //
    // Back fill the length of the block.
    //
    buflen = stream->buffer - stream->blkmax;
    SIO_functions::copy( UCHR_CAST(&buflen),
                         stream->blkmax, SIO_LEN_QB, 1 );
}

//
// Pointer relocation on write.
//
// Some of these variables are a little terse!  Expanded meanings:
//
// ptol:  Iterator pointing to lower bound in the 'pointer to' multimap
// ptoh:  Iterator pointing to upper bound in the 'pointer to' multimap
// ptoi:  Iterator for the 'pointer to' multimap (runs [ptol, ptoh) )
// pati:  Iterator in the 'pointed at' map (search map for ptol->first)
//

//
// The next piece of code was just too valuable a debugging tool to toss out!
//
/*
printf("\nPointer to:\n\n" );
for( ptoi  = stream->pointerTo->begin();
     ptoi != stream->pointerTo->end();
     ptoi++ )
{
    printf("%016lx    %016lx\n", ptoi->first, ptoi->second );
}

printf("\nPointed at:\n\n" );
for( pati  = stream->pointedAt->begin();
     pati != stream->pointedAt->end();
     pati++ )
{
    printf("%016lx    %016lx\n", pati->first, pati->second );
}
*/

match = 0x00000001;
ptol  = stream->pointerTo->begin();
while( ptol != stream->pointerTo->end() )
{
    ptoh = stream->pointerTo->upper_bound( ptol->first );
    pati = stream->pointedAt->find( ptol->first );

    if( pati != stream->pointedAt->end() )
    {
        pointer = stream->bufloc + 
                  reinterpret_cast<SIO_POINTER_DECL>(pati->second);

        SIO_functions::copy( UCHR_CAST( &match ), pointer, SIO_LEN_QB, 1 );

        for( ptoi = ptol; ptoi != ptoh; ptoi++ )
	{
            pointer = stream->bufloc + 
                      reinterpret_cast<SIO_POINTER_DECL>(ptoi->second);

            SIO_functions::copy( UCHR_CAST( &match ), pointer, SIO_LEN_QB, 1 );
	}
    }

    match++;
    ptol = ptoh;
}

//
// That's all folks!
//
return( SIO_RECORD_SUCCESS );
}
