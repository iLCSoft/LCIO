//// lXDR.cc
//
// Simple XDR class, see header
//
// WGL, 24 May 2002
//
////
#include "UTIL/lXDR.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__APPLE_CC__)
#include "/usr/include/sys/types.h"
#endif

#if defined(__linux) || defined(__CYGWIN__) || defined(__APPLE_CC__)
#include <netinet/in.h>
#endif
#ifdef _MSC_VER
#include <winsock.h>
#else
#include <sys/socket.h>
#endif

namespace UTIL{
////
//
// Constructor, destructor
//
////
lXDR::~lXDR()
{
   if (_fp) {
      fclose(_fp);
      _fp = 0;
   }
   if (_fileName) {
      delete [] _fileName;
      _fileName = 0;
   }
   return;
}

lXDR::lXDR(const char *filename, bool open_for_write) : _fileName(0), _fp(0) 
{
   setFileName(filename, open_for_write);
   if (htonl(1L) == 1L) _hasNetworkOrder = true;
   else                 _hasNetworkOrder = false;
   return;
}

void lXDR::setFileName(const char *filename, bool open_for_write)
{
//
// First check if we can open this file
//
   if (filename == 0) {
      _error = LXDR_OPENFAILURE;
      return;
   }
#ifdef _MSC_VER
   FILE *fp = fopen(filename, open_for_write ? "wb" : "rb");
#else
   FILE *fp = fopen(filename, open_for_write ? "w" : "r");
#endif
   if (fp == 0) {
      _error = LXDR_OPENFAILURE;
      return;
   }

   if (_fp) fclose(_fp);
   _fp = fp;

   if (_fileName) {
      delete [] _fileName;
      _fileName = 0;
   }

   int n = strlen(filename);
   _fileName = new char [n + 1];
   strncpy(_fileName, filename, n);
   _fileName[n] = '\0';

   _openForWrite = open_for_write;

   _error = LXDR_SUCCESS;
   return;
}

double lXDR::ntohd(double d) const
{
//
// If we already have network order, we don't swap
//
   if (_hasNetworkOrder == false) {
      union {
         double        d;
         unsigned char b[8];
      } dd;
      int i;

      dd.d = d;
      for (i = 0; i < 4; i++) {
         unsigned char c = dd.b[i];
         dd.b[i]         = dd.b[7 - i];
         dd.b[7 - i]     = c;
      }
      d = dd.d;
   }
   return(d);
}

long lXDR::checkRead(long *l)
{
   if (_openForWrite) return(_error = LXDR_READONLY);
   if (_fp == 0)      return(_error = LXDR_NOFILE);
   if (l) {
      // je: in architectures where long isn't 4 byte long this code crashes
      //long nr;
      //if ((nr = fread(l, 4, 1, _fp)) != 1) return(_error = LXDR_READERROR);
      //*l = ntohl(*l);

      int32_t buf;
      if (fread(&buf, 4, 1, _fp) != 1) return(_error = LXDR_READERROR);
      *l = ((int32_t)ntohl(buf));
   }
   return(LXDR_SUCCESS);
}

long lXDR::checkRead(double *d)
{
   if (_openForWrite) return(_error = LXDR_READONLY);
   if (_fp == 0)      return(_error = LXDR_NOFILE);
   if (d) {
      if (fread(d, 8, 1, _fp) != 1) return(_error = LXDR_READERROR);
      *d = ntohd(*d);
   }
   return(LXDR_SUCCESS);
}

long lXDR::checkRead(float *f)
{
   if (_openForWrite) return(_error = LXDR_READONLY);
   if (_fp == 0)      return(_error = LXDR_NOFILE);
   if (f) {
      if (fread(f, 4, 1, _fp) != 1) return(_error = LXDR_READERROR);
      // je: in architectures where long isn't 4 byte long this code crashes
      //*((long *) f) = ntohl(*((long *) f));

      *((int32_t *) f) = ntohl(*((int32_t *) f));
   }
   return(LXDR_SUCCESS);
}

long lXDR::readLong(void)
{
   long l = 0;
   checkRead(&l);
   return(l);
}

double lXDR::readDouble(void)
{
   double d = 0.0;
   checkRead(&d);
   return(d);
}

double lXDR::readFloat(void)
{
   float f = 0.0;
   checkRead(&f);
   return((double) f);
}

const char *lXDR::readString(long &length)
{
   if (checkRead(&length)) return(0);
   long rl = (length + 3) & 0xFFFFFFFC;
   char *s = new char[rl + 1];
   if (fread(s, 1, rl, _fp) != (unsigned long) rl) {
      _error = LXDR_READERROR;
      delete [] s;
      return(0);
   }
   s[rl] = '\0';
   _error = LXDR_SUCCESS;
   return(s);
}

long *lXDR::readLongArray(long &length)
{
   if (checkRead(&length)) return(0);
   long *s = new long[length];
   // je: in architectures where long isn't 4 byte long this code crashes
   //if (fread(s, 4, length, _fp) != (unsigned long) length) {
   //   _error = LXDR_READERROR;
   //   delete [] s;
   //   return(0);
   //}
   //if (_hasNetworkOrder == false) for (long i = 0; i < length; i++) s[i] = ntohl(s[i]);

   int32_t *buf = new int32_t[length];
   if (fread(buf, 4, length, _fp) != (unsigned long) length) {
       _error = LXDR_READERROR;
       delete [] buf;
       delete [] s;
       return(0);
   }
   for (long i = 0; i < length; i++){
       if (_hasNetworkOrder == false){
           s[i] = ((int32_t)ntohl(buf[i]));
       }
       else{
            s[i] = (long)buf[i];
       }
   }
   delete [] buf;
   _error = LXDR_SUCCESS;
   return(s);
}

double *lXDR::readDoubleArray(long &length)
{
   if (checkRead(&length)) return(0);
   double *s = new double[length];
   if (fread(s, 8, length, _fp) != (unsigned long) length) {
      _error = LXDR_READERROR;
      delete [] s;
      return(0);
   }
   if (_hasNetworkOrder == false) for (long i = 0; i < length; i++) s[i] = ntohd(s[i]);
   _error = LXDR_SUCCESS;
   return(s);
}

double *lXDR::readFloatArray(long &length)
{
   if (checkRead(&length)) return(0);
   long *st = new long[length];
   // je: FIXME this will cause problems in architectures where long isn't 4 byte long
   if (fread(st, 4, length, _fp) != (unsigned long) length) {
      _error = LXDR_READERROR;
      delete [] st;
      return(0);
   }
   double *s = new double[length];
   // je: FIXME what happens if _hasNetworkOrder == true?!
   if (_hasNetworkOrder == false) {
      for (long i = 0; i < length; i++) {
         long l = ntohl(st[i]);
         s[i] = (double) (*((float *) &l));
      }
   }
   _error = LXDR_SUCCESS;
   return(s);
}

long lXDR::checkWrite(long *l)
{
   if (_openForWrite == false) return(_error = LXDR_WRITEONLY);
   if (_fp == 0)               return(_error = LXDR_NOFILE);
   if (l) {
      long ll = htonl(*l);
      // je: FIXME this will cause problems in architectures where long isn't 4 byte long
      if (fwrite(&ll, 4, 1, _fp) != 4) return(_error = LXDR_WRITEERROR);
   }
   return(LXDR_SUCCESS);
}

long lXDR::checkWrite(double *d)
{
   if (_openForWrite == false) return(_error = LXDR_WRITEONLY);
   if (_fp == 0)               return(_error = LXDR_NOFILE);
   if (d) {
      double dd = htond(*d);
      if (fwrite(&dd, 8, 1, _fp) != 8) return(_error = LXDR_WRITEERROR);
   }
   return(LXDR_SUCCESS);
}

long lXDR::writeLong(long data)
{
   return(checkWrite(&data));
}

long lXDR::writeDouble(double data)
{
   return(checkWrite(&data));
}

long lXDR::writeString(const char *data)
{
   return(writeString(data, strlen(data)));
}

long lXDR::writeString(const char *data, long length)
{
   if (checkWrite(&length)) return(_error);
   if (fwrite(data, 1, length, _fp) != (unsigned long) length) return(_error = LXDR_WRITEERROR);
   long l = ((length + 3) & 0xFFFFFFFC) - length;
   if (fwrite(&l, 1, l, _fp) != (unsigned long) l) return(_error = LXDR_WRITEERROR);
   return(_error = LXDR_SUCCESS);
}

long lXDR::writeLongArray(const long *data, long length)
{
   if (checkWrite(&length)) return(_error);
   long *s = (long *) data;
   if (_hasNetworkOrder == false) {
      s = new long[length];
      for (long i = 0; i < length; i++) s[i] = htonl(data[i]);
   }
   // je: FIXME this will cause problems in architectures where long isn't 4 byte long
   long l = fwrite(s, 4, length, _fp);
   if (_hasNetworkOrder == false) delete [] s;
   if (l != length) return(_error = LXDR_WRITEERROR);
   return(_error = LXDR_SUCCESS);
}

long lXDR::writeDoubleArray(const double *data, long length)
{
   if (checkWrite(&length)) return(_error);
   double *s = (double *) data;
   if (_hasNetworkOrder == false) {
      s = new double[length];
      for (long i = 0; i < length; i++) s[i] = htond(data[i]);
   }
   long l = fwrite(s, 8, length, _fp);
   if (_hasNetworkOrder == false) delete [] s;
   if (l != length) return(_error = LXDR_WRITEERROR);
   return(_error = LXDR_SUCCESS);
}


long lXDR::filePosition(long pos)
{
   if (_fp == 0) {
      _error = LXDR_NOFILE;
      return(-1);
   }
   if (pos == -1) return(ftell(_fp));
   if (fseek(_fp, pos, SEEK_SET)) {
      _error = LXDR_SEEKERROR;
      return(-1);
   }
   return(pos);
}

}// end namespace
