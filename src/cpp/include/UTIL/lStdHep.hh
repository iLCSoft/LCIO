//// lStdHep.hh
//
// Header file for a light-weight StdHep class.
// This class is based on the light-weight XDR class lXDR,
// and parses/writes StdHep files. It was mainly written,
// to provide a faster alternative to the more cumdersome
// methods using mcfio in CLHEP.
//
// W.G.J. Langeveld, 24 May 2002
//
// Release notes:
// - Version 1.0 (23-Oct-2003, WGL):
//   o Implements "read" but not "write" functions
//   o Standalone (previous versions depended on sgarray,
//     which comes with the "vec" package in the Lelaps
//     distribution). It now uses one std::vector.
// - Version 1.1 (18-Mar-2004, WGL):
//   o Fixed a file positioning bug which made it fail to
//     read some files.
// - Version 1.2 (18-Mar-2004, WGL):
//   o Fixed a bug where it would skip one event at the
//     boundaries between event blocks.
// - Version 1.3 (19-Mar-2004, WGL):
//   o Improved handling of end-of-file. Now, when there
//     are no more events, readEvent() returns an error.
//     The error code is LSH_ENDOFFILE.
// - Version 1.4 (30-Mar-2004, WGL):
//   o Fixed memory leak
// - Version 1.5 (10-Aug-2004, WGL):
//   o Added numEvents() method by request.
//
////
#ifndef LSTDHEP__HH
#define LSTDHEP__HH

#include "UTIL/lXDR.hh"
#include <vector>


namespace UTIL{

//
// MCFIO block codes
//
#define LSH_GENERIC           0
#define LSH_FILEHEADER        1    // supported 5/28/2002
#define LSH_EVENTTABLE        2    // supported 5/28/2002
#define LSH_SEQUENTIALHEADER  3
#define LSH_EVENTHEADER       4    // supported 5/28/2002
#define LSH_NOTHING           5
//
// lStdHepError codes
//
#define LSH_SUCCESS           0
#define LSH_BLOCKERROR      101
#define LSH_NOEVENTTABLE    102
#define LSH_NOEVENT         103
#define LSH_NOTSUPPORTED    104
#define LSH_EVTABLECORRUPT  105
#define LSH_ENDOFFILE       106
//
// StdHep block codes
//
#define LSH_STDHEP          101    // supported 5/28/2002
#define LSH_OFFTRACKARRAYS  102
#define LSH_OFFTRACKSTRUCT  103
#define LSH_TRACEARRAYS     104
#define LSH_STDHEPM         105
#define LSH_STDHEPBEG       106    // supported 5/28/2002
#define LSH_STDHEPEND       107    // supported 5/28/2002
#define LSH_STDHEPCXX       108
#define LSH_STDHEPEV4       201    // supported 1/23/2006

////
//
// The basic lStdHep track. Note that access controls are
// unneeded here since this is an old standard unlikely to
// change.
//
////
struct lStdTrack {
   double         X;
   double         Y;
   double         Z;
   double         T;
   double         Px;
   double         Py;
   double         Pz;
   double         E;
   double         M;
   long           pid;
   long           status;
   long           mother1;
   long           mother2;
   long           daughter1;
   long           daughter2;
};


////
//
// The basic lStdHep event
//
////
struct lStdEvent : public std::vector<lStdTrack>  {
   long evtNum;
   long nTracks(void) { return(size()); };
};

////
//
// The lStdHep class is the "handle" for the StdHep file, and
// provides the access mechanism to the events.
//
////
class lStdHep : public lXDR {
private:
//
// The current version/revision is:
//
   enum { MAJOR = 2, MINOR = 0, DAY = 23, MONTH = 1, YEAR = 2006 };
//       ========================================================
public:
   static int         getMajor(void) { return(MAJOR); };
   static int         getMinor(void) { return(MINOR); };
   static const char *getText(void)  {
      static char buff[80];
      sprintf(buff, "lStdHep version %d.%d (%02d.%02d.%d) by W.G.J. Langeveld, SLAC",
              MAJOR, MINOR, DAY, MONTH, YEAR);
      return(buff);
   };


//
// Constructors, destructor
// ------------------------
// Constructor opens file, destructor closes file. Once opened for
// reading, the file cannot be written to, and v.v.
//
   lStdHep(const char *filename = 0, bool open_for_write = false);
//
// Prevent copying:
//
private:
   lStdHep(const lStdHep &);
public:
   virtual ~lStdHep();
//
// Prevent assignment:
//
private:
   lStdHep       &operator=(const lStdHep &);
public:
//
// See if there are more events
//
   bool           more(void);
//
// Event reading functions. They return the last error encountered,
// or LSH_SUCCESS.
// - Read the next event into the event buffer:
//
   long           readEvent(void);
//
// - Fill the provided lStdEvent with the current event:
//
   long           getEvent(lStdEvent &lse) const;
//
// - Combine readEvent() with getEvent():
//
   long           readEvent(lStdEvent &lse);
//
// Get the number of events in the input file
//
   long           numEvents()      const { return(numevts);                  };
   long           numEventsExpected()      const { return(numevts_expect);   };

//
// Direct access to the event buffer. Note that using readEvent(void)
// in combination with the functions below is faster than using
// readEvent(lStdEvent &), especially when only a few quantities are
// needed.
//
   long           blockId()         const { return(event.blockid);            };
   long           nTracks(void)     const { return(event.nhep);               };
   long           evtNum(void)      const { return(event.nevhep);             };
   long           runNum(void)      const { return(event.runnum);             };

   double         X(int i)          const { return(event.vhep[i * 4 + 0]);    };
   double         Y(int i)          const { return(event.vhep[i * 4 + 1]);    };
   double         Z(int i)          const { return(event.vhep[i * 4 + 2]);    };
   double         T(int i)          const { return(event.vhep[i * 4 + 3]);    };
   double         Px(int i)         const { return(event.phep[i * 5 + 0]);    };
   double         Py(int i)         const { return(event.phep[i * 5 + 1]);    };
   double         Pz(int i)         const { return(event.phep[i * 5 + 2]);    };
   double         E(int i)          const { return(event.phep[i * 5 + 3]);    };
   double         M(int i)          const { return(event.phep[i * 5 + 4]);    };
   long           pid(int i)        const { return(event.idhep[i]);           };
   long           status(int i)     const { return(event.isthep[i]);          };
   long           mother1(int i)    const { return(event.jmohep[i + i + 0]);  };
   long           mother2(int i)    const { return(event.jmohep[i + i + 1]);  };
   long           daughter1(int i)  const { return(event.jdahep[i + i + 0]);  };
   long           daughter2(int i)  const { return(event.jdahep[i + i + 1]);  };
   double         eventweight(void) const { return(event.eventweight);        };
   double         alphaQED(void)    const { return(event.alphaqed);           };
   double         alphaQCD(void)    const { return(event.alphaqcd);           };
   double         scale(int i, int j) const { return(event.scale[i * 10 + j] ); };
   double         spinX(int i)      const { return(event.spin[i * 3 + 0] );   };
   double         spinY(int i)      const { return(event.spin[i * 3 + 1] );   };
   double         spinZ(int i)      const { return(event.spin[i * 3 + 2] );   };
   long           colorflow(int i, int j)  const { return(event.colorflow[i * 2 + j] ); };
   long           idrup(void)       const { return(event.idrup);              };
//
// Call this to make sure you can call things like scale, spin and colorflow:
//
   bool           isStdHepEv4(void) const { return(event.scale != 0);         };
//
// Event writing functions. They return the last error encountered,
// or LSH_SUCCESS.
// - Write the current event buffer to thefile:
//
   long           writeEvent(void);
//
// - Fill the event buffer with the data from the provided lStdEvent:
//
   long           setEvent(const lStdEvent &lse);
//
// - Combine setEvent() with writeEvent():
//
   long           writeEvent(lStdEvent &lse);
//
// Direct access to the event buffer.
//
   void           setNTracks(long n)          { event.nhep = n; return;               };
   void           setEvtNum(long n)           { event.nevhep = n; return;             };

   void           setX(int i, double x)       { event.vhep[i * 4 + 0] = x; return;    };
   void           setY(int i, double y)       { event.vhep[i * 4 + 1] = y; return;    };
   void           setZ(int i, double z)       { event.vhep[i * 4 + 2] = z; return;    };
   void           setT(int i, double t)       { event.vhep[i * 4 + 3] = t; return;    };
   void           setPx(int i, double px)     { event.phep[i * 5 + 0] = px; return;   };
   void           setPy(int i, double py)     { event.phep[i * 5 + 1] = py; return;   };
   void           setPz(int i, double pz)     { event.phep[i * 5 + 2] = pz; return;   };
   void           setE(int i, double e)       { event.phep[i * 5 + 3] = e; return;    };
   void           setM(int i, double m)       { event.phep[i * 5 + 4] = m; return;    };
   void           setPid(int i, long aPid)     { event.idhep[i] = aPid; return;         };
   void           setStatus(int i, long s)    { event.isthep[i] = s; return;          };
   void           setMother1(int i, long n)   { event.jmohep[i + i + 0] = n; return;  };
   void           setMother2(int i, long n)   { event.jmohep[i + i + 1] = n; return;  };
   void           setDaughter1(int i, long n) { event.jdahep[i + i + 0] = n; return;  };
   void           setDaughter2(int i, long n) { event.jdahep[i + i + 1] = n; return;  };
//
// Informational printout
//
   void           printFileHeader(FILE *fp = 0);
   void           printBeginRunRecord(FILE *fp = 0);
   void           printEndRunRecord(FILE *fp = 0);
   void           printEventTable(FILE *fp = 0);
   void           printEventHeader(FILE *fp = 0);
   void           printEvent(FILE *fp = 0);
   void           printTrack(int i, FILE *fp = 0);

private:
   long           readFileHeader(void);
//
// File Header
//
   long           ntot;
   const char    *version;
   const char    *title;
   const char    *comment;
   const char    *date;
   const char    *closingDate;

   long           numevts_expect;
   long           numevts;
   long           firstTable;
   long           dimTable;
   long           nNTuples;
   long           nBlocks;
   long          *blockIds;
   const char   **blockNames;
//
// Event table
//
   class EventTable {
   public:
      EventTable();
      ~EventTable();
      void cleanup(void);
      long read(lStdHep &ls);
      long print(FILE *fp);
//
// ...Empty flag
//
      long  isEmpty;
//
// Index into the event table
//
      long  ievt;
//
// ...MCFIO header
//
      long  blockid;
      long  ntot;
      const char *version;
//
// ...Location of next table
//
      long  nextlocator;
//
// ...The event table itself
//
      long  numEvts;
      long *evtnums;
      long *storenums;
      long *runnums;
      long *trigMasks;
      long *ptrEvents;
   };
   EventTable     eventTable;
//
// The event
//
   class Event {
   public:
      Event();
      ~Event();
      void cleanup(void);
      long read(lStdHep &ls);
      long printHeader(FILE *fp);
      long print(FILE *fp);
//
// ...Empty flag
//
      long isEmpty;
//
// ...MCFIO header
//
      long blockid;
      long ntot;
      const char *version;
//
// ...Event header:
//
      long    evtnum;
      long    storenum;
      long    runnum;
      long    trigMask;
      long    nBlocks;
      long    dimBlocks;
      long    nNTuples;
      long    dimNTuples;
      long   *blockIds;
      long   *ptrBlocks;
//
// ...Event:
//
      long    nevhep;
      long    nhep;
      long   *isthep;
      long   *idhep;
      long   *jmohep;
      long   *jdahep;
      double *phep;
      double *vhep;
//
// ...New for STDHEPEV4:
//
      double  eventweight;
      double  alphaqed;
      double  alphaqcd;
      double *scale;
      double *spin;
      long   *colorflow;
      long    idrup;
//
// ...Begin run record:
//
      long    bnevtreq;
      long    bnevtgen;
      long    bnevtwrt;
      double  bstdecom;
      double  bstdxsec;
      double  bstdseed1;
      double  bstdseed2;
//
// ...End run record:
//
      long    enevtreq;
      long    enevtgen;
      long    enevtwrt;
      double  estdecom;
      double  estdxsec;
      double  estdseed1;
      double  estdseed2;
   };
   Event          event;
};

} // end namespace

#endif
