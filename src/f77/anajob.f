c**********************************************************
c
      program anajob
c
c Trivial test program to demonstrate the use of LCIO 
c from Fortran
c gaede 03/13/2003
c**********************************************************
      implicit none

      integer lfactcreatelcreader, reader
      integer lrdrreadnextevent, event
      integer lrdrOpen, lrdrClose, status
      integer levtgetrunnumber, levtgeteventnumber, runnum, evtnum 
      character*100 levtgetdetectorname , detname
      integer i,j,k

      reader = lfactcreatelcreader()
      write(*,*) " created reader at " , reader


      status = lrdropen( reader, 'simjob.sio' ) 

      if( status.eq.0) then
         write(*,*) " could not open file simjob.sio "
         goto 99
      else
         write(*,*) " opened file simjob.sio "
      endif

c     ---- event loop ------
      do 10
         event = lrdrreadnextevent( reader ) 
         if( event.eq.0 ) goto 11 

c         write(*,*) " read event : " , event
         
         runnum = levtgetrunnumber( event ) 
         evtnum = levtgeteventnumber( event )
         detname = levtgetdetectorname( event ) 
         
         write(*,*) "  run: ",runnum,", evt: ",evtnum, " det: ",detname 
         
 10   continue
 11   continue
c     ---- end event loop ------


      status = lrdrclose( reader )
      write(*,*) " closed file simjob.sio - status : " , status

 99   continue
      end
