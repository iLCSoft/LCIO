// ----------------------------------------------------------------------------
// CVS $Id: SIO_definitions.h,v 1.2 2010-06-22 13:49:54 gaede Exp $
// ----------------------------------------------------------------------------
// => Assorted definitions for the SIO package.
// ----------------------------------------------------------------------------
//
// General Description:
//
// SIO_definitions.h provides all the static definitions for the SIO package.
//
// ----------------------------------------------------------------------------

#ifndef SIO_DEFINITIONS_H
#define SIO_DEFINITIONS_H 1

//
// Error format
//
#define SIO_M_SEVERITY              0x00000007
#define SIO_M_MESSAGE               0x0000FFF8
#define SIO_M_FACILITY              0x0FFF0000

#define SIO_V_SEVERITY              0
#define SIO_V_MESSAGE               3
#define SIO_V_FACILITY              16

//
// Error codes ... stream
//
#define SIO_STREAM_FACILITY         0x08000000
#define SIO_STREAM_SUCCESS          0x08000001
#define SIO_STREAM_NOTFOUND         0x08000014
#define SIO_STREAM_ALREADYOPEN      0x08000024
#define SIO_STREAM_OPENFAIL         0x08000034
#define SIO_STREAM_BADMODE          0x08000044
#define SIO_STREAM_NOTOPEN          0x08000054
#define SIO_STREAM_GOTEOF           0x08000064
#define SIO_STREAM_READONLY         0x08000074
#define SIO_STREAM_WRITEONLY        0x08000084
#define SIO_STREAM_NOSUCHRECORD     0x08000094
#define SIO_STREAM_BADWRITE         0x080000a4
#define SIO_STREAM_BADSTATE         0x080000b4
#define SIO_STREAM_OFFEND           0x080000c4
#define SIO_STREAM_NOALLOC          0x080000d4
#define SIO_STREAM_EOF              0x080000e4
#define SIO_STREAM_NORECMARKER      0x080000f4
#define SIO_STREAM_BADCOMPRESS      0x08000104

//
// Error codes ... record
//
#define SIO_RECORD_FACILITY         0x08010000
#define SIO_RECORD_SUCCESS          0x08010001
#define SIO_RECORD_NOTFOUND         0x08010014
#define SIO_RECORD_BADARGUMENT      0x08010024
#define SIO_RECORD_DUPCONNECT       0x08010034
#define SIO_RECORD_NOTCONNECTED     0x08010044
#define SIO_RECORD_NOBLKMARKER      0x08010054

//
// Error codes ... block
//
#define SIO_BLOCK_FACILITY          0x08020000
#define SIO_BLOCK_SUCCESS           0x08020001
#define SIO_BLOCK_NOTFOUND          0x08020014
#define SIO_BLOCK_SKIP              0x08020024

//
// Handy dandy unit specifiers.
//
#define SIO_KBYTE     0x00000400
#define SIO_MBYTE     0x00100000

//
// To make version handling easier.
//
#define SIO_VERSION_ENCODE( major, minor )  (((major) << 16) + (minor))
#define SIO_VERSION_MINOR( version )         ((version) & 0x0000ffff)
#define SIO_VERSION_MAJOR( version )        (((version) & 0xffff0000) >> 16)

//
// Enumerations for stream mode, status and verbosity.
//
typedef enum {
    SIO_MODE_READ,
    SIO_MODE_WRITE_NEW,
    SIO_MODE_WRITE_APPEND,
    SIO_MODE_READ_WRITE,
    SIO_MODE_UNDEFINED
} SIO_stream_mode; 

typedef enum {
    SIO_OP_READ,
    SIO_OP_WRITE,
    SIO_OP_CLEAR
} SIO_operation; 

typedef enum {
    SIO_STATE_OPEN,
    SIO_STATE_CLOSED,
    SIO_STATE_ERROR
} SIO_stream_state; 

typedef enum {
    SIO_SILENT,
    SIO_ERRORS,
    SIO_ALL
} SIO_verbosity;

#endif






