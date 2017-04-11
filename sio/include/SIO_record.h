// ----------------------------------------------------------------------------
// CVS $Id: SIO_record.h,v 1.2 2004-12-23 13:24:07 gaede Exp $
// ----------------------------------------------------------------------------
// => Controller for an SIO record.                                
// ----------------------------------------------------------------------------
//
// General Description:
//
// Each SIO_record controls one record type.                
//
// ----------------------------------------------------------------------------

#ifndef SIO_RECORD_H
#define SIO_RECORD_H 1

#include <map>
#include <string>

#include "SIO_definitions.h"

class SIO_stream;
class SIO_block;

typedef std::map< std::string, SIO_block* >           connectMap_c;
typedef std::map< std::string, SIO_block* >::iterator connectMap_i;

#define SIO_OPT_COMPRESS   0x00000001

// ----------------------------------------------------------------------------
// Class SIO_record.
// ----------------------------------------------------------------------------
class SIO_record
{
public:
    unsigned int              connect( const char* );
    unsigned int              connect( SIO_block*  );
    unsigned int           disconnect( const char* );
    unsigned int           disconnect( SIO_block*  );

  //fg20041222 -- added for multiple write streams
    unsigned int           disconnectAll() ;


    SIO_block*             getConnect( const char* );
    bool                   getCompress();
    std::string*           getName();
    bool                   getUnpack();
    SIO_verbosity          getVerbosity();  
    bool                   setCompress( bool );           
    bool                   setUnpack( bool );
    SIO_verbosity          setVerbosity( SIO_verbosity );  

private:
    SIO_record( const char*, SIO_verbosity );
   ~SIO_record();

    unsigned int           connect( std::string*, SIO_block* );
    unsigned int           disconnect( std::string*, SIO_block* );
    unsigned int           getOptions();  
    unsigned int           read(  SIO_stream*, unsigned int );
    unsigned int           write( SIO_stream* );

    connectMap_c           connectMap{};    // Map of connected blocks
    std::string            name;            // Record name
    unsigned int           options{0};      // Options (flag word)   
    bool                   unpack{false};   // Unpack incoming records?
    SIO_verbosity          verbosity{};     // Reporting level

friend class SIO_recordManager;           // Access to constructor/destructor
friend class SIO_stream;                  // Access to read/write/getOptions
}; 
#endif
