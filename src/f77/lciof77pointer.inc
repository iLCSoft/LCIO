****************************************************
*  Include file for the low level Fortran interface 
*  of LCIO. 
*  Definition of PTRTYPE which may be architecture 
*  dependent (taken from sio package)
*  must be included from includes using PTRTYPE
*
*  H.Vogt
*  03/19/2004
***************************************************

C Check for 32/64 bit architetures

** ----------------------------------------------------------------------------
** Deal with pointer length.  Currently, the only problem is alpha which uses
** 64 bit pointers.                                                          
**                                                              
** OS            CPU           Macro         Provided by   Pointer size
** ------------  ------------  ------------  ------------  -----------
** AIX           PPC(?)        _AIX          GNU compiler  4 bytes
** OSF1          Alpha         __alpha__     GNU compiler  8 bytes
** Linux         x86           __i386__      GNU compiler  4 bytes
** Linux         Opteron       _LP64         GNU compiler  8 bytes
** Linux         Itanium       _LP64         GNU compiler  8 bytes
** SunOS         Sparc         __sparc__     GNU compiler  4 bytes
** Windows/NT    Alpha         _M_ALPHA      VC  compiler  8 bytes
** Windows/NT    x86           _M_IX86       VC  compiler  4 bytes
** Windows/NT    MIPS          _M_MRX000     VC  compiler  ? bytes
** Windows/NT    PPC           _M_PPC        VC  compiler  4 bytes
** ----------------------------------------------------------------------------
#if defined(__alpha__) || defined(_M_ALPHA) || defined(_LP64)
#define PTRTYPE integer*8
#endif

#if defined(_AIX)      || defined(__i386__)  || defined(__sparc__) || defined(_M_IX86) || defined(_M_PPC)
#define PTRTYPE integer
#endif


C fallback if no arch string is defined 
#ifndef PTRTYPE
#ifdef __SIZEOF_SIZE_T__
C guess sizeof(size_t) is probably sizeof(void*)
#define PTRTYPE integer*__SIZEOF_SIZE_T__
#else
C make a random guess, tends to work on 32 bit systems
#define PTRTYPE integer
#endif
! ifdef __SIZEOF_SIZE_T__
#endif 
! ifndef  PTRTYPE
