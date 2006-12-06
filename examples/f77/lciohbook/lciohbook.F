********************************************************
*
      program lciohbook
*
*  Simple example that opens an LCIO file and creates
*  an hbook file wit a few histograms for MCParticles
*  using the the fortran interface.
*
*  @author: F. Gaede
*  @date: 11/03/2003
********************************************************
      implicit none
      
c--   hbook/paw common
      integer nwpawc
      real hmemor
      parameter(nwpawc=20000)
      common/pawc/ hmemor(nwpawc)

      integer lrecl, istat, icycle

c--   LCIO interface
#include "lciof77api.inc"

      PTRTYPE reader, event, mcp, mccol
      integer status, ievent
      integer i,j,k, nmcp, lenocc
      real costh, p(3), pxy, ptot, phi
      double precision pdouble(3)
      character*80 filename
      character*80 mcpcolname
      data mcpcolname /'MCParticle'/


      call hlimit(nwpawc)


      lrecl=1024
      call hropen(1,'lciohbk','lcio.hbook','N',lrecl,istat)

c--   book some histograms
      call hbook1(10,'mcp energy',400,0.,100.,0.)
      call hbook1(20,'mcp cos(theta)',100,-1.,1.,0.)
      call hbook1(25,'mcp phi',72,-180.,180.,0.)
      call hbook1(30,'mcp e/p',50,0.8,2.,0.)



c--   open LCIO file
ccc      write(*,*) 'Give LCIO file name:'
      read(*,'(a)') filename
      write(*,*) 'input file is: ', filename(1:lenocc(filename))
      write(*,*) 'histo file is: ', 'lcio.hbook'

      reader = lcrdrcreate()
      status = lcrdropen( reader, filename )
      
      if( status.eq.LCIO_ERROR) then
         write(*,*) ' could not open file '// filename
         goto 99
      endif
      

c     ---- event loop ------
      do 
         
         event = lcrdrreadnextevent( reader , LCIO_READ_ONLY )
         if( event.eq.0 ) goto 11 
         
         mccol = lcevtgetcollection( event, mcpcolname )
         if( mccol.eq. 0) then 
            write(*,*) ' collection MCParticle not found in event' 
         else
            nmcp = lccolgetnumberofelements( mccol )
            
            do i=1,nmcp
               
               mcp = lccolgetelementat( mccol, i )
               if( mcp.ne.0 ) then
                  
c     --------- use only final state particles
                  if( (lcmcpgetnumberofdaughters( mcp ).eq.0).and.
     $                 ( lcmcpgetpdg( mcp).gt.100 ).and.
     $                 ( lcmcpgetnumberofparents( mcp ).ne.0) ) then
                     continue
                  else
                     goto 10
                  endif
                  
                  call hfill( 10, real(lcmcpgetenergy(mcp)), 0., 1.)
                  status = lcmcpgetmomentum( mcp , pdouble )
                  p(1) = pdouble(1)
                  p(2) = pdouble(2)
                  p(3) = pdouble(3)
                  
                  pxy = sqrt( p(1)*p(1)+p(2)*p(2) ) 
                  ptot = sqrt( p(1)*p(1)+p(2)*p(2)+p(3)*p(3) )
                  costh = pxy/ptot*p(3)/abs(p(3))
                  
                  call hfill( 20, costh , 0. , 1. )
                  
                  phi = atan2( p(1) , p(2) ) 
                  call hfill( 25, phi*180./3.14159 , 0. , 1. )
                  

                  if( -.00001.lt.phi.and.phi.lt.0.00001) then
                     write(*,*) phi, ' - ' ,lcmcpgetpdg( mcp )
                  endif

                  call hfill
     &              ( 30, real(lcmcpgetenergy(mcp))/ptot , 0., 1.)

               endif
 10            continue
            enddo
         endif
      enddo
 11   continue

c     ---- end event loop ------
      

      status = lcrdrclose( reader )

 99   continue

c--   write and close hbook file
      call hrout(0,icycle,' ')
      call rzstat(' ',999,' ')
      call hrend('lciohbk')

      end



