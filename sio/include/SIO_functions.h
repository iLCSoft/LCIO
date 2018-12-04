// ----------------------------------------------------------------------------
// CVS $Id: SIO_functions.h,v 1.6 2010-07-14 18:13:08 jeremy Exp $
// ----------------------------------------------------------------------------
// => Primitive functions for reading/writing SIO streams         
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_funtions provides the primitive functions for reading from/writing to
// SIO streams.
//
// ----------------------------------------------------------------------------

#ifndef SIO_FUNCTIONS_H
#define SIO_FUNCTIONS_H 1

//
// Take the drudgery out of error handling.
//
#ifdef SIO_TS_IMPL

#define SIO_DATA( rec, pnt, cnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_data(pnt, cnt) : \
    rec->read_data(pnt, cnt); \
  if( !(status & 1) ) \
    return status; \
}
#define SIO_PNTR( rec, pnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_pointer_to( (SIO_POINTER_DECL *)(pnt) ) : \
    rec->read_pointer_to( (SIO_POINTER_DECL *)(pnt) ); \
  if( !(status & 1) ) \
    return status; \
}
#define SIO_PTAG( rec, pnt ) { \
  status = (rec->mode() != SIO_MODE_READ) ? \
    rec->write_pointed_at( (SIO_POINTER_DECL *)(pnt) ) : \
    rec->read_pointed_at( (SIO_POINTER_DECL *)(pnt) ); \
  if( !(status & 1) ) \
    return status; \
}
#else

#define SIO_DATA( rec, pnt, cnt ) status = SIO_functions::data( (rec), (pnt), (cnt) ); if( !(status & 1) ) return status;
#define SIO_PNTR( rec, pnt )   status = SIO_functions::pointer_to( (rec), (SIO_POINTER_DECL *)(pnt) );   if( !(status & 1) )       return status;
#define SIO_PTAG( rec, pnt )   status = SIO_functions::pointed_at( (rec), (SIO_POINTER_DECL *)(pnt) );   if( !(status & 1) )       return status;

#endif

class SIO_stream;

class SIO_functions
{
public:

    static unsigned int data( SIO_stream*,          char*,         const int );
    static unsigned int data( SIO_stream*, unsigned char*,         const int );
    static unsigned int data( SIO_stream*,          short*,        const int );
    static unsigned int data( SIO_stream*, unsigned short*,        const int );
    static unsigned int data( SIO_stream*,          int*,          const int );
    static unsigned int data( SIO_stream*, unsigned int*,          const int );
    static unsigned int data( SIO_stream*,          SIO_64BITINT*, const int );
    static unsigned int data( SIO_stream*, unsigned SIO_64BITINT*, const int );
    static unsigned int data( SIO_stream*,          float*,        const int );
    static unsigned int data( SIO_stream*,          double*,       const int );

    static unsigned int pointed_at( SIO_stream*, SIO_POINTER_DECL* );
    static unsigned int pointer_to( SIO_stream*, SIO_POINTER_DECL* );

    static bool validateName( const char* );
    
    static void copy( unsigned char*, unsigned char*, 
                      const int,      const int      );

private:


    static unsigned int xfer( SIO_stream*,    const int, 
                              const int,      unsigned char* );

    friend class SIO_stream;              // Direct access to copy
    friend class SIO_record;              // Direct access to copy
}; 

#endif
