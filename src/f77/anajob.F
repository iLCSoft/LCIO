***********************************************************
*
      program anajob
*
*     A program to demonstrate the use of the
*     fortran interface to LCIO for reading LCIO files.
*
*     H. Vogt  30/09/2003
*     modified by F. Gaede 10/30/2003
***********************************************************
      implicit none

#include "lciof77api.inc"
#include "lciof77apiext.inc"

      PTRTYPE reader, event, runhdr
      
      integer status, irun, ievent

      reader = lcrdrcreate()
c      write(*,*) " created reader at " , reader
      
      status = lcrdropen( reader, 'f_simjob.slcio' )
      
      if( status.eq.0) then
         write(*,*) " could not open file simjob.slcio "
         goto 99
      else
         write(*,*) " opened file simjob.slcio "
      endif
      

c---- first loop over runs and print information
      do 
         runhdr = lcrdrreadnextrunheader( reader, LCIO_READ_ONLY )
         if( runhdr.eq.0 ) goto 9
         
         write(*,*) "  Run : ",lcrhdgetrunnumber( runHdr )
     $        , lcrhdgetdetectorname( runHdr )
     $        , lcrhdgetdescription( runHdr )
         
      enddo
 9    continue

      status = lcrdrclose( reader ) 

      status = lcrdropen( reader, 'simjob.slcio' )
      
      if( status.eq.0) then
         write(*,*) " could not reopen file simjob.slcio "
         goto 99
      else
         write(*,*) " reopened file simjob.slcio "
      endif


c     ---- event loop ------
      do 
         
         event = lcrdrreadnextevent( reader )
         if( event.eq.0 ) goto 11 
         
         status = lcdumpevent( event )


      enddo
 11   continue
c     ---- end event loop ------


      status = lcrdrclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status

 99   continue
      end

