//// lStdHep.cc
//
// This class is based on the light-weight XDR class lXDR,
// and parses/writes StdHep files. It was mainly written,
// to provide a faster alternative to the more cumbersome
// methods using mcfio in StdHep.
//
// W.G.J. Langeveld, 24 May 2002
//
////
#include "UTIL/lStdHep.hh"
#include "string.h"
#include "stdlib.h"

namespace UTIL{

////
//
// The main lStdHep class.
//
////

//
// Constructors, destructor
// ------------------------
// Constructor opens file, destructor closes file. Once opened for
// reading, the file cannot be written to, and v.v.
//
lStdHep::lStdHep(const char *filename, bool open_for_write) : 
  lXDR(filename, open_for_write),
  ntot(0),version(0),title(0),comment(0),date(0),closingDate(0),numevts_expect(0),numevts(0),
  firstTable(0),dimTable(0),nNTuples(0),nBlocks(0),blockIds(0),blockNames(0)
{
   if (open_for_write) {
      setError(LSH_NOTSUPPORTED);
   }
   else {
      readFileHeader();
   }
   return;
}

lStdHep::~lStdHep()
{
   delete [] version;
   delete [] date;
   delete [] closingDate;
   delete [] comment;
   delete [] title;
   delete [] blockIds;
   if (blockNames) {
      for (int i = 0; i < nBlocks; i++) {
         delete [] blockNames[i];
      }
      delete [] blockNames;
   }
   return;
}

void lStdHep::printFileHeader(FILE *fp)
{
   if (fp == 0) fp = stdout;

   fprintf(fp, "====== File Header ===========\n");
   fprintf(fp, "    total blocks: %ld\n", ntot);
   fprintf(fp, "         version: %s\n",  version);
   fprintf(fp, "           title: %s\n",  title);
   fprintf(fp, "         comment: %s\n",  comment);
   fprintf(fp, "            date: %s",    date);
   fprintf(fp, "    closing date: %s",    closingDate);
   fprintf(fp, " expected events: %ld\n", numevts_expect);
   fprintf(fp, "          events: %ld\n", numevts);
   fprintf(fp, "      firstTable: %ld\n", firstTable);
   fprintf(fp, "        dimTable: %ld\n", dimTable);
   fprintf(fp, "        nNTuples: %ld\n", nNTuples);
   fprintf(fp, "         nBlocks: %ld\n", nBlocks);
   if (nBlocks) fprintf(fp, "     block names:\n");
   for (int i = 0; i < nBlocks; i++) {
      fprintf(fp, "                : %s\n", blockNames[i]);
   }
   fprintf(fp, "=============================\n");
   return;
}

void lStdHep::printEventTable(FILE *fp)
{
   if (fp == 0) fp = stdout;
   eventTable.print(fp);
   return;
}

void lStdHep::printEventHeader(FILE *fp)
{
   if (fp == 0) fp = stdout;
   event.printHeader(fp);
   return;
}

void lStdHep::printEvent(FILE *fp)
{
   if (fp == 0) fp = stdout;
   event.print(fp);
   return;
}

void lStdHep::printTrack(int i, FILE *fp)
{
   if (fp == 0) fp = stdout;
   if (i < event.nhep) {
      fprintf(fp, "    Track: id: %ld, vtx: (%g, %g, %g, %g), mom: (%g, %g, %g, %g, %g)\n",
                   pid(i), X(i), Y(i), Z(i), T(i), Px(i), Py(i), Pz(i), E(i), M(i));
      fprintf(fp, "    Track: wgt: %g, alpha QED: %g, alpha QCD: %g, idrup: %ld\n",
                   eventweight(), alphaQED(), alphaQCD(), idrup());
   }
   return;
}

void lStdHep::printBeginRunRecord(FILE *fp)
{
   if (fp == 0) fp = stdout;
   return;
}

void lStdHep::printEndRunRecord(FILE *fp)
{
   if (fp == 0) fp = stdout;
   return;
}

bool lStdHep::more(void)
{
   return(getError() == LSH_SUCCESS);
}

long lStdHep::readEvent(void)
{
//
// Look for an event or an event table
//
   event.isEmpty = 1;
   while (1) {
      if (eventTable.ievt < eventTable.numEvts) {
         if (filePosition(eventTable.ptrEvents[eventTable.ievt]) !=
                          eventTable.ptrEvents[eventTable.ievt]) return(getError());

         if (event.read(*this) != LSH_SUCCESS) return(getError());
         eventTable.ievt++;

         if (event.isEmpty) continue;
         return(getError());
      }

      eventTable.isEmpty = 1;
      while (eventTable.isEmpty) {
         if (eventTable.nextlocator == -2) {
//
// This was the last event table, signal quitting. Not an error.
//
            setError(LSH_ENDOFFILE);
            return(getError());
         }
         else if (eventTable.nextlocator == -1) {
            setError(LSH_EVTABLECORRUPT);
            return(getError());
         }
//
// Go to the next event table
//
         if (filePosition(eventTable.nextlocator) != eventTable.nextlocator) return(getError());
         if (eventTable.read(*this) != LSH_SUCCESS) return(getError());
      }
   }
   return(getError());
}

long lStdHep::getEvent(lStdEvent &lse) const
{
   if (long status = getError() != LSH_SUCCESS) return(status);

   lse.evtNum  = event.nevhep;

   lse.clear();
   for (int i = 0; i < event.nhep; i++) {
      lStdTrack lst;
      lst.X         = X(i);
      lst.Y         = Y(i);
      lst.Z         = Z(i);
      lst.T         = T(i);
      lst.Px        = Px(i);
      lst.Py        = Py(i);
      lst.Pz        = Pz(i);
      lst.E         = E(i);
      lst.M         = M(i);
      lst.pid       = pid(i);
      lst.status    = status(i);
      lst.mother1   = mother1(i);
      lst.mother2   = mother2(i);
      lst.daughter1 = daughter1(i);
      lst.daughter2 = daughter2(i);
      lse.push_back(lst);
   }
   return(LSH_SUCCESS);
}

long lStdHep::readEvent(lStdEvent &lse)
{
   long status = readEvent();
   if (status != LSH_SUCCESS) return(status);
   return(getEvent(lse));
}

long lStdHep::readFileHeader(void)
{
   long len, blockid;

   blockid   = readLong();
   if (blockid != LSH_FILEHEADER) {
      setError(LSH_BLOCKERROR);
      return(getError());
   }
   ntot      = readLong();
   version   = readString(len);

   title     = readString(len);
   comment   = readString(len);
   date      = readString(len);
   if ((strcmp(version, "2.00") == 0) || (strcmp(version, "1.00") == 0)) {
      closingDate = new char[len + 1];
      strcpy((char *) closingDate, date);
   }
   else {
      closingDate = readString(len);
   }

   numevts_expect = readLong();
   numevts        = readLong();
   firstTable     = readLong();
   dimTable       = readLong();
   nBlocks        = readLong();
   if (*version != '2') {
      nNTuples    = 0;
   }
   else {
      nNTuples     = readLong();
   }

   blockIds       = readLongArray(nBlocks);
   blockNames     = new const char *[nBlocks];
   for (int i = 0; i < nBlocks; i++) blockNames[i] = readString(len);
   if (nNTuples > 0) setError(LSH_NOTSUPPORTED);
//
// Read the first event table
//
   eventTable.read(*this);
   return(getError());
}

long lStdHep::writeEvent(void)
{
   return(LSH_NOTSUPPORTED);
}

long lStdHep::setEvent(const lStdEvent &lse)
{
// ***************set up event buffer!
   setNTracks(lse.size());
   setEvtNum(lse.evtNum);

   for (int i = 0; i < event.nhep; i++) {
      setX        (i, lse[i].X);
      setY        (i, lse[i].Y);
      setZ        (i, lse[i].Z);
      setT        (i, lse[i].T);
      setPx       (i, lse[i].Px);
      setPy       (i, lse[i].Py);
      setPz       (i, lse[i].Pz);
      setE        (i, lse[i].E);
      setM        (i, lse[i].M);
      setPid      (i, lse[i].pid);
      setStatus   (i, lse[i].status);
      setMother1  (i, lse[i].mother1);
      setMother2  (i, lse[i].mother2);
      setDaughter1(i, lse[i].daughter1);
      setDaughter2(i, lse[i].daughter2);
   }
   return(LSH_SUCCESS);
}

long lStdHep::writeEvent(lStdEvent &lse)
{
   long status = writeEvent();
   if (status != LSH_SUCCESS) return(status);
   return(setEvent(lse));
}

lStdHep::EventTable::EventTable() :
   isEmpty(1), ievt(0), blockid(0), ntot(0), version(0),
   nextlocator(-3), numEvts(0), evtnums(0),
   storenums(0), runnums(0), trigMasks(0), ptrEvents(0)
{
   return;
}

lStdHep::EventTable::~EventTable()
{
   cleanup();
   return;
}

void lStdHep::EventTable::cleanup(void)
{
   delete [] version;    version   = 0;
   delete [] evtnums;    evtnums   = 0;
   delete [] storenums;  storenums = 0;
   delete [] runnums;    runnums   = 0;
   delete [] trigMasks;  trigMasks = 0;
   delete [] ptrEvents;  ptrEvents = 0;
   isEmpty = 1;
   ievt    = ntot = blockid = numEvts = 0; // leave nextlocator alone!
   return;
}

long lStdHep::EventTable::read(lStdHep &ls)
{
   long len;

   cleanup();

   blockid  = ls.readLong();
   ntot     = ls.readLong();
   version  = ls.readString(len);

   if (blockid != LSH_EVENTTABLE) {
      ls.setError(LSH_NOEVENTTABLE);
      return(ls.getError());
   }
   nextlocator = ls.readLong();
   numEvts     = ls.readLong();
   evtnums     = ls.readLongArray(len);
   storenums   = ls.readLongArray(len);
   runnums     = ls.readLongArray(len);
   trigMasks   = ls.readLongArray(len);
   ptrEvents   = ls.readLongArray(len);
   if (numEvts > 0) isEmpty = 0;
   return(ls.getError());
}

long lStdHep::EventTable::print(FILE *fp)
{
   fprintf(fp, " EventTable: blockid: %ld, ntot: %ld, version: %s\n", blockid, ntot, version);
   fprintf(fp, " EventTable: nextlocator: %ld, numEvts: %ld\n", nextlocator, numEvts);
   for (int i = 0; i < numEvts; i++) {
      fprintf(fp, " EventTable: %d: evtnums %ld storenums %ld runnums %ld trigMasks %ld ptrEvents %ld\n",
               i, evtnums[i], storenums[i], runnums[i], trigMasks[i], ptrEvents[i]);
      if (i == 10) {
         fprintf(fp, " EventTable: etc.\n");
         break;
      }
   }
   return(0);
}

lStdHep::Event::Event() :
   isEmpty(0), blockid(0),ntot(0),version(0),evtnum(0),storenum(0),runnum(0),
   trigMask(0),nBlocks(0),dimBlocks(0),nNTuples(0),dimNTuples(0),blockIds(0),
   ptrBlocks(0),nevhep(0),nhep(0),isthep(0),idhep(0),jmohep(0),jdahep(0),phep(0),
   vhep(0),eventweight(0),alphaqed(0),alphaqcd(0),scale(0),spin(0),colorflow(0),idrup(0),
   bnevtreq(0),bnevtgen(0),bnevtwrt(0),bstdecom(0),bstdxsec(0),bstdseed1(0),bstdseed2(0),
   enevtreq(0),enevtgen(0),enevtwrt(0),estdecom(0),estdxsec(0),estdseed1(0),estdseed2(0)
 
{
   return;
}

lStdHep::Event::~Event()
{
   cleanup();
}

void lStdHep::Event::cleanup(void)
{
   delete [] version;     version   = 0;
   delete [] ptrBlocks;   ptrBlocks = 0;
   delete [] blockIds;    blockIds  = 0;
   delete [] isthep;      isthep    = 0;
   delete [] idhep;       idhep     = 0;
   delete [] jmohep;      jmohep    = 0;
   delete [] jdahep;      jdahep    = 0;
   delete [] phep;        phep      = 0;
   delete [] vhep;        vhep      = 0;
   delete [] scale;       scale     = 0;
   delete [] spin;        spin      = 0;
   delete [] colorflow;   colorflow = 0;
   blockid = ntot = nevhep = nhep = 0;
   isEmpty = 1;
   return;
}

long lStdHep::Event::read(lStdHep &ls)
{
//
// Read event header
//
   long len;

   cleanup();

   blockid = ls.readLong();
   ntot    = ls.readLong();
   version = ls.readString(len);
   if (blockid != LSH_EVENTHEADER) ls.setError(LSH_NOEVENT);

   evtnum    = ls.readLong();
   storenum  = ls.readLong();
   runnum    = ls.readLong();
   trigMask  = ls.readLong();
   nBlocks   = ls.readLong();
   dimBlocks = ls.readLong();

   if (*version == '2') {
      nNTuples = ls.readLong();
      dimNTuples = ls.readLong();
      if (dimBlocks) {
         blockIds  = ls.readLongArray(len);
         ptrBlocks = ls.readLongArray(len);
      }
      if (dimNTuples) {
         ls.setError(LSH_NOTSUPPORTED);
         return(ls.getError());
      }
   }
   else {
      nNTuples   = 0;
      dimNTuples = 0;
      blockIds   = ls.readLongArray(len);
      ptrBlocks  = ls.readLongArray(len);
   }
//
// Read event
//
   for (int i = 0; i < nBlocks; i++) {
      blockid = ls.readLong();
      ntot    = ls.readLong();
      if (version) delete [] version;
      version = ls.readString(len);

      isEmpty = 0;
      switch (blockIds[i]) {
         case LSH_STDHEP          : // 101
            nevhep = ls.readLong();
            nhep   = ls.readLong();
            if (isthep) delete [] isthep;
            isthep = ls.readLongArray(len);
            if (idhep)  delete [] idhep;
            idhep  = ls.readLongArray(len);
            if (jmohep) delete [] jmohep;
            jmohep = ls.readLongArray(len);
            if (jdahep) delete [] jdahep;
            jdahep = ls.readLongArray(len);
            if (phep)   delete [] phep;
            phep   = ls.readDoubleArray(len);
            if (vhep)   delete [] vhep;
            vhep   = ls.readDoubleArray(len);
            break;
         case LSH_STDHEPEV4       : // 201
            nevhep = ls.readLong();
            nhep   = ls.readLong();
            if (isthep) delete [] isthep;
            isthep = ls.readLongArray(len);
            if (idhep)  delete [] idhep;
            idhep  = ls.readLongArray(len);
            if (jmohep) delete [] jmohep;
            jmohep = ls.readLongArray(len);
            if (jdahep) delete [] jdahep;
            jdahep = ls.readLongArray(len);
            if (phep)   delete [] phep;
            phep   = ls.readDoubleArray(len);
            if (vhep)   delete [] vhep;
            vhep   = ls.readDoubleArray(len);
//
// New stuff for STDHEPEV4:
//
            eventweight = ls.readDouble();
            alphaqed    = ls.readDouble();
            alphaqcd    = ls.readDouble();
            if (scale) delete [] scale;
            scale       = ls.readDoubleArray(len);
            if (spin)  delete [] spin;
            spin        = ls.readDoubleArray(len);
            if (colorflow) delete [] colorflow;
            colorflow   = ls.readLongArray(len);
            idrup       = ls.readLong();
            break;
         case LSH_OFFTRACKARRAYS  : // 102
         case LSH_OFFTRACKSTRUCT  : // 103
         case LSH_TRACEARRAYS     : // 104
         case LSH_STDHEPM         : // 105
            break;
         case LSH_STDHEPBEG       : // 106
            bnevtreq  = ls.readLong();
            bnevtgen  = ls.readLong();
            bnevtwrt  = ls.readLong();
            bstdecom  = ls.readFloat();
            bstdxsec  = ls.readFloat();
            bstdseed1 = ls.readDouble();
            bstdseed2 = ls.readDouble();
            isEmpty  = 1;
            break;
         case LSH_STDHEPEND       : // 107
            enevtreq  = ls.readLong();
            enevtgen  = ls.readLong();
            enevtwrt  = ls.readLong();
            estdecom  = ls.readFloat();
            estdxsec  = ls.readFloat();
            estdseed1 = ls.readDouble();
            estdseed2 = ls.readDouble();
            isEmpty = 1;
            break;
         case LSH_STDHEPCXX       : // 108
            break;
      }
   }
   return(ls.getError());
}

long lStdHep::Event::printHeader(FILE *fp)
{
   fprintf(fp, "  EventHeader: blockid: %ld, ntot: %ld, version: %s\n", blockid, ntot, version);
   fprintf(fp, "             : evtnum: %ld, storenum: %ld, runnum: %ld, trigMask: %ld, nBlocks: %ld, dimBlocks: %ld\n",
           evtnum, storenum, runnum, trigMask, nBlocks, dimBlocks);
   fprintf(fp, "             : nNTuples: %ld, dimNTuples: %ld\n", nNTuples, dimNTuples);

   for (int i = 0; i < nBlocks; i++) {
      const char *labels[10] = {"Event", "Off-track arrays", "Off-track struct", "Trace arrays",
                                "Event with multiple interactions", "Begin run", "End run", "StdHepCXX",
                                "EventV4", "Unknown" };
      int j = blockIds[i] - 101;
      if (blockIds[i] == LSH_STDHEPEV4) j = 8;
      if ((j < 0) || (j > 9)) j = 9;
      fprintf(fp, "             : %d: blockIds %ld (%s) ptrBlocks %ld\n",
              i, blockIds[i], labels[j], ptrBlocks[i]);
   }
   return(0);
}

long lStdHep::Event::print(FILE *fp)
{
   fprintf(fp, "   Event: nevhep: %ld, nhep: %ld\n", nevhep, nhep);
   return(0);
}

}
