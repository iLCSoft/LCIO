//// lXDR.hh
//
// Header file for a light-weight XDR class
// This class does not support the full XDR protocol, and
// neither does it work for all platforms. It was mainly 
// written, in combination with lStdHep, to provide a faster
// alternative to the more cumdersome methods using mcfio in
// CLHEP.
//
// W.G.J. Langeveld, 24 May 2002
//
// Release notes:
// - Version 1.0 (23-Oct-2003)
//
////
#ifndef LXDR__HH
#define LXDR__HH

#include <stdio.h>

namespace UTIL{

////
//
// The main lXDR class.
//
////
class lXDR {
private:
//
// The current version/revision is:
//
   enum { MAJOR = 1, MINOR = 0, DAY = 23, MONTH = 10, YEAR = 2003 };
//       ========================================================
public:
   static int         getMajor(void) { return(MAJOR); };
   static int         getMinor(void) { return(MINOR); };
   static const char *getText(void)  {
      static char buff[80];
      sprintf(buff, "lXDR version %d.%d (%02d.%02d.%d) by W.G.J. Langeveld, SLAC",
              MAJOR, MINOR, DAY, MONTH, YEAR);
      return(buff);
   };
public:
//
// Constructors, destructor
// ------------------------
// Constructor opens file, destructor closes file. Once opened for
// reading, the file cannot be written to, and v.v.
//
   lXDR(const char *filename = 0, bool open_for_write = false);
private: // Prevent copying
   lXDR(const lXDR &);
public:
   virtual ~lXDR();
//
// Change the file being read/written. If another file is currently
// being read or written, it is first closed. The new file position
// is the start of the file.
//
   void        setFileName(const char *filename, bool open_for_write = false);
   const char *getFileName(void) const { return(_fileName); };
//
// Prevent assignment:
//
private:
   lXDR       &operator=(const lXDR &);
public:
//
// Check for errors in the last operation.
//
   long        getError(void) const { return(_error); };
//
// Read data.
// ----------
// The following routines read single longs floats or doubles.
// Check getError() for succes or failure.
//
   long        readLong(void);
   double      readFloat(void);  // Note that this returns a double!!
   double      readDouble(void);
//
// The following routines read the length of an array of char, long or double
// from the file, allocate a suitably large array, and read the data from the
// file. Character strings are null terminated.
// Check getError() for succes or failure.
//
   const char *readString(long &length);
   long       *readLongArray(long &length);
   double     *readFloatArray(long &length); // Note that this returns an array of doubles!!
   double     *readDoubleArray(long &length);
//
// Write data
// ----------
// The following routines write single longs or doubles.
// They return getError().
//
   long        writeLong(long data);
   long        writeDouble(double data);
//
// The following routines write the length of an array of char, long or double
// to the file, then write the data itself.
// The functions return getError().
//
   long        writeString(const char *data);
   long        writeString(const char *data, long length);
   long        writeLongArray(const long *data, long length);
   long        writeDoubleArray(const double *data, long length);

   void        setError(long error) { _error = error; return; };
//
// Set or get (with no arguments) file position.
//
   long        filePosition(long pos = -1);

private:
   char      *_fileName;
   FILE      *_fp;
   long       _error;
   bool       _openForWrite;

   bool       _hasNetworkOrder;
   double     ntohd(double d) const;
   double     htond(double d) const { return(ntohd(d)); };

   long       checkRead(long *);
   long       checkRead(float *);
   long       checkRead(double *);
   long       checkWrite(long *);
   long       checkWrite(double *);
};

#define LXDR_SUCCESS         0
#define LXDR_OPENFAILURE     1
#define LXDR_READONLY        2
#define LXDR_WRITEONLY       3
#define LXDR_NOFILE          4
#define LXDR_READERROR       5
#define LXDR_WRITEERROR      6
#define LXDR_SEEKERROR       7
 
}
#endif
